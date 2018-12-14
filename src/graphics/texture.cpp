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

  void Texture::init(gpu::Texture t)
  {
    gpu_.tex = t;
  }

  void Texture::load(const char* file)
  {
    set_data(gpu::Texture::loadFromFile(file, gpu_.info));
    dirty_ = true;
  }

  void Texture::load(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft)
  {
    std::vector<unsigned char*> data = gpu::Texture::loadCubemapFromFile(rt, lf, up, dn, bk, ft, gpu_.info);

    for (uint32 i = 0; i < 6; ++i)
    {
      set_data(data[i], i);
    }

    dirty_ = true;
  }

  void Texture::load(const char* cubemap_folder_path, const char* extension)
  {
    std::string folder = cubemap_folder_path;
    load((folder + "/rt." + extension).c_str(),
         (folder + "/lf." + extension).c_str(),
         (folder + "/up." + extension).c_str(),
         (folder + "/dn." + extension).c_str(), 
         (folder + "/bk." + extension).c_str(), 
         (folder + "/ft." + extension).c_str());
  }

  void Texture::setup()
  {
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

  void Texture::set_data(unsigned char* data, uint32 index)
  {
    if (data_[index] != nullptr)
    {
      free(data_[index]);
      data_[index] = nullptr;
    }
    data_[index] = data;
  }

  bool Texture::hasChanged()
  {
    return dirty_;
  }

  unsigned char* Texture::data() const
  {
    return data_[0];
  }

  uint32 Texture::id()
  {
    if (!internal_id_)
    {
      auto b = RenderContext::GetResource(gpu_.tex.id, &gpu_.tex.ctx->textures_, &gpu_.tex.ctx->back_end_->textures);
      internal_id_ = b.first->id;
    }
    return internal_id_;
  }
}