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

#include "../../../include/graphics/materials/render_pass_instance.h"
#include "../../../include/graphics/materials/render_pass.h"
#include "../../../include/graphics/window.h"
#include "../../../include/engine/engine.h"
#include "../../../include/core/assets.h"

namespace vxr
{

  namespace mat
  {

    RenderPassInstance::RenderPassInstance() :
      active_render_pass_(0)
    {
    }

    RenderPassInstance::~RenderPassInstance()
    {
    }

    void RenderPassInstance::onGUI()
    {
      ImGui::Text(sharedRenderPass()->name().c_str());
      ImGui::Spacing();
    }

    void RenderPassInstance::init(string shared_render_pass_name, uint32 i)
    {
      uint32 num_materials = i + 1;
      if (shared_render_passes_.size() < num_materials)
      {
        shared_render_passes_.resize(num_materials);
        input_textures_.resize(num_materials);
        output_textures_.resize(num_materials);
      }

      shared_render_passes_[i] = Engine::ref().assetManager()->shared_render_pass(shared_render_pass_name.c_str());

      if (!shared_render_passes_[i])
      {
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not intialize textures for %s (%s). Unknown shared render pass.\n", name().c_str(), type().c_str());
        return;
      }

      input_textures_[i].resize(shared_render_passes_[i]->num_input_textures());
      output_textures_[i].resize(shared_render_passes_[i]->num_output_textures());

      uvec2 screen_size = Engine::ref().window()->params().size;
      depth_texture_.alloc()->set_name("Depth");
      depth_texture_->set_type(TextureType::T2D);
      depth_texture_->set_size(screen_size.x, screen_size.y);
      depth_texture_->set_texels_format(TexelsFormat::Depth_U16);
    }

    void RenderPassInstance::init(std::initializer_list<string> shared_render_pass_name)
    {
      uint32 i = 0;
      for (auto material : shared_render_pass_name)
      {
        init(material, i++);
      }
    }

    void RenderPassInstance::set_active_render_pass(uint32 index)
    {
      active_render_pass_ = index;
    }

    uint32 RenderPassInstance::active_render_pass() const
    {
      return active_render_pass_;
    }

    ref_ptr<RenderPass> RenderPassInstance::sharedRenderPass() const
    {
      return shared_render_passes_[active_render_pass_];
    }

    void RenderPassInstance::set_input_texture(uint32 index, ref_ptr<Texture> texture)
    {
      input_textures_[active_render_pass_][index] = texture;
    }

    ref_ptr<Texture> RenderPassInstance::input_texture(uint32 index /* = 0 */) const
    {
      return input_textures_[active_render_pass_][index];
    }

    std::vector<ref_ptr<Texture>> RenderPassInstance::input_textures() const
    {
      return input_textures_[active_render_pass_];
    }

    void RenderPassInstance::set_output_texture(uint32 index, ref_ptr<Texture> texture)
    {
      output_textures_[active_render_pass_][index] = texture;
    }

    ref_ptr<Texture> RenderPassInstance::output_texture(uint32 index /* = 0 */) const
    {
      return output_textures_[active_render_pass_][index];
    }

    std::vector<ref_ptr<Texture>> RenderPassInstance::output_textures() const
    {
      return output_textures_[active_render_pass_];
    }

    ref_ptr<Texture> RenderPassInstance::depth_texture() const
    {
      return depth_texture_;
    }
  }

}