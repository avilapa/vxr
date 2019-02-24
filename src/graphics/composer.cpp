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

#include "../../include/graphics/composer.h"
#include "../../include/graphics/mesh.h"
#include "../../include/core/assets.h"
#include "../../include/engine/engine.h"
#include "../../include/engine/gpu.h"
#include "../../include/components/camera.h"
#include "../../include/components/light.h"

namespace vxr
{

  Composer::Composer() :
    render_size_(uvec2(0)),
    screen_texture_(nullptr),
    screen_prepass_(nullptr)
  {
    set_name("Composer");
    render_to_screen_.alloc()->init("Screen Material");
  }

  Composer::~Composer()
  {
  }

  void Composer::onGUI()
  {
  }

  void Composer::addRenderPass(ref_ptr<mat::RenderPassInstance> pass)
  {
    render_passes_.push_back(pass);
  }

  void Composer::set_render_size(uvec2 render_size)
  {
    render_size_ = render_size;
  }

  uint32 Composer::final_texture_id()
  {
    if (screen_texture_ != nullptr)
    {
      return screen_texture_->id();
    }
    return 0;
  }

  void Composer::init()
  {
    if (!initialized_)
    {
      screen_prepass_.alloc()->set_name("Screen Material Instance");
      initialized_ = true;
    }
    
    render_size_ = Engine::ref().window()->params().size;

    screen_texture_ = screen_prepass_->output_texture(0);

    ref_ptr<mat::RenderPass> shared_pass = screen_prepass_->sharedRenderPass();
    if (!shared_pass->setupTextureOutput(screen_prepass_->output_textures(), screen_prepass_->depth_texture()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Screen material texture output could not be setup correctly.\n");
      return;
    }

    if (!shared_pass->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Screen material could not be setup correctly.\n");
      return;
    }
  }

  void Composer::setupFirstPass()
  {
    ref_ptr<mat::RenderPass> shared_pass = screen_prepass_->sharedRenderPass();

    DisplayList frame;
    frame.setupViewCommand()
      .set_viewport({ 0,0, (uint16)(render_size_.x), (uint16)render_size_.y })
      .set_framebuffer(shared_pass->framebuffer())
      .set_resolution(render_size_);
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void Composer::setupLastPass()
  {
    uvec2 size = Engine::ref().window()->params().size;
    DisplayList frame;
    frame.setupViewCommand()
      .set_viewport({ 0,0, (uint16)size.x, (uint16)size.y });
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void Composer::renderToScreen()
  {
    render_to_screen_->set_texture(0, screen_texture_);
    ref_ptr<mat::Material> shared_render_pass = render_to_screen_->sharedMaterial();
    if (!shared_render_pass->setupTextureTypes(render_to_screen_->textures()))
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Screen material textures could not be setup correctly.\n");
      return;
    }

    if (!shared_render_pass->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Screen material could not be setup correctly.\n");
      return;
    }

    DisplayList frame;
    frame.clearCommand()
      .set_color(Color::Blue)
      .set_clear_color(true)
      .set_clear_depth(true);
    frame.setupMaterialCommand()
      .set_material(shared_render_pass->material())
      .set_buffer(0, Engine::ref().assetManager()->default_quad()->vertexBuffer())
      .set_v_texture(shared_render_pass->textureInput());
    frame.renderCommand()
      .set_index_buffer(Engine::ref().assetManager()->default_quad()->indexBuffer())
      .set_count(Engine::ref().assetManager()->default_quad()->indexCount())
      .set_type(Engine::ref().assetManager()->default_quad()->indexFormat());
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void Composer::applyPostprocessing()
  {
    DisplayList frame;
    ref_ptr<Texture> last_output_texture = screen_prepass_->output_texture(0);
    for (auto i : render_passes_)
    {
      i->set_input_texture(0, last_output_texture);
      ref_ptr<mat::RenderPass> shared_render_pass = i->sharedRenderPass();
      if (!shared_render_pass->setupTextureInput(i->input_textures()))
      {
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Screen material texture input could not be setup correctly.\n");
        continue;
      }

      if (!shared_render_pass->setupTextureOutput(i->output_textures(), i->depth_texture()))
      {
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Screen material texture output could not be setup correctly.\n");
        continue;
      }

      if (!shared_render_pass->setup())
      {
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR] Screen material could not be setup correctly.\n");
        continue;
      }

      frame.setupViewCommand()
        .set_viewport({ 0,0, (uint16)(render_size_.x), (uint16)render_size_.y })
        .set_framebuffer(shared_render_pass->framebuffer())
        .set_resolution(render_size_);
      frame.clearCommand()
        .set_color(Color::Blue)
        .set_clear_color(true)
        .set_clear_depth(true);
      if (shared_render_pass->uniforms_enabled())
      {
        frame.fillBufferCommand()
          .set_buffer(shared_render_pass->uniformBuffer())
          .set_data(&i->uniforms_)
          .set_size(sizeof(i->uniforms_));
      }
      frame.setupMaterialCommand()
        .set_material(shared_render_pass->material())
        .set_buffer(0, Engine::ref().assetManager()->default_quad()->vertexBuffer())
        .set_v_texture(shared_render_pass->textureInput())
        .set_uniform_buffer(0, Engine::ref().camera()->common_uniforms_buffer())
        .set_uniform_buffer(1, Engine::ref().light()->light_uniforms_buffer())
        .set_uniform_buffer(2, ((shared_render_pass->uniforms_enabled()) ? shared_render_pass->uniformBuffer() : gpu::Buffer{}));
      frame.renderCommand()
        .set_index_buffer(Engine::ref().assetManager()->default_quad()->indexBuffer())
        .set_count(Engine::ref().assetManager()->default_quad()->indexCount())
        .set_type(Engine::ref().assetManager()->default_quad()->indexFormat());

      last_output_texture = i->output_texture(0);
    }
    screen_texture_ = last_output_texture;
    Engine::ref().submitDisplayList(std::move(frame));
  }

}