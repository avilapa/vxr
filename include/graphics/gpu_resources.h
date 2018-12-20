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

#include "../core/object.h"

// ----------------------------------------------------------------------------------------
//  The following structures have been partially extracted from px_render.h 
//  by Jose L. Hidalgo (PpluX), and later modified to fit vxr needs.
//  Link: https://github.com/pplux/px/blob/master/px_render.h
// ----------------------------------------------------------------------------------------

/**
* \file gpu_resources.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief GPU Buffer, Texture, Material and Framebuffer resources.
*
*/
namespace vxr
{
  class RenderContext;

  namespace gpu
  {

    struct Resource 
    {
      struct Type 
      {
        enum Enum 
        {
          Invalid,
          Buffer,
          Texture,
          Material,
          Framebuffer,
        };
      };
      RenderContext* ctx;
      uint32 id;
      Type::Enum type;
    };

    struct Buffer : public Resource
    {
      Buffer(RenderContext* ctx = nullptr, uint32_t id = 0) : Resource{ ctx, id, Type::Buffer } {}
      struct Info
      {
        BufferType::Enum type_;
        uint32 size_;
        Usage::Enum usage_;
        const char* name_;
      };
    };

    struct Texture : public Resource
    {
      Texture(RenderContext* ctx = nullptr, uint32 id = 0) : Resource{ ctx, id, Type::Texture } {}
      struct Info /// initializer
      {
        uint16 width = 1;
        uint16 height = 1;
        uint16 depth = 1;
        SamplerFiltering::Enum minification_filter = SamplerFiltering::Nearest;
        SamplerFiltering::Enum magnification_filter = SamplerFiltering::Nearest;
        SamplerWrapping::Enum wrapping[3] = { SamplerWrapping::Repeat, SamplerWrapping::Repeat, SamplerWrapping::Repeat };
        TexelsFormat::Enum format = TexelsFormat::None;
        Usage::Enum usage = Usage::Static;
        TextureType::Enum type = TextureType::T2D;
      };

      static unsigned char* loadFromFile(const char* file, Texture::Info& tex);
      static std::vector<unsigned char*> loadCubemapFromFile(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft, Texture::Info& tex);
    };

    struct Material : public Resource
    {
      Material(RenderContext *ctx = nullptr, uint32_t id = 0) : Resource{ ctx, id, Type::Material } {}
      struct Info
      {
        struct Shader
        {
          string vert;
          string frag;
        } shader = {};
        VertexDeclaration attribs[kMaxVertexAttribs] = {};
        TextureType::Enum textures[kMaxTextureUnits] = {};
        Primitive::Enum primitive = Primitive::Triangles;
        Cull::Enum cull = Cull::Back;
        RenderMode::Enum render_mode = RenderMode::Solid;
        struct
        {
          BlendFactor::Enum src_rgb = BlendFactor::SrcAlpha;
          BlendFactor::Enum dst_rgb = BlendFactor::OneMinusSrcAlpha;
          BlendOp::Enum     op_rgb = BlendOp::Add;
          BlendFactor::Enum src_alpha = BlendFactor::SrcAlpha;
          BlendFactor::Enum dst_alpha = BlendFactor::OneMinusSrcAlpha;
          BlendOp::Enum     op_alpha = BlendOp::Add;
          vec4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
          bool enabled = false;
        } blend;
        CompareFunc::Enum depth_func = CompareFunc::Less;
        bool rgba_write = true;
        bool depth_write = true;
      };
    };

    struct Framebuffer : public Resource
    {
      Framebuffer(RenderContext *ctx = nullptr, uint32 id = 0) : Resource{ ctx, id, Type::Framebuffer } {}
      struct Info
      {
        Texture::Info color_texture_info[kMaxTextureUnits];
        Texture::Info depth_stencil_texture_info;
        uint16 num_color_textures; /// initializer 1
        uvec2 size;
      };

      Texture color_texture(uint16 index = 0) const;
      Texture depth_stencil_texture() const;
    };

  } /* end of gpu namespace */

} /* end of vxr namespace */