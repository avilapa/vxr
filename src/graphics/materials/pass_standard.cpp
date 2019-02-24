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

#include "../../../include/graphics/materials/pass_standard.h"
#include "../../../include/components/camera.h"
#include "../../../include/engine/engine.h"
#include "../../../include/graphics/window.h"

namespace vxr
{

  namespace mat
  {

    Screen::Screen()
    {
      set_name("Screen");
      set_shaders("screen_standard.vert", "screen_standard.frag");

      set_num_input_textures(1);
      set_num_output_textures(1);
      set_uniforms_enabled(false);
    }

    Screen::Instance::Instance()
    {
      init("Screen");

      uvec2 size = Engine::ref().window()->params().size;
      ref_ptr<Texture> tex;
      tex.alloc()->set_type(TextureType::T2D);
      tex->set_texels_format(TexelsFormat::RGBA_U8);
      tex->set_size(size.x, size.y);
      set_output_texture(0, tex);
    }

    BuildCubemap::BuildCubemap()
    {
      set_name("Build Cubemap");
      set_shaders("build_cubemap.vert", "build_cubemap.frag");

      set_num_input_textures(1);
      set_num_output_textures(1);
      set_uniforms_enabled(true);
      set_uniforms_name("CubemapConvolution");

      set_cull(Cull::Front);
      set_depth_write(false);
      set_depth_func(CompareFunc::LessEqual);
    }

    BuildCubemap::Instance::Instance()
    {
      init("Build Cubemap");

      ref_ptr<Texture> t;
      t.alloc()->set_type(TextureType::CubeMap);
      t->set_texels_format(TexelsFormat::RGBA_U8);
      t->set_size(1024, 1024);
      set_output_texture(0, t);

      mat4 capture_proj = Camera::BuildProjection(90.0f, 1.0f, 0.1f, 10.0f);
      mat4 capture_view[] =
      {
         Camera::BuildView(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)),
         Camera::BuildView(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)),
         Camera::BuildView(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f,  1.0f)),
         Camera::BuildView(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f, -1.0f)),
         Camera::BuildView(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)),
         Camera::BuildView(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f))
      };

      for (uint32 i = 0; i < 6; ++i)
      {
        convolution_uniforms[i].cubemap_convolution.conv_proj = capture_proj;
        convolution_uniforms[i].cubemap_convolution.conv_view = capture_view[i];
        convolution_uniforms[i].cubemap_convolution.aux.x = 1.0f; // HDR
      }
    }

    void BuildCubemap::Instance::set_hdr(bool is_hdr)
    {
      for (uint32 i = 0; i < 6; ++i)
      {
        convolution_uniforms[i].cubemap_convolution.aux.x = is_hdr ? 1.0f : 0.0f;
      }
    }

  }
}