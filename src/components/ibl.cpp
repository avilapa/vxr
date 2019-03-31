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
#include "../../include/core/assets.h"
#include "../../include/core/scene.h"
#include "../../include/graphics/materials/pass_standard.h"
#include "../../include/graphics/mesh.h"

namespace vxr 
{

  IBL::IBL()
  {
    set_name("Image Based Light");
    contributes_ = false;
    initialization_level_ = 0;

    build_cubemap_.alloc();
    compute_irradiance_.alloc();
    prefiltered_cubemap_.alloc();
    brdf_integration_.alloc();
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

  void IBL::set_texture(ref_ptr<Texture> texture)
  {
    switch (texture->texture_type())
    {
    case TextureType::T2D:
      build_cubemap_->set_input_texture(0, texture);
      build_cubemap_->set_hdr(texture->hdr());
      cubemap_texture_ = build_cubemap_->output_texture();
      initialization_level_ = 0;
      break;
    case TextureType::CubeMap:
      cubemap_texture_ = texture;
      initialization_level_ = 1;
      break;
    default:
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "Texture type is not valid for Image Based Lighting component.");
      break;
    }
  }

  ref_ptr<Texture> IBL::cubemap_texture() const
  {
    return cubemap_texture_;
  }

  ref_ptr<Texture> IBL::irradiance_cubemap_texture() const
  {
    return compute_irradiance_->output_texture(0);
  }

  ref_ptr<Texture> IBL::prefiltered_cubemap_texture() const
  {
    return prefiltered_cubemap_->output_texture(0);
  }

  ref_ptr<Texture> IBL::brdf_lut() const
  {
    return brdf_integration_->output_texture(0);
  }

  bool IBL::initialized() const
  {
    return initialization_level_ >= 4;
  }

  System::IBL::IBL()
  {
  }

  System::IBL::~IBL()
  {
  }

  void System::IBL::set_main(ref_ptr<vxr::IBL> light)
  {
    if (!light || !scene_)
    {
      main_ = nullptr;
      cleanMaterialIBLTextures();
      return;
    }

    if (main_ != light)
    {
      if (light->gameObject()->scene_id() != scene_->id())
      {
        return;
      }
      main_ = light;
      if (main_->initialized())
      {
        ///main_->initialization_level_ = 0;
        updateMaterialIBLTextures();
      }
    }
  }

  ref_ptr<IBL> System::IBL::main() const
  {
    return main_;
  }

  void System::IBL::onSceneChanged()
  {
    ComponentSystem::onSceneChanged();
    set_main(nullptr);
  }

  void System::IBL::start()
  {
    // Find IBL in scene
    ref_ptr<Skybox> skybox = Engine::ref().scene()->skybox();
    if (skybox != nullptr)
    {
      set_main(skybox->getComponent<vxr::IBL>());
    }
    else
    {
      set_main(nullptr);
    }
  }

  void System::IBL::renderUpdate()
  {
    if (!main_)
    {
      return;
    }

    VXR_TRACE_SCOPE("VXR", "IBL Render Update");
    if (!main_->gameObject()->active() || main_->initialized())
    {
      return;
    }

    if (!buildCubemap())
    {
      return;
    }

    if (main_->cubemap_texture()->loading())
    {
      return;
    }

    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [IBL] Recomputing maps...\n");

    if (!computeIrradiance())
    {
      return;
    }

    if (!computePrefiltering())
    {
      return;
    }

    if (!integrateBRDF())
    {
      return;
    }

    updateMaterialIBLTextures();
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [IBL] Maps recomputed correctly.\n");
  }

