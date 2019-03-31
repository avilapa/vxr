// ----------------------------------------------------------------------------------------
// MIT License
// 
// Copyright(c) 2018 Víctor Ávila
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ----------------------------------------------------------------------------------------

#include "../../include/components/renderer.h"

#include "../../include/components/mesh_filter.h"
#include "../../include/components/light.h"
#include "../../include/engine/engine.h"
#include "../../include/core/gameobject.h"
#include "../../include/core/scene.h"
#include "../../include/graphics/materials/material.h"

namespace vxr 
{

  Renderer::Renderer()
  {
    set_name("Renderer");
  }

  Renderer::~Renderer()
  {
  }

  void Renderer::onGUI()
  {
    if (ImGui::TreeNodeEx((void*)(intptr_t)id(), 0, "Material"))
    {
      ImGui::Spacing();
      material->onGUI();
      ImGui::TreePop();
    }
  }

  System::Renderer::Renderer()
  {
  }

  System::Renderer::~Renderer()
  {
  }

  void System::Renderer::renderUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Renderer Render Update");
    transparent_.clear();

    DisplayList frame;
    for (auto &c : components_)
    {
      // Check if the object has to be rendered.
      if (setup(c))
      {
        if (c->material->sharedMaterial()->gpu_.info.blend.enabled)
        {
          transparent_.push_back(c);
          continue;
        }

        // Send render commands.
        render(c, &frame);
      }
    }
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void System::Renderer::renderPostUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Renderer Render Post Update");
    ref_ptr<vxr::Camera> camera = Engine::ref().camera()->main();
    if (!camera)
    {
      return;
    }

    if (camera->clear_flags() != vxr::Camera::ClearFlags::Skybox)
    {
      return;
    }

    DisplayList frame;
    for (auto &c : transparent_) /// Need to sort the transparent objects.
    {
      // Send render commands.
      render(c, &frame);
    }
    Engine::ref().submitDisplayList(std::move(frame));

    if (!scene_->skybox())
    {
      return;
    }

    if (setupSkybox())
    {
      renderSkybox();
    }
  }


  bool System::Renderer::setup(ref_ptr<vxr::Renderer> c)
  {
    VXR_TRACE_SCOPE("VXR", "Setup");

    /// TODO: This should be checked once in Transform System and other Systems should read from a 'screenData' vector.
    if (scene_->id() != c->gameObject()->scene_id() || !c->gameObject()->active())
    {
      return false;
    }

    ref_ptr<vxr::MeshFilter> mesh_component = c->getComponent<vxr::MeshFilter>();
    if (!mesh_component)
    {
      return false;
    }

    ref_ptr<Mesh> mesh = mesh_component->mesh;
    if (!mesh)
    {
      return false;
    }

    if (!c->material)
    {
      return false;
    }

    ref_ptr<mat::Material> shared_material = c->material->sharedMaterial();
    if (!shared_material->setupTextureTypes(c->material->textures()))
    {
      return false;
    }

    if (!shared_material->setup())
    {
      return false;
    }

    if (!mesh->setup())
    {
      return false;
    }

    return true;
  }

  void System::Renderer::render(ref_ptr<vxr::Renderer> c, DisplayList* frame)
  {
    VXR_TRACE_SCOPE("VXR", "Render");

    ref_ptr<mat::Material> shared_material = c->material->sharedMaterial();
    ref_ptr<Mesh> mesh = c->getComponent<vxr::MeshFilter>()->mesh;
    if (shared_material->uniforms_enabled())
    {
      VXR_TRACE_BEGIN("VXR", "Fill Uniform Buffer");
      frame->fillBufferCommand()
        .set_buffer(shared_material->uniformBuffer())
        .set_data(&c->material->uniforms_)
        .set_size(sizeof(c->material->uniforms_));
      VXR_TRACE_END("VXR", "Fill Uniform Buffer");
    }
    VXR_TRACE_BEGIN("VXR", "Setup Material");
    frame->setupMaterialCommand()
      .set_material(shared_material->material())
      .set_buffer(0, mesh->vertexBuffer())
      .set_v_texture(shared_material->textureInput())
      .set_uniform_buffer(0, Engine::ref().camera()->common_uniforms_buffer())
      .set_uniform_buffer(1, Engine::ref().light()->light_uniforms_buffer())
      .set_uniform_buffer(2, ((shared_material->uniforms_enabled()) ? shared_material->uniformBuffer() : gpu::Buffer{}))
      .set_model_matrix(c->transform()->world_transform());
    VXR_TRACE_END("VXR", "Setup Material");
    VXR_TRACE_BEGIN("VXR", "Render");
    frame->renderCommand()
      .set_index_buffer(mesh->indexBuffer())
      .set_count(mesh->indexCount())
      .set_type(mesh->indexFormat());
    ///TODO: Missing instancing.
    VXR_TRACE_END("VXR", "Render");
  }

  bool System::Renderer::setupSkybox()
  {
    VXR_TRACE_SCOPE("VXR", "Setup Skybox");

    ref_ptr<vxr::Renderer> skybox = scene_->skybox()->getComponent<vxr::Renderer>();

    if (skybox->material->texture(0)->loading())
    {
      return false;
    }

    ref_ptr<mat::Material> shared_material = skybox->material->sharedMaterial();
    if (!shared_material->setupTextureTypes(skybox->material->textures()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[WARNING]: Invalid scene skybox textures data.\n");
      return false;
    }

    if (!shared_material->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Invalid scene skybox material data.\n");
      return false;
    }

    ref_ptr<Mesh> cube = scene_->skybox()->getComponent<vxr::MeshFilter>()->mesh;
    if (!cube->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Invalid scene skybox mesh data.\n");
      return false;
    }

    return true;
  }

  void System::Renderer::renderSkybox()
  {
    VXR_TRACE_SCOPE("VXR", "Render Skybox");
    DisplayList frame;
    VXR_TRACE_BEGIN("VXR", "Skybox");
    ref_ptr<vxr::Renderer> skybox = scene_->skybox()->getComponent<vxr::Renderer>();
    ref_ptr<mat::Material> shared_material = skybox->material->sharedMaterial();
    ref_ptr<Mesh> cube = scene_->skybox()->getComponent<vxr::MeshFilter>()->mesh;
    VXR_TRACE_BEGIN("VXR", "Fill Uniform Buffer");
    frame.fillBufferCommand()
      .set_buffer(shared_material->uniformBuffer())
      .set_data(&skybox->material->uniforms_)
      .set_size(sizeof(skybox->material->uniforms_));
    VXR_TRACE_END("VXR", "Fill Uniform Buffer");
    frame.setupMaterialCommand()
      .set_material(shared_material->material())
      .set_buffer(0, cube->vertexBuffer())
      .set_v_texture(shared_material->textureInput())
      .set_uniform_buffer(0, Engine::ref().camera()->common_uniforms_buffer())
      .set_uniform_buffer(1, shared_material->uniformBuffer())
      .set_model_matrix(skybox->transform()->world_transform());
    frame.renderCommand()
      .set_index_buffer(cube->indexBuffer())
      .set_count(cube->indexCount())
      .set_type(cube->indexFormat());
    VXR_TRACE_END("VXR", "Skybox");
    Engine::ref().submitDisplayList(std::move(frame));
  }

  ref_ptr<System::Renderer> System::Getter<Renderer>::get() 
  {
    return Engine::ref().renderer();
  }
}