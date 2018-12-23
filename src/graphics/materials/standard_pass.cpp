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

#include "../../../include/graphics/materials/standard_pass.h"

#include "../../../include/engine/engine.h"

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
      t->set_size(2048, 2048);
      set_output_texture(0, t);

      mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
      mat4 captureViews[] =
      {
         glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
         glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
         glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
         glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
         glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
         glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
      };

      for (uint32 i = 0; i < 6; ++i)
      {
        conv[i].cubemap_convolution.conv_proj = captureProjection;
        conv[i].cubemap_convolution.conv_view = captureViews[i];
      }
    }

    void BuildCubemap::Instance::set_projection_matrix(mat4 proj)
    {
      uniforms_.cubemap_convolution.conv_proj = proj;
    }

    void BuildCubemap::Instance::set_view_matrix(mat4 view)
    {
      uniforms_.cubemap_convolution.conv_view = view;
    }

  }
}