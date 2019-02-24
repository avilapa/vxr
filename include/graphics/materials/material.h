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
* \file material.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Material class containing all parameters and options.
*
*/
namespace vxr
{
  namespace System { class Renderer; }
  namespace mat
  {

    class Material : public Object
    {
      VXR_OBJECT(Material, Object);
      friend class System::Renderer;
    public:
      Material();
      virtual ~Material();

      uint32 num_textures() const;

      void set_num_textures(uint32 count);
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
      bool setupTextureTypes(std::vector<ref_ptr<Texture>> textures);

      gpu::Material material() const;
      gpu::Buffer uniformBuffer() const;
      std::vector<gpu::Texture> textureInput() const;

      // Common textures must have lower indices than instance textures.
      void set_common_texture(uint32 index, ref_ptr<Texture> texture);

    private:
      bool initialized_ = false;
      bool use_uniforms_ = true;

      uint32 common_textures_;

      const char* uniforms_name_ = "Uniforms";
      Usage::Enum uniforms_usage_ = Usage::Dynamic;

      struct GPU
      {
        gpu::Material mat;
        gpu::Material::Info info;
        gpu::Buffer uniform_buffer;
        std::vector<gpu::Texture> tex;
      } gpu_;
    };
    
  } /* end of mat namespace */

} /* end of vxr namespace */