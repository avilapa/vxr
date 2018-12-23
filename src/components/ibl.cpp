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

#include "../../include/components/ibl.h"

#include "../../include/engine/engine.h"
#include "../../include/core/gameobject.h"
#include "../../include/core/scene.h"
#include "../../include/graphics/materials/standard_pass.h"
#include "../../include/graphics/mesh.h"

namespace vxr 
{

  IBL::IBL()
  {
    set_name("Image Based Light");
    contributes_ = false;
    initialized_ = false;

    build_cubemap_.alloc();

    ref_ptr<Texture> input;
    input.alloc()->load("../../assets/textures/skybox/barcelona_rooftop/barcelona_rooftop_3k.hdr", true);
    build_cubemap_->set_input_texture(0, input);
  }

  IBL::~IBL()
  {
  }

  void IBL::onGUI()
  {
    ImGui::Spacing();
    ImGui::Text("Is contributing to current scene:"); ImGui::SameLine();
    if (contributes_)
    {
      ImGui::PushStyleColor(0, ImVec4(0.12f, 0.9f, 0.3f, 1.0f));
    }
    else
    {
      ImGui::PushStyleColor(0, ImVec4(0.8f, 0.3f, 0.12f, 1.0f));
    }
    ImGui::Text(((contributes_) ? "YES" : "NO"));
    ImGui::PopStyleColor();
    ImGui::Spacing();
    
  }

  ref_ptr<Texture> IBL::cubemap_texture()
  {
    return build_cubemap_->output_texture(0);
  }

  System::IBL::IBL()
  {
    cubemap_.alloc();
  }

  System::IBL::~IBL()
  {

  }

  void System::IBL::set_main(ref_ptr<vxr::IBL> light)
  {
    if (main_ != light)
    {
      main_ = light;
    }
  }

  ref_ptr<IBL> System::IBL::main() const
  {
    return main_;
  }

  void System::IBL::init()
  {
    if (!cubemap_->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not set up IBL cubemap.\n");
      return;
    }
  }

  void System::IBL::update()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
      ref_ptr<Skybox> skybox = Engine::ref().scene()->skybox();
      if (skybox != nullptr)
      {
        set_main(skybox->getComponent<vxr::IBL>());
      }
      // Scene changed
    }
  }

  void System::IBL::renderUpdate()
  {
    if (!main_ || !scene_)
    {
      return;
    }

    VXR_TRACE_SCOPE("VXR", "IBL Render Update");
    
    if (!main_->gameObject()->active() || main_->initialized_)
    {
      return;
    }

    buildCubemap();
    computeIrradiance();
    computePrefiltering();
    integrateBRDF();

    main_->initialized_ = true;
  }

  void System::IBL::renderPostUpdate()
  {
    if (!main_ || !scene_)
    {
      return;
    }

    VXR_TRACE_SCOPE("VXR", "IBL Render Post Update");
  }

  void System::IBL::buildCubemap()
  {
    ref_ptr<mat::RenderPass> shared_render_pass = main_->build_cubemap_->sharedRenderPass();
    if (!shared_render_pass->setupTextureInput(main_->build_cubemap_->input_textures()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Build Cubemap texture input could not be setup correctly.\n");
      return;
    }

    if (!shared_render_pass->setupTextureOutput(main_->build_cubemap_->output_textures(), main_->build_cubemap_->depth_texture()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Build Cubemap texture output could not be setup correctly.\n");
      return;
    }

    if (!shared_render_pass->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Build Cubemap could not be setup correctly.\n");
      return;
    }
    vec2 texture_size = shared_render_pass->gpu_.out_tex[0]->size();

    DisplayList frame;
    for (uint32 faces = 0; faces < 6; ++faces)
    {
      frame.setupViewCommand()
        .set_viewport({ 0, 0, (uint16)texture_size.x, (uint16)texture_size.y })
        .set_framebuffer(shared_render_pass->gpu_.fbo)
        .set_cubemap_target((CubemapTarget::Enum)(CubemapTarget::PositiveX + faces));
      frame.clearCommand()
        .set_color(Color::Blue)
        .set_clear_color(true)
        .set_clear_depth(true);
      frame.fillBufferCommand()
        .set_buffer(shared_render_pass->gpu_.uniform_buffer)
        .set_data(&main_->build_cubemap_->conv[faces])
        .set_size(sizeof(main_->build_cubemap_->conv[faces]));
      frame.setupMaterialCommand()
        .set_material(shared_render_pass->gpu_.mat)
        .set_buffer(0, cubemap_->gpu_.vertex.buffer)
        .set_v_texture(shared_render_pass->gpu_.in_tex)
        .set_uniform_buffer(0, shared_render_pass->gpu_.uniform_buffer);
      frame.renderCommand()
        .set_index_buffer(cubemap_->gpu_.index.buffer)
        .set_count(cubemap_->indexCount())
        .set_type(cubemap_->indexFormat());
    }

    Engine::ref().submitDisplayList(std::move(frame));
  }

  void System::IBL::computeIrradiance()
  {
    DisplayList frame;
    for (uint32 faces = 0; faces < 6; ++faces)
    {

    }
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void System::IBL::computePrefiltering()
  {
    DisplayList frame;
    for (uint32 i = 0; i < 5; ++i)
    {
      for (uint32 faces = 0; faces < 6; ++faces)
      {

      }
    }
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void System::IBL::integrateBRDF()
  {
    DisplayList frame;
    Engine::ref().submitDisplayList(std::move(frame));
  }

  ref_ptr<System::IBL> System::Getter<IBL>::get()
  {
    return Engine::ref().ibl();
  }
}