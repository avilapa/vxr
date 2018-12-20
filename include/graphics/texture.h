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

#include "../graphics/render_context.h"

/**
* \file texture.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Texture class.
*
*/
namespace vxr
{

  namespace mat { class Material; class RenderPass; }
  class Composer;

  class Texture : public Object
  {
    VXR_OBJECT(Texture, Object);
    friend class mat::Material;
    friend class mat::RenderPass;
    friend class Composer;
  public:
    Texture();
    ~Texture();

    void init(gpu::Texture t); /// TEMP: Needed for high level framebuffer usage
    
    void load(const char* file);
    void load(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft);
    void load(const char* cubemap_folder_path, const char* extension);

    void set_size(uint16 width = 1, uint16 height = 1, uint16 depth = 1);
    void set_offset(uint16 x = 0, uint16 y = 0, uint16 z = 0);
    void set_filtering(SamplerFiltering::Enum min, SamplerFiltering::Enum mag);
    void set_wrapping(SamplerWrapping::Enum wrap);
    void set_wrapping(SamplerWrapping::Enum wrap_1, SamplerWrapping::Enum wrap_2, SamplerWrapping::Enum wrap_3);
    void set_texels_format(TexelsFormat::Enum format);
    void set_usage(Usage::Enum usage);
    void set_type(TextureType::Enum type);
    void set_build_mipmap(bool build_mipmap);
    void set_data(unsigned char* data, uint32 index = 0);

    bool hasChanged();
    unsigned char* data() const;
    uint32 id();

  private:
    bool dirty_ = false;
    unsigned char* data_[6];

    uint32 internal_id_ = 0;

    struct GPU
    {
      gpu::Texture tex;
      gpu::Texture::Info info;
      uint16 offset[3] = { 0, 0, 0 };
      bool build_mipmap = false;
    } gpu_;

    void setup();
  };

} /* end of vxr namespace */