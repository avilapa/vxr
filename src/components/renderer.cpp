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

#include "../../include/engine/engine.h"
#include "../../include/core/gameobject.h"
#include "../../include/graphics/materials/material.h"
#include "../../include/graphics/ui.h"
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

  void System::Renderer::init()
  {
    
  }

  void System::Renderer::update()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
      // Scene changed
    }
  }

  void System::Renderer::renderUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Renderer Render Update");
    if (!scene_)
    {
      return;
    }

    gpu::Buffer common_uniforms_buffer = Engine::ref().camera()->common_uniforms_.buffer;
    gpu::Buffer lights_uniforms_buffer = Engine::ref().light()->light_uniforms_.buffer;

    DisplayList frame;
    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id())
      {
        continue;
      }
      VXR_TRACE_BEGIN("VXR", "Component Update");
      if (!c->gameObject()->active())
      {
        continue; /// Check in transform system? Create screenData vector?
      }

      ref_ptr<vxr::MeshFilter> mf = c->getComponent<vxr::MeshFilter>();
      if (!mf)
      {
        continue;
      }

      if (!mf->mesh)
      {
        continue;
      }

      if (!c->material)
      { 
        continue;
      }

      ref_ptr<Material> shared_material = c->material->sharedMaterial();

      shared_material->setupTextureTypes(c->material->textures_[c->material->active_material_]);
      if (!shared_material->initialized_)
      {
        shared_material->setup();
      }

      if (!mf->mesh->setup())
      {
        continue;
      }

      VXR_TRACE_BEGIN("VXR", "Send Commands");
      if (shared_material->use_uniforms_)
      {
        VXR_TRACE_BEGIN("VXR", "Fill Uniform Buffer");
        frame.fillBufferCommand()
          .set_buffer(shared_material->gpu_.uniform_buffer)
          .set_data(&c->material->uniforms_)
          .set_size(sizeof(c->material->uniforms_));
        VXR_TRACE_END("VXR", "Fill Uniform Buffer");
        VXR_TRACE_BEGIN("VXR", "Setup Material");
        frame.setupMaterialCommand()
          .set_material(shared_material->gpu_.mat)
          .set_buffer(0, mf->mesh->gpu_.vertex.buffer)
          .set_v_texture(shared_material->gpu_.tex)
          .set_uniform_buffer(0, common_uniforms_buffer)
          .set_uniform_buffer(1, lights_uniforms_buffer)
          .set_uniform_buffer(2, shared_material->gpu_.uniform_buffer)
          .set_model_matrix(c->transform()->worldMatrix());
        VXR_TRACE_END("VXR", "Setup Material");
      }
      else
      {
        VXR_TRACE_BEGIN("VXR", "Setup Material");
        frame.setupMaterialCommand()
          .set_material(shared_material->gpu_.mat)
          .set_buffer(0, mf->mesh->gpu_.vertex.buffer)
          .set_v_texture(shared_material->gpu_.tex)
          .set_uniform_buffer(0, common_uniforms_buffer)
          .set_uniform_buffer(1, lights_uniforms_buffer)
          .set_model_matrix(c->transform()->worldMatrix());
        VXR_TRACE_END("VXR", "Setup Material");
      }
      VXR_TRACE_BEGIN("VXR", "Render");
      ///TODO: Missing instancing
      frame.renderCommand()
        .set_index_buffer(mf->mesh->gpu_.index.buffer)
        .set_count(mf->mesh->indexCount())
        .set_type(mf->mesh->indexFormat());
      VXR_TRACE_END("VXR", "Render");
      VXR_TRACE_END("VXR", "Send Commands");
      VXR_TRACE_END("VXR", "Component Update");
    }
    Engine::ref().submitDisplayList(std::move(frame));
  }

  ref_ptr<System::Renderer> System::Getter<Renderer>::get() 
  {
    return Engine::ref().renderer();
  }
}