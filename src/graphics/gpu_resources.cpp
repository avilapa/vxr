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
#include "../../include/engine/engine.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "../../deps/stb/stb_image.h"

namespace vxr
{

  namespace gpu
  {

    void* Texture::loadFromFile(const char* file, Texture::Info& tex, bool flip)
    {
      VXR_TRACE_SCOPE("VXR", "Texture Load");

      int w, h, comp, bytes_per_pixel;
      void* data;
      bool hdr = false;
        
      string extension = file; 
      extension = extension.substr(extension.find_last_of(".") + 1);

      if (extension == "hdr")
      {
        data = stbi_loadf(file, &w, &h, &comp, 0);
        hdr = true;
        bytes_per_pixel = comp * sizeof(float);
      }
      else
      {
        data = stbi_load(file, &w, &h, &comp, 0);
        bytes_per_pixel = comp * sizeof(stbi_uc);
      }

      if (flip)
      {
        stbi__vertical_flip(data, w, h, bytes_per_pixel);
      }

      if (!data)
      {
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [TEXTURE] Load failed. Unknown texture format.\n");
        return nullptr;
      }

      if (w < 1 || h < 1)
      {
        free(data);
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [TEXTURE] Load failed. Invalid texture data.\n");
        return nullptr;
      }

      tex.width = w;
      tex.height = h;

      switch (comp)
      {
      case 1: tex.format = ((hdr) ? TexelsFormat::R_F16 : TexelsFormat::R_U8); break;
      case 2: tex.format = ((hdr) ? TexelsFormat::RG_F16 : TexelsFormat::RG_U8); break;
      case 3: tex.format = ((hdr) ? TexelsFormat::RGB_F16 : TexelsFormat::RGB_U8); break;
      case 4: tex.format = ((hdr) ? TexelsFormat::RGBA_F16 : TexelsFormat::RGBA_U8); break;
      }

      VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [TEXTURE] Loaded (%s).\n", file);
      return data;
    }

    std::vector<void*> Texture::loadCubemapFromFile(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft, Texture::Info& tex, bool flip)
    {
      VXR_TRACE_SCOPE("VXR", "Texture Load (Cubemap)");
      int w, h, comp;
      std::vector<void*> data;
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
          VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [TEXTURE] Load failed. Unknown texture format.\n");
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
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [TEXTURE] Load failed. Invalid texture data.\n");
        return data;
      }

      if (flip)
      {
        for (uint32 i = 0; i < 6; ++i)
        {
          stbi__vertical_flip(data[i], w, h, comp * sizeof(stbi_uc));
        }
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

      VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [TEXTURE] Loaded (%s).\n", rt);
      return data;
    }

    Texture Framebuffer::color_texture(uint16 index) const 
    {
      if (!ctx) 
      {
        return Texture();
      }
      ctx->CheckValidResource(id, &ctx->framebuffers_);
      uint32 pos = RenderContext::index(id);
      return ctx->framebuffers_[pos].color_textures[index];
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

    void Framebuffer::set_color_texture(Texture t, uint16 index)
    {
      if (!ctx)
      {
        return;
      }
      ctx->CheckValidResource(id, &ctx->framebuffers_);
      uint32 pos = RenderContext::index(id);
      ctx->framebuffers_[pos].color_textures[index] = t;
    }

    void Framebuffer::set_depth_stencil_texture(Texture t)
    {
      if (!ctx)
      {
        return;
      }
      ctx->CheckValidResource(id, &ctx->framebuffers_);
      uint32 pos = RenderContext::index(id);
      ctx->framebuffers_[pos].depth_texture = t;
    }

  } /* end of gpu namespace */

} /* end of vxr namespace */