  bool System::IBL::buildCubemap()
  {
    if (main_->initialization_level_ >= 1)
    {
      return true;
    }

    if (!main_->build_cubemap_->input_texture(0))
    {
      return false;
    }

    if (main_->build_cubemap_->input_texture(0)->loading())
    {
      return false;
    }

    ref_ptr<mat::RenderPass> shared_render_pass = main_->build_cubemap_->sharedRenderPass();
    if (!shared_render_pass->setupTextureInput(main_->build_cubemap_->input_textures()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Build Cubemap texture input could not be setup correctly.\n");
      return false;
    }

    if (!shared_render_pass->setupTextureOutput(main_->build_cubemap_->output_textures(), main_->build_cubemap_->depth_texture()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Build Cubemap texture output could not be setup correctly.\n");
      return false;
    }

    if (!shared_render_pass->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Build Cubemap could not be setup correctly.\n");
      return false;
    }
    vec2 texture_size = main_->build_cubemap_->output_texture(0)->size();

    DisplayList frame;
    for (uint32 face = 0; face < 6; ++face)
    {
      frame.setupViewCommand()
        .set_viewport({ 0, 0, (uint16)texture_size.x, (uint16)texture_size.y })
        .set_framebuffer(shared_render_pass->framebuffer())
        .set_cubemap_target((CubemapTarget::Enum)(CubemapTarget::PositiveX + face));
      frame.clearCommand()
        .set_color(Color::Olive)
        .set_clear_color(true)
        .set_clear_depth(true);
      frame.fillBufferCommand()
        .set_buffer(shared_render_pass->uniformBuffer())
        .set_data(&main_->build_cubemap_->convolution_uniforms[face])
        .set_size(sizeof(main_->build_cubemap_->convolution_uniforms[face]));
      frame.setupMaterialCommand()
        .set_material(shared_render_pass->material())
        .set_buffer(0, Engine::ref().assetManager()->default_cube()->vertexBuffer())
        .set_v_texture(shared_render_pass->textureInput())
        .set_uniform_buffer(0, shared_render_pass->uniformBuffer());
      frame.renderCommand()
        .set_index_buffer(Engine::ref().assetManager()->default_cube()->indexBuffer())
        .set_count(Engine::ref().assetManager()->default_cube()->indexCount())
        .set_type(Engine::ref().assetManager()->default_cube()->indexFormat());
    }

    Engine::ref().submitDisplayList(std::move(frame));
    main_->initialization_level_++;
    return true;
  }

  bool System::IBL::computeIrradiance()
  {
    if (main_->initialization_level_ >= 2)
    {
      return true;
    }

    main_->compute_irradiance_->set_input_texture(0, main_->cubemap_texture());
    ref_ptr<mat::RenderPass> shared_render_pass = main_->compute_irradiance_->sharedRenderPass();
    if (!shared_render_pass->setupTextureInput(main_->compute_irradiance_->input_textures()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Irradiance Computation texture input could not be setup correctly.\n");
      return false;
    }

    if (!shared_render_pass->setupTextureOutput(main_->compute_irradiance_->output_textures(), main_->compute_irradiance_->depth_texture()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Irradiance Computation texture output could not be setup correctly.\n");
      return false;
    }

    if (!shared_render_pass->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Irradiance Computation could not be setup correctly.\n");
      return false;
    }
    vec2 texture_size = main_->compute_irradiance_->output_texture(0)->size();

    DisplayList frame;
    for (uint32 face = 0; face < 6; ++face)
    {
      frame.setupViewCommand()
        .set_viewport({ 0, 0, (uint16)texture_size.x, (uint16)texture_size.y })
        .set_framebuffer(shared_render_pass->framebuffer())
        .set_cubemap_target((CubemapTarget::Enum)(CubemapTarget::PositiveX + face));
      frame.clearCommand()
        .set_color(Color::Olive)
        .set_clear_color(true)
        .set_clear_depth(true);
      frame.fillBufferCommand()
        .set_buffer(shared_render_pass->uniformBuffer())
        .set_data(&main_->compute_irradiance_->convolution_uniforms[face])
        .set_size(sizeof(main_->compute_irradiance_->convolution_uniforms[face]));
      frame.setupMaterialCommand()
        .set_material(shared_render_pass->material())
        .set_buffer(0, Engine::ref().assetManager()->default_cube()->vertexBuffer())
        .set_v_texture(shared_render_pass->textureInput())
        .set_uniform_buffer(0, shared_render_pass->uniformBuffer());
      frame.renderCommand()
        .set_index_buffer(Engine::ref().assetManager()->default_cube()->indexBuffer())
        .set_count(Engine::ref().assetManager()->default_cube()->indexCount())
        .set_type(Engine::ref().assetManager()->default_cube()->indexFormat());
    }
    Engine::ref().submitDisplayList(std::move(frame));
    main_->initialization_level_++;
    return true;
  }

  bool System::IBL::computePrefiltering()
  {
    if (main_->initialization_level_ >= 3)
    {
      return true;
    }

    main_->prefiltered_cubemap_->set_input_texture(0, main_->cubemap_texture());
    ref_ptr<mat::RenderPass> shared_render_pass = main_->prefiltered_cubemap_->sharedRenderPass();
    if (!shared_render_pass->setupTextureInput(main_->prefiltered_cubemap_->input_textures()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Cubemap prefiltering texture input could not be setup correctly.\n");
      return false;
    }

    if (!shared_render_pass->setupTextureOutput(main_->prefiltered_cubemap_->output_textures(), main_->prefiltered_cubemap_->depth_texture()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Cubemap prefiltering texture output could not be setup correctly.\n");
      return false;
    }

    if (!shared_render_pass->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Cubemap prefiltering could not be setup correctly.\n");
      return false;
    }
    vec2 texture_size = main_->prefiltered_cubemap_->output_texture(0)->size();

    DisplayList frame;
    for (uint32 mip = 0; mip < mat::PrefilterCubemap::Instance::NUM_MIP_LEVELS; ++mip)
    {
      uvec2 mip_size = uvec2(texture_size.x * std::pow(0.5f, mip), texture_size.y * std::pow(0.5f, mip));
      float roughness = (float)mip / (float)(mat::PrefilterCubemap::Instance::NUM_MIP_LEVELS - 1);

      main_->prefiltered_cubemap_->set_roughness(mip, roughness);

      for (uint32 face = 0; face < 6; ++face)
      {
        frame.setupViewCommand()
          .set_viewport({ 0, 0, (uint16)mip_size.x, (uint16)mip_size.y })
          .set_framebuffer(shared_render_pass->framebuffer())
          .set_cubemap_target((CubemapTarget::Enum)(CubemapTarget::PositiveX + face))
          .set_mip_level(mip);
        frame.clearCommand()
          .set_color(Color::Olive)
          .set_clear_color(true)
          .set_clear_depth(true);
        frame.fillBufferCommand()
          .set_buffer(shared_render_pass->uniformBuffer())
          .set_data(&main_->prefiltered_cubemap_->convolution_uniforms[6 * mip + face])
          .set_size(sizeof(main_->prefiltered_cubemap_->convolution_uniforms[6 * mip + face]));
        frame.setupMaterialCommand()
          .set_material(shared_render_pass->material())
          .set_buffer(0, Engine::ref().assetManager()->default_cube()->vertexBuffer())
          .set_v_texture(shared_render_pass->textureInput())
          .set_uniform_buffer(0, shared_render_pass->uniformBuffer());
        frame.renderCommand()
          .set_index_buffer(Engine::ref().assetManager()->default_cube()->indexBuffer())
          .set_count(Engine::ref().assetManager()->default_cube()->indexCount())
          .set_type(Engine::ref().assetManager()->default_cube()->indexFormat());
      }
    }
    Engine::ref().submitDisplayList(std::move(frame));
    main_->initialization_level_++;
    return true;
  }

  bool System::IBL::integrateBRDF()
  {
    if (main_->initialization_level_ >= 4)
    {
      return true;
    }

    ref_ptr<mat::RenderPass> shared_render_pass = main_->brdf_integration_->sharedRenderPass();
    if (!shared_render_pass->setupTextureOutput(main_->brdf_integration_->output_textures(), main_->brdf_integration_->depth_texture()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] BRDF Integration texture output could not be setup correctly.\n");
      return false;
    }

    if (!shared_render_pass->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] BRDF Integration could not be setup correctly.\n");
      return false;
    }
    vec2 texture_size = main_->brdf_integration_->output_texture(0)->size();

    DisplayList frame;
    frame.setupViewCommand()
      .set_viewport({ 0, 0, (uint16)texture_size.x, (uint16)texture_size.y })
      .set_framebuffer(shared_render_pass->framebuffer());
    frame.clearCommand()
      .set_color(Color::Olive)
      .set_clear_color(true)
      .set_clear_depth(true);
    frame.setupMaterialCommand()
      .set_material(shared_render_pass->material())
      .set_buffer(0, Engine::ref().assetManager()->default_quad()->vertexBuffer());
    frame.renderCommand()
      .set_index_buffer(Engine::ref().assetManager()->default_quad()->indexBuffer())
      .set_count(Engine::ref().assetManager()->default_quad()->indexCount())
      .set_type(Engine::ref().assetManager()->default_quad()->indexFormat());
    Engine::ref().submitDisplayList(std::move(frame));
    main_->initialization_level_++;
    return true;
  }

  void System::IBL::subscribeMaterialForIBLTextures(const string& material_name)
  {
    pbr_materials_.push_back(material_name);
  }

  void System::IBL::updateMaterialIBLTextures()
  {
    for (uint32 i = 0; i < pbr_materials_.size(); ++i)
    {
      ref_ptr<mat::Material> mat = Engine::ref().assetManager()->shared_material(pbr_materials_[i].c_str());
      mat->set_common_texture(0, main_->irradiance_cubemap_texture());
      mat->set_common_texture(1, main_->prefiltered_cubemap_texture());
      mat->set_common_texture(2, main_->brdf_lut());
    }
  }

  void System::IBL::cleanMaterialIBLTextures()
  {
    ref_ptr<Texture> tex_cube = Engine::ref().assetManager()->default_cubemap();
    ref_ptr<Texture> tex_brdf = Engine::ref().assetManager()->default_texture_white();
    for (uint32 i = 0; i < pbr_materials_.size(); ++i)
    {
      ref_ptr<mat::Material> mat = Engine::ref().assetManager()->shared_material(pbr_materials_[i].c_str());
      mat->set_common_texture(0, tex_cube);
      mat->set_common_texture(1, tex_cube);
      mat->set_common_texture(2, tex_brdf);
    }
  }

  ref_ptr<System::IBL> System::Getter<IBL>::get()
  {
    return Engine::ref().ibl();
  }
}