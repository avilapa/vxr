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

#include "../../../include/graphics/materials/pass_filters.h"

#include "../../../include/engine/engine.h"
#include "../../../include/graphics/window.h"

namespace vxr
{

  namespace mat
  {

    Negative::Negative()
    {
      set_name("Filter/Negative");
      set_shaders("screen_standard.vert", "screen_negative.frag");

      set_num_input_textures(1);
      set_num_output_textures(1);
      set_uniforms_enabled(false);
    }

    Negative::Instance::Instance()
    {
      init("Filter/Negative");

      uvec2 screen_size = Engine::ref().window()->params().size;
      ref_ptr<Texture> t;
      t.alloc()->set_type(TextureType::T2D);
      t->set_texels_format(TexelsFormat::RGBA_U8);
      t->set_size(screen_size.x, screen_size.y);
      set_output_texture(0, t);
    }

    Grayscale::Grayscale()
    {
      set_name("Filter/Grayscale");
      set_shaders("screen_standard.vert", "screen_grayscale.frag");

      set_num_input_textures(1);
      set_num_output_textures(1);
      set_uniforms_enabled(false);
    }

    Grayscale::Instance::Instance()
    {
      init("Filter/Grayscale");

      uvec2 screen_size = Engine::ref().window()->params().size;
      ref_ptr<Texture> t;
      t.alloc()->set_type(TextureType::T2D);
      t->set_texels_format(TexelsFormat::RGBA_U8);
      t->set_size(screen_size.x, screen_size.y);
      set_output_texture(0, t);
    }
  }
}    