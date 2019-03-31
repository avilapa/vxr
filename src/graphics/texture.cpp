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

#include "../../include/graphics/texture.h"

#include "../../include/engine/engine.h"
#include "../../include/engine/gpu.h"

#if defined (VXR_OPENGL)
#  include "../graphics/backend/opengl/gl_backend.h"
#elif defined (VXR_DX11)
#  include "../graphics/backend/dx11/dx11_backend.h"
#else
#  error Backend must be defined on GENie.lua (e.g. try parameters --gl OR --dx11).
#endif

namespace vxr
{

  Texture::Texture()
  {
    set_name("Texture");
    for (uint32 i = 0; i < 6; ++i)
    {
      data_[i] = nullptr;
    }
  }

  Texture::~Texture()
  {
    for (uint32 i = 0; i < 6; ++i)
    {
      if (data_[i])
      {
        free(data_[i]);
        data_[i] = nullptr;
      }
    }
  }

  bool Texture::setup()
  {
    if (loading_)
    {
      return false;
    }

    VXR_TRACE_SCOPE("VXR", "Texture Setup");

    DisplayList add_to_frame;
    if (!gpu_.tex.id)
    {
      gpu_.tex = Engine::ref().gpu()->createTexture(gpu_.info);
    }
    add_to_frame.fillTextureCommand()
      .set_texture(gpu_.tex)
      .set_data(data_[0])
      .set_data_1(data_[1])
      .set_data_2(data_[2])
      .set_data_3(data_[3])
      .set_data_4(data_[4])
      .set_data_5(data_[5])
      .set_offset_x(gpu_.offset[0])
      .set_offset_y(gpu_.offset[1])
      .set_offset_z(gpu_.offset[2])
      .set_build_mipmap(gpu_.build_mipmap)
      .set_width(gpu_.info.width)
      .set_height(gpu_.info.height)
      .set_depth(gpu_.info.depth);
    Engine::ref().submitDisplayList(std::move(add_to_frame));
    dirty_ = false;
    return true;
  }

  void Texture::set_size(uint16 width /* = 1 */, uint16 height /* = 1 */, uint16 depth /* = 1 */)
  {
    gpu_.info.width = width;
    gpu_.info.height = height;
    gpu_.info.depth = depth;
    dirty_ = true;
  }

  void Texture::set_offset(uint16 x /* = 0 */, uint16 y /* = 0 */, uint16 z /* = 0 */)
  {
    gpu_.offset[0] = x;
    gpu_.offset[1] = y;
    gpu_.offset[2] = z;
    dirty_ = true;
  }

  void Texture::set_filtering(SamplerFiltering::Enum min, SamplerFiltering::Enum mag)
  {
    gpu_.info.minification_filter = min;
    gpu_.info.magnification_filter = mag;
    dirty_ = true;
  }

  void Texture::set_wrapping(SamplerWrapping::Enum wrap)
  {
    set_wrapping(wrap, wrap, wrap);
    dirty_ = true;
  }

  void Texture::set_wrapping(SamplerWrapping::Enum wrap_1, SamplerWrapping::Enum wrap_2, SamplerWrapping::Enum wrap_3)
  {
    gpu_.info.wrapping[0] = wrap_1;
    gpu_.info.wrapping[1] = wrap_2;
    gpu_.info.wrapping[2] = wrap_3;
    dirty_ = true;
  }

  void Texture::set_texels_format(TexelsFormat::Enum format)
  {
    gpu_.info.format = format;
    dirty_ = true;
  }

  void Texture::set_usage(Usage::Enum usage)
  {
    gpu_.info.usage = usage;
    dirty_ = true;
  }

  void Texture::set_type(TextureType::Enum type)
  {
    gpu_.info.type = type;
    dirty_ = true;
  }

  void Texture::set_build_mipmap(bool build_mipmap)
  {
    gpu_.build_mipmap = build_mipmap;
    dirty_ = true;
  }

  void Texture::set_data(void* data, uint32 index)
  {
    if (data_[index] != nullptr)
    {
      free(data_[index]);
      data_[index] = nullptr;
    }
    data_[index] = data;
  }

  void Texture::set_data(Color color, uint32 index)
  {
    if (data_[index] != nullptr)
    {
      free(data_[index]);
      data_[index] = nullptr;
    }

    /// TODO: Assumes uchar type.
    data_[index] = (void*)malloc(3 * sizeof(unsigned char));

    static const unsigned char background_color[] =
    {
      (unsigned char)(color.r * 255.0f),
      (unsigned char)(color.g * 255.0f),
      (unsigned char)(color.b * 255.0f),
    };

    data_[index] = (void*)background_color;
  }

  void Texture::set_hdr(bool hdr)
  {
    hdr_ = hdr;
  }

  uvec2 Texture::size() const
  {
    return uvec2(gpu_.info.width, gpu_.info.height);
  }

  TextureType::Enum Texture::texture_type() const
  {
    return gpu_.info.type;
  }

  bool Texture::hasChanged() const
  {
    return dirty_;
  }

  bool Texture::loading() const
  {
    return loading_;
  }

  void* Texture::data() const
  {
    return data_[0];
  }

  bool Texture::hdr() const
  {
    return hdr_;
  }

  uint32 Texture::id()
  {
    if (!internal_id_)
    {
      if (!gpu_.tex.ctx)
      {
        return 0;
      }
      auto b = RenderContext::GetResource(gpu_.tex.id, &gpu_.tex.ctx->textures_, &gpu_.tex.ctx->back_end_->textures);
      internal_id_ = b.first->id;
    }
    return internal_id_;
  }

  string Texture::path() const
  {
    return path_;
  }
}