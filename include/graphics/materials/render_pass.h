#pragma once

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

#include "../render_context.h"
#include "../texture.h"

/**
* \file render_pass.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Render Pass (Postprocess) class containing all parameters and options.
*
*/
namespace vxr
{

  namespace mat
  {

    class RenderPass : public Object
    {
      VXR_OBJECT(RenderPass, Object);
    public:
      RenderPass();
      virtual ~RenderPass();

      void set_num_input_textures(uint32 count);
      uint32 num_input_textures() const;

      void set_num_output_textures(uint32 count);
      uint32 num_output_textures() const;

      void set_shaders(const char* vert, const char* frag);

      void set_cull(Cull::Enum cull);
      void set_render_mode(RenderMode::Enum render_mode);
      void set_blend_params(bool enabled, vec4 color, BlendFactor::Enum src_rgb, BlendFactor::Enum dst_rgb, BlendOp::Enum op_rgb, BlendFactor::Enum src_alpha, BlendFactor::Enum dst_alpha, BlendOp::Enum op_alpha);
      void set_depth_func(CompareFunc::Enum depth_func);
      void set_rgba_write(bool enabled);
      void set_depth_write(bool enabled);

      void set_uniforms_enabled(bool enabled);
      void set_uniforms_name(const char* name);
      void set_uniforms_usage(Usage::Enum usage);

      bool uniforms_enabled() const;

      // Returning false does not output any errors to console.
      bool setup();
      bool setupTextureInput(std::vector<ref_ptr<Texture>> in_textures);
      bool setupTextureOutput(std::vector<ref_ptr<Texture>> out_textures, ref_ptr<Texture> depth_texture);

      gpu::Material material() const;
      gpu::Framebuffer framebuffer() const;
      gpu::Buffer uniformBuffer() const;
      std::vector<gpu::Texture> textureInput() const;
      std::vector<ref_ptr<Texture>> textureOutput() const;

    private:
      bool initialized_ = false;
      bool use_uniforms_ = true;

      const char* uniforms_name_ = "Uniforms";
      Usage::Enum uniforms_usage_ = Usage::Dynamic;

      struct GPU
      {
        gpu::Material mat;
        gpu::Material::Info mat_info;
        gpu::Buffer uniform_buffer;
        std::vector<gpu::Texture> in_tex;

        gpu::Framebuffer fbo;
        gpu::Framebuffer::Info fbo_info;
        std::vector<ref_ptr<Texture>> out_tex;
        ref_ptr<Texture> depth_tex;
      } gpu_;
    };
    
  } /* end of mat namespace */

} /* end of vxr namespace */