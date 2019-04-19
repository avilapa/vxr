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

      uint32 index = 0;
#if VXR_MESH_PRECOMPUTE_TANGENTS
      gpu_.mat_info.attribs[index++] = { "attr_tangent",  VertexFormat::Float4 };
#endif     
      gpu_.mat_info.attribs[index++] = { "attr_position", VertexFormat::Float3 };
      gpu_.mat_info.attribs[index++] = { "attr_normal",   VertexFormat::Float3 };
      gpu_.mat_info.attribs[index++] = { "attr_uv",       VertexFormat::Float2 };
    }

    RenderPass::~RenderPass()
    {
    }

    bool RenderPass::setup()
    {
      if (!initialized_)
      {
        VXR_TRACE_SCOPE("VXR", "Render Pass Setup");

        gpu_.mat = Engine::ref().gpu()->createMaterial(gpu_.mat_info);

        if (use_uniforms_)
        {
          gpu_.uniform_buffer = Engine::ref().gpu()->createBuffer({ BufferType::Uniform, sizeof(Shader::UniformData), uniforms_usage_, uniforms_name_ });
        }

        gpu_.fbo = Engine::ref().gpu()->createFramebuffer(gpu_.fbo_info);

        initialized_ = true;
      }

      /// TODO: Review this. Is accessing the instance to update the textures the proper thing to do?
      /// TODO: Fix framebuffer sizes changing on scene load.
      for (uint32 i = 0; i < gpu_.fbo_info.num_color_textures; ++i)
      {
        gpu_.fbo.set_color_texture(gpu_.out_tex[i]->gpu_.tex, i);
        gpu_.out_tex[i]->gpu_.tex = gpu_.fbo.color_texture(i);
      }
      gpu_.fbo.set_depth_stencil_texture(gpu_.depth_tex->gpu_.tex);
      gpu_.depth_tex->gpu_.tex = gpu_.fbo.depth_stencil_texture();

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
          if (!in_textures[i]->setup())
          {
            return false;
          }
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
          if (!out_textures[i]->setup())
          {
            return false;
          }
        }
        gpu_.fbo_info.color_texture_info[i] = out_textures[i]->gpu_.info;
        gpu_.out_tex[i] = out_textures[i];
      }

      gpu_.fbo_info.num_color_textures = (uint16)gpu_.out_tex.size();

      if (depth_texture->hasChanged())
      {
        if (!depth_texture->setup())
        {
          return false;
        }
      }
      gpu_.fbo_info.depth_stencil_texture_info = depth_texture->gpu_.info;
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

    bool RenderPass::uniforms_enabled() const
    {
      return use_uniforms_;
    }

    void RenderPass::set_shaders(const char* vert, const char* frag)
    {
      gpu_.mat_info.shader.vert = Shader::Load(vert);
      gpu_.mat_info.shader.frag = Shader::Load(frag);
    }

    void RenderPass::set_cull(Cull::Enum cull)
    {
      gpu_.mat_info.cull = cull;
    }

    void RenderPass::set_render_mode(RenderMode::Enum render_mode)
    {
      gpu_.mat_info.render_mode = render_mode;
    }

    void RenderPass::set_blend_params(bool enabled, vec4 color, BlendFactor::Enum src_rgb, BlendFactor::Enum dst_rgb, BlendOp::Enum op_rgb, BlendFactor::Enum src_alpha, BlendFactor::Enum dst_alpha, BlendOp::Enum op_alpha)
    {
      gpu_.mat_info.blend.enabled = enabled;
      gpu_.mat_info.blend.color = color;
      gpu_.mat_info.blend.src_rgb = src_rgb;
      gpu_.mat_info.blend.dst_rgb = dst_rgb;
      gpu_.mat_info.blend.op_rgb = op_rgb;
      gpu_.mat_info.blend.src_alpha = src_alpha;
      gpu_.mat_info.blend.dst_alpha = dst_alpha;
      gpu_.mat_info.blend.op_alpha = op_alpha;
    }

    void RenderPass::set_depth_func(CompareFunc::Enum depth_func)
    {
      gpu_.mat_info.depth_func = depth_func;
    }

    void RenderPass::set_rgba_write(bool enabled)
    {
      gpu_.mat_info.rgba_write = enabled;
    }

    void RenderPass::set_depth_write(bool enabled)
    {
      gpu_.mat_info.depth_write = enabled;
    }

    gpu::Material RenderPass::material() const
    {
      return gpu_.mat;
    }

    gpu::Framebuffer RenderPass::framebuffer() const
    {
      return gpu_.fbo;
    }

    gpu::Buffer RenderPass::uniformBuffer() const
    {
      return gpu_.uniform_buffer;
    }

    std::vector<gpu::Texture> RenderPass::textureInput() const
    {
      return gpu_.in_tex;
    }

    std::vector<ref_ptr<Texture>> RenderPass::textureOutput() const
    {
      return gpu_.out_tex;
    }
  }
}