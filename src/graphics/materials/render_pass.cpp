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

#include "../../../include/graphics/materials/render_pass.h"
#include "../../../include/graphics/materials/shader.h"
#include "../../../include/engine/engine.h"
#include "../../../include/engine/GPU.h"

namespace vxr
{

  namespace mat
  {

    RenderPass::RenderPass()
    {
      set_name("Render Pass");
      if (gpu_.mat_info.shader.vert == "")
      {
        gpu_.mat_info.shader.vert = Shader::Load("screen_standard.vert");
      }
      if (gpu_.mat_info.shader.frag == "")
      {
        gpu_.mat_info.shader.frag = Shader::Load("screen_standard.frag");
      }
      gpu_.mat_info.attribs[0] = { "attr_position", VertexFormat::Float3 };
      gpu_.mat_info.attribs[1] = { "attr_normal",   VertexFormat::Float3 };
      gpu_.mat_info.attribs[2] = { "attr_uv",       VertexFormat::Float2 };
    }

    RenderPass::~RenderPass()
    {
    }

    bool RenderPass::setup()
    {
      if (initialized_)
      {
        return true;
      }

      VXR_TRACE_SCOPE("VXR", "Render Pass Setup");
      gpu_.mat = Engine::ref().gpu()->createMaterial(gpu_.mat_info);

      if (use_uniforms_)
      {
        gpu_.uniform_buffer = Engine::ref().gpu()->createBuffer({ BufferType::Uniform, sizeof(Shader::UniformData), uniforms_usage_, uniforms_name_ });
      }

      gpu::Framebuffer::Info fb_info;
      for (uint32 i = 0; i < num_output_textures(); ++i)
      {
        fb_info.color_texture_info[i] = gpu_.out_tex[i]->gpu_.info;
      }
      fb_info.depth_stencil_texture_info = gpu_.depth_tex->gpu_.info;
      fb_info.num_color_textures = (uint16)num_output_textures();

      gpu_.fbo = Engine::ref().gpu()->createFramebuffer(fb_info);

      for (uint32 i = 0; i < gpu_.out_tex.size(); ++i)
      {
        gpu_.out_tex[i]->gpu_.tex = gpu_.fbo.color_texture(i);
      }
      gpu_.depth_tex->gpu_.tex = gpu_.fbo.depth_stencil_texture();

      initialized_ = true;
      return true;
    }

    bool RenderPass::setupTextureInput(std::vector<ref_ptr<Texture>> in_textures)
    {
      for (uint32 i = 0; i < gpu_.in_tex.size(); ++i)
      {
        if (!in_textures[i])
        {
          return false;
        }

        if (in_textures[i]->hasChanged())
        {
          in_textures[i]->setup();
        }
        gpu_.mat_info.textures[i] = in_textures[i]->gpu_.info.type;
        gpu_.in_tex[i] = in_textures[i]->gpu_.tex;
      }
      return true;
    }

    bool RenderPass::setupTextureOutput(std::vector<ref_ptr<Texture>> out_textures, ref_ptr<Texture> depth_texture)
    {
      for (uint32 i = 0; i < gpu_.out_tex.size(); ++i)
      {
        if (out_textures[i]->hasChanged())
        {
          out_textures[i]->setup();
        }
        gpu_.out_tex[i] = out_textures[i];
      }

      if (depth_texture->hasChanged())
      {
        depth_texture->setup();
      }
      gpu_.depth_tex = depth_texture;

      return true;
    }

    void RenderPass::set_num_input_textures(uint32 count)
    {
      gpu_.in_tex.resize(count);
    }

    uint32 RenderPass::num_input_textures() const
    {
      return gpu_.in_tex.size();
    }

    void RenderPass::set_num_output_textures(uint32 count)
    {
      gpu_.out_tex.resize(count);
    }

    uint32 RenderPass::num_output_textures() const
    {
      return gpu_.out_tex.size();
    }

    void RenderPass::set_uniforms_enabled(bool enabled)
    {
      use_uniforms_ = enabled;
    }

    void RenderPass::set_uniforms_name(const char* name)
    {
      uniforms_name_ = name;
    }

    void RenderPass::set_uniforms_usage(Usage::Enum usage)
    {
      uniforms_usage_ = usage;
    }

    void RenderPass::set_shaders(const char* vert, const char* frag)
    {
      gpu_.mat_info.shader.vert = Shader::Load(vert);
      gpu_.mat_info.shader.frag = Shader::Load(frag);
    }
  }
}