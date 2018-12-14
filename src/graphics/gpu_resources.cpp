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

#include "../../include/graphics/render_context.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "../../deps/stb/stb_image.h"

namespace vxr
{

  namespace gpu
  {

    unsigned char* Texture::loadFromFile(const char* file, Texture::Info& tex)
    {
      int w, h, comp;
      unsigned char *data = stbi_load(file, &w, &h, &comp, 0);
      if (!data)
      {
        VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Unknown texture format.\n");
        return nullptr;
      }

      if (w < 1 || h < 1)
      {
        free(data);
        VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Invalid texture data.\n");
        return nullptr;
      }

      tex.width = w;
      tex.height = h;
      switch (comp)
      {
      case 1: tex.format = TexelsFormat::R_U8; break;
      case 2: tex.format = TexelsFormat::RG_U8; break;
      case 3: tex.format = TexelsFormat::RGB_U8; break;
      case 4: tex.format = TexelsFormat::RGBA_U8; break;
      }

      return data;
    }

    std::vector<unsigned char*> Texture::loadCubemapFromFile(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft, Texture::Info& tex)
    {
      int w, h, comp;
      std::vector<unsigned char*> data;
      data.push_back(stbi_load(rt, &w, &h, &comp, 0));
      data.push_back(stbi_load(lf, &w, &h, &comp, 0));
      data.push_back(stbi_load(up, &w, &h, &comp, 0));
      data.push_back(stbi_load(dn, &w, &h, &comp, 0));
      data.push_back(stbi_load(bk, &w, &h, &comp, 0));
      data.push_back(stbi_load(ft, &w, &h, &comp, 0));
      
      bool error = false;
      for (uint32 i = 0; i < 6; ++i)
      {
        if (!data[i])
        {
          VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Unknown texture format.\n");
          error = true;
        }
      }

      if (error)
      {
        return data;
      }

      if (w < 1 || h < 1)
      {
        for (uint32 i = 0; i < 6; ++i)
        {
          free(data[0]);
        }
        VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Invalid texture data.\n");
        return data;
      }

      tex.width = w;
      tex.height = h;
      switch (comp)
      {
      case 1: tex.format = TexelsFormat::R_U8; break;
      case 2: tex.format = TexelsFormat::RG_U8; break;
      case 3: tex.format = TexelsFormat::RGB_U8; break;
      case 4: tex.format = TexelsFormat::RGBA_U8; break;
      }

      return data;
    }

    Texture Framebuffer::color_texture(uint16 p) const 
    {
      if (!ctx) 
      {
        return Texture();
      }
      ctx->CheckValidResource(id, &ctx->framebuffers_);
      uint32 pos = RenderContext::index(id);
      return ctx->framebuffers_[pos].color_textures[p];
    }

    Texture Framebuffer::depth_stencil_texture() const 
    {
      if (!ctx) 
      {
        return Texture();
      }
      ctx->CheckValidResource(id, &ctx->framebuffers_);
      uint32 pos = RenderContext::index(id);
      return ctx->framebuffers_[pos].depth_texture;
    }

  } /* end of gpu namespace */

} /* end of vxr namespace */