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

// ----------------------------------------------------------------------------------------
//  The following functions have been extracted from px_render.h by Jose L. Hidalgo (PpluX), 
//  and later modified and expanded to fit vxr needs.
//  Link: https://github.com/pplux/px/blob/master/px_render.h
// ----------------------------------------------------------------------------------------

#include "gl_backend.h"

#ifdef VXR_OPENGL

#include "../../../include/graphics/render_context.h"
#include "../../../include/graphics/shader.h"

static void OnError(const char *format, ...) 
{
  va_list args;
  va_start(args, format);
  char buffer[2048];
  std::vsnprintf(buffer, 2048, format, args);
  va_end(args);
  VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: %s\n", buffer);
}
static void CheckGLError(const char* operation) 
{
  vxr::int32 error = glGetError();
  if (error) 
  {
    OnError("[OPENGL_ERROR]: 0x%x (%s)\n", error, operation);
    return;
  }
}
#define GLCHECK_STR_STR(A) #A
#define GLCHECK_STR(A) GLCHECK_STR_STR(A)
#define GLCHECK(...) {__VA_ARGS__; CheckGLError(__FILE__  ":" GLCHECK_STR(__LINE__) "->" #__VA_ARGS__);}

namespace vxr
{

  namespace gpu
  {

    void InitBackEnd(BackEnd** b, const Params::GPU &params)
    {
      *b = new BackEnd();

      (*b)->buffers.alloc(params.max_buffers);
      (*b)->textures.alloc(params.max_textures);
      (*b)->materials.alloc(params.max_materials);
      (*b)->framebuffers.alloc(params.max_framebuffers);
    }

    void DestroyBackEnd(BackEnd** b)
    {
      delete *b;
      *b = nullptr;
    }

    static GLenum TranslateVertexType(uint32 format);
    static GLenum Translate(Usage::Enum e);
    static GLenum Translate(IndexFormat::Enum e);
    static GLenum Translate(SamplerFiltering::Enum e);
    static GLenum Translate(SamplerWrapping::Enum e);
    static GLenum Translate(Primitive::Enum e);
    static GLenum Translate(BlendFactor::Enum e);
    static GLenum Translate(BlendOp::Enum e);
    static GLenum Translate(CompareFunc::Enum e);

    void ClearScreen(const DisplayList::ClearData& d)
    {
      uint32 mask = 0;
      if (d.clear_color)
      {
        GLCHECK(glClearColor(d.color.r, d.color.g, d.color.b, d.color.a));
        mask |= GL_COLOR_BUFFER_BIT;
        GLCHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
      }
      if (d.clear_depth)
      {
        GLCHECK(glClearDepth(d.depth));
        mask |= GL_DEPTH_BUFFER_BIT;
        GLCHECK(glDepthMask(GL_TRUE));
      }
      if (d.clear_stencil)
      {
        GLCHECK(glClearStencil(d.stencil));
        mask |= GL_STENCIL_BUFFER_BIT;
      }
      GLCHECK(glClear(mask));
    }

    void FillBuffer(DisplayList::FillBufferData& d)
    {
      auto b = RenderContext::GetResource(d.buffer.id, &d.buffer.ctx->buffers_, &d.buffer.ctx->back_end_->buffers);
      GLuint id = b.second->buffer;
      GLenum target = 0;
      switch (b.first->info.type_)
      {
      case BufferType::Vertex:  target = GL_ARRAY_BUFFER; break;
      case BufferType::Index:   target = GL_ELEMENT_ARRAY_BUFFER; break;
      case BufferType::Uniform: target = GL_UNIFORM_BUFFER; break;
      default: break;
      }

      if (!id)
      {
        GLCHECK(glGenBuffers(1, &id));
        GLCHECK(glBindBuffer(target, id));
        GLCHECK(glBufferData(target, d.size, nullptr, Translate(b.first->info.usage_)));
        b.second->buffer = id;
      }

      GLCHECK(glBindBuffer(target, id));
      
      if (b.first->info.size_ != d.size)
      {
        GLCHECK(glBufferData(target, d.size, nullptr, Translate(b.first->info.usage_)));
        b.first->info.size_ = d.size;
      }

	    GLCHECK(glBufferSubData(target, d.offset, d.size, d.data));

      if (target == GL_UNIFORM_BUFFER)
      {
        GLCHECK(glBindBufferBase(target, id, id));
      }
    }

    static void InitTextureParams(GLenum target, const Texture::Info &info)
    {
      GLCHECK(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, Translate(info.magnification_filter)));
      GLCHECK(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, Translate(info.minification_filter)));
      GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_S, Translate(info.wrapping[0])));
      if (target > TextureType::T1D) GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_T, Translate(info.wrapping[1])));
      if (target > TextureType::T2D) GLCHECK(glTexParameteri(target, GL_TEXTURE_WRAP_R, Translate(info.wrapping[2])));
    }

    static void InitTexture(std::pair<TextureInstance*, BackEnd::Texture*> t)
    {
      GLuint id = t.second->texture;
      auto& back_end = t.second;
      if (!id)
      {
        GLCHECK(glGenTextures(1, &id));
        back_end->texture = id;
        t.first->id = id;
        switch (t.first->info.format) {
        case TexelsFormat::R_U8:
          back_end->format = GL_RED;
          back_end->internal_format = GL_R8;
          back_end->type = GL_UNSIGNED_BYTE;
          break;
        case TexelsFormat::RG_U8:
          back_end->format = GL_RG;
          back_end->internal_format = GL_RG8;
          back_end->type = GL_UNSIGNED_BYTE;
          break;
        case TexelsFormat::RGB_U8:
          back_end->format = GL_RGB;
          back_end->internal_format = GL_RGB8;
          back_end->type = GL_UNSIGNED_BYTE;
          break;
        case TexelsFormat::RGBA_U8:
          back_end->format = GL_RGBA;
          back_end->internal_format = GL_RGBA8;
          back_end->type = GL_UNSIGNED_BYTE;
          break;
        case TexelsFormat::Depth_U16:
          back_end->format = GL_DEPTH_COMPONENT;
          back_end->internal_format = GL_DEPTH_COMPONENT16;
          back_end->type = GL_UNSIGNED_SHORT;
          break;
        case TexelsFormat::DepthStencil_U16:
          back_end->format = GL_DEPTH_STENCIL;
          back_end->internal_format = GL_DEPTH24_STENCIL8;
          back_end->type = GL_UNSIGNED_SHORT;
          break;
        case TexelsFormat::Depth_U24:
          back_end->format = GL_DEPTH_COMPONENT;
          back_end->internal_format = GL_DEPTH_COMPONENT24;
          back_end->type = GL_UNSIGNED_INT;
          break;
        case TexelsFormat::DepthStencil_U24:
          back_end->format = GL_DEPTH_STENCIL;
          back_end->internal_format = GL_DEPTH24_STENCIL8;
          back_end->type = GL_UNSIGNED_INT;
          break;
        }
        switch (t.first->info.type)
        {
        case TextureType::T1D:
          t.second->target = GL_TEXTURE_1D;
          GLCHECK(glBindTexture(GL_TEXTURE_1D, id));
          GLCHECK(glTexImage1D(GL_TEXTURE_1D, 0, back_end->internal_format, t.first->info.width, 0, back_end->format, back_end->type, nullptr));
          InitTextureParams(GL_TEXTURE_1D, t.first->info);
          break;
        case TextureType::T2D:
          t.second->target = GL_TEXTURE_2D;
          GLCHECK(glBindTexture(GL_TEXTURE_2D, id));
          GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, back_end->internal_format, t.first->info.width, t.first->info.height, 0, back_end->format, back_end->type, nullptr));
          InitTextureParams(GL_TEXTURE_2D, t.first->info);
          break;
        case TextureType::T3D:
          t.second->target = GL_TEXTURE_3D;
          GLCHECK(glBindTexture(GL_TEXTURE_3D, id));
          GLCHECK(glTexImage3D(GL_TEXTURE_3D, 0, back_end->internal_format, t.first->info.width, t.first->info.height, t.first->info.depth, 0, back_end->format, back_end->type, nullptr));
          InitTextureParams(GL_TEXTURE_3D, t.first->info);
          break;
        case TextureType::CubeMap:
          ///
          break;
        }
      }
    }

    void FillTexture(DisplayList::FillTextureData& d)
    {
      auto t = RenderContext::GetResource(d.texture.id, &d.texture.ctx->textures_, &d.texture.ctx->back_end_->textures);
      InitTexture(t);
      auto& back_end = *t.second;

      d.width = d.width ? d.width : t.first->info.width;
      d.height = d.height ? d.height : t.first->info.height;
      d.depth = d.depth ? d.depth : t.first->info.depth;

      GLCHECK(glBindTexture(back_end.target, back_end.texture));
      GLCHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
      switch (t.first->info.type)
      {
      case TextureType::T1D:
        GLCHECK(glTexSubImage1D(GL_TEXTURE_1D, 0, d.offset_x, d.width, back_end.format, back_end.type, d.data))
          break;
      case TextureType::T2D:
        GLCHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, d.offset_x, d.offset_y, d.width, d.height, back_end.format, back_end.type, d.data))
          break;
      case TextureType::T3D:
        GLCHECK(glTexSubImage3D(GL_TEXTURE_3D, 0, d.offset_x, d.offset_y, d.offset_z, d.width, d.height, d.depth, back_end.format, back_end.type, d.data))
          break;
      case TextureType::CubeMap:
        ///
        break;
      }
      if (d.build_mipmap) GLCHECK(glGenerateMipmap(back_end.target));
    }

    static uint32 CompileShader(GLenum type, const char *src)
    {
      uint32 shader = glCreateShader(type);
      if (!shader)
      {
        OnError("OpenGL: Could not create Shader");
        return 0;
      }
      if (shader)
      {
        int32 compiled = 0;
        GLCHECK(glShaderSource(shader, 1, &src, NULL));
        GLCHECK(glCompileShader(shader));
        GLCHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));

        if (!compiled)
        {
          static const size_t max_length = 2048;
          char buffer[max_length];
          GLCHECK(glGetShaderInfoLog(shader, max_length, NULL, buffer));
          OnError("Error Compiling Shader(%d):%s\nCODE:\n%.256s\n", type, buffer, src);
          GLCHECK(glDeleteShader(shader));
          return 0;
        }
      }
      return shader;
    }

    void SetupMaterial(DisplayList::SetupMaterialData& d)
    {
      bool main_material_changed = !(d.material.id == d.material.ctx->main_material.material.id);
      d.material.ctx->main_material = d;
      auto mat = RenderContext::GetResource(d.material.ctx->main_material.material.id, &d.material.ctx->materials_, &d.material.ctx->back_end_->materials);
      if (main_material_changed)
      {
        if (mat.second->program == 0)
        {
          for (auto &i : mat.second->texture_uniforms_location) { i = -1; }

          uint32 shader_v = CompileShader(GL_VERTEX_SHADER, mat.first->vert_shader.c_str());
          uint32 shader_f = CompileShader(GL_FRAGMENT_SHADER, mat.first->frag_shader.c_str());
          if (!shader_v || !shader_f)
          {
            return;
          }

          uint32 program_id = glCreateProgram();
          if (!program_id)
          {
            OnError("Could not create program object");
            return;
          }
          GLCHECK(glAttachShader(program_id, shader_v));
          GLCHECK(glAttachShader(program_id, shader_f));

          for (auto i = 0; i < kMaxVertexAttribs; ++i)
          {
            const auto &attrib = mat.first->info.attribs[i];
            if (attrib.format)
            {
              GLCHECK(glBindAttribLocation(program_id, i, attrib.name));
            }
            else
            {
              break;
            }
          }

          GLCHECK(glLinkProgram(program_id));
          int32 link_status = GL_FALSE;
          GLCHECK(glGetProgramiv(program_id, GL_LINK_STATUS, &link_status));
          if (link_status != GL_TRUE)
          {
            char log[2048];
            GLCHECK(glGetProgramInfoLog(program_id, 2048, NULL, log));
            GLCHECK(glDeleteProgram(program_id));
            OnError("Could not link program --> %s", log);
            return;
          }
          mat.second->program = program_id;

          GLCHECK(glDeleteShader(shader_v));
          GLCHECK(glDeleteShader(shader_f));

          for (auto i = 0; i < kMaxTextureUnits; ++i)
          {
            char name[32];
            snprintf(name, 32, "u_tex%d", i);
            if (mat.first->info.textures[i])
            {
              mat.second->texture_uniforms_location[i] = glGetUniformLocation(program_id, name);
            }
          }
        }

        GLCHECK(glUseProgram(mat.second->program));

        if (mat.first->info.blend.enabled)
        {
          GLCHECK(glEnable(GL_BLEND));
          vec4 c = mat.first->info.blend.color;
          GLCHECK(glBlendColor(c.r, c.g, c.b, c.a));
          GLCHECK(glBlendEquationSeparate(Translate(mat.first->info.blend.op_rgb), Translate(mat.first->info.blend.op_alpha)));
          GLCHECK(glBlendFuncSeparate(Translate(mat.first->info.blend.src_rgb), Translate(mat.first->info.blend.dst_rgb), Translate(mat.first->info.blend.src_alpha), Translate(mat.first->info.blend.dst_alpha)));
        }
        else
        {
          GLCHECK(glDisable(GL_BLEND));
        }

        switch (mat.first->info.cull)
        {
        case Cull::Front:
          GLCHECK(glEnable(GL_CULL_FACE));
          GLCHECK(glCullFace(GL_FRONT));
          break;
        case Cull::Back:
          GLCHECK(glEnable(GL_CULL_FACE));
          GLCHECK(glCullFace(GL_BACK));
          break;
        case Cull::Disabled:
          GLCHECK(glDisable(GL_CULL_FACE));
          break;
        }

        GLboolean rgbw = (mat.first->info.rgba_write) ? GL_TRUE : GL_FALSE;
        GLboolean dephtw = (mat.first->info.depth_write) ? GL_TRUE : GL_FALSE;
        GLCHECK(glColorMask(rgbw, rgbw, rgbw, rgbw));
        GLCHECK(glDepthMask(dephtw));

        if (mat.first->info.depth_func != CompareFunc::Disabled)
        {
          GLCHECK(glDepthFunc(Translate(mat.first->info.depth_func)));
          GLCHECK(glEnable(GL_DEPTH_TEST));
        }
        else
        {
          GLCHECK(glDisable(GL_DEPTH_TEST));
        }

        for (auto i = 0; i < kMaxVertexAttribs; ++i)
        {
          const auto &attrib = mat.first->info.attribs[i];
          if (attrib.format)
          {
            GLCHECK(glEnableVertexAttribArray(i));
            switch (attrib.vertex_step)
            {
            case VertexStep::PerVertex:
              GLCHECK(glVertexAttribDivisor(i, 0));
              break;
            case VertexStep::PerInstance:
              GLCHECK(glVertexAttribDivisor(i, 1));
              break;
            }
          }
          else
          {
            GLCHECK(glDisableVertexAttribArray(i));
          }
        }

        for (auto i = 0; i < kMaxUniformBuffers; ++i)
        {
          if (d.uniform_buffer[i].id)
          {
            auto ubo = RenderContext::GetResource(d.uniform_buffer[i].id, &d.material.ctx->buffers_, &d.material.ctx->back_end_->buffers);
            GLuint uniformIndex = glGetUniformBlockIndex(mat.second->program, ubo.first->info.name_);
            GLCHECK(glUniformBlockBinding(mat.second->program, uniformIndex, ubo.second->buffer));
          }
        }
      }

      glUniformMatrix4fv(glGetUniformLocation(mat.second->program, Shader::u_model), 1, GL_FALSE, &d.model_matrix[0][0]);

      if (d.scissor.z > 0.0f && d.scissor.w > 0.0f)
      {
        GLCHECK(glScissor(d.scissor.x, d.scissor.y, d.scissor.z, d.scissor.w));
        GLCHECK(glEnable(GL_SCISSOR_TEST));
      }
      else
      {
        GLCHECK(glDisable(GL_SCISSOR_TEST));
      }
    }

    void SetupView(DisplayList::SetupViewData& d)
    {
      if (d.framebuffer.id != 0)
      {
        auto fb = RenderContext::GetResource(d.framebuffer.id, &d.framebuffer.ctx->framebuffers_, &d.framebuffer.ctx->back_end_->framebuffers);
        GLuint fb_id = fb.second->framebuffer;
        if (!fb_id)
        {
          GLCHECK(glGenFramebuffers(1, &fb_id));
          GLCHECK(glBindFramebuffer(GL_FRAMEBUFFER, fb_id));
          for (uint16 i = 0; i < fb.first->info.num_color_textures; ++i)
          {
            auto tex = RenderContext::GetResource(fb.first->color_textures[i].id, &fb.first->color_textures[i].ctx->textures_, &fb.first->color_textures[i].ctx->back_end_->textures);
            InitTexture(tex);
            if (tex.second->target != GL_TEXTURE_2D)
            {
              OnError("Invalid texture type, expected texture 2D (color %u)", i);
            }
            GLCHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex.second->texture, 0));
          }
          if (RenderContext::CheckValidResource(fb.first->depth_texture.id, &d.framebuffer.ctx->textures_))
          {
            auto tex = RenderContext::GetResource(fb.first->depth_texture.id, &fb.first->depth_texture.ctx->textures_, &fb.first->depth_texture.ctx->back_end_->textures);
            InitTexture(tex);
            if (tex.second->target != GL_TEXTURE_2D)
            {
              OnError("Invalid texture type, expected texture 2D (depth/stencil)");
            }
            switch (tex.first->info.format)
            {
            case TexelsFormat::Depth_U16:
            case TexelsFormat::Depth_U24:
              GLCHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.second->texture, 0));
              break;
            case TexelsFormat::DepthStencil_U16:
            case TexelsFormat::DepthStencil_U24:
              GLCHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.second->texture, 0));
              break;
            default:
              OnError("Invalid texels-format for a depth/stencil texture ... was %u", tex.first->info.format);
            }
          }

          fb.second->framebuffer = fb_id;
        }
        else 
        {
          GLCHECK(glBindFramebuffer(GL_FRAMEBUFFER, fb_id));          
        }
        if (fb.first->info.size.x != d.resolution.x || fb.first->info.size.y != d.resolution.y)
        {
          for (uint16 i = 0; i < fb.first->info.num_color_textures; ++i)
          {
            auto tex = RenderContext::GetResource(fb.first->color_textures[i].id, &fb.first->color_textures[i].ctx->textures_, &fb.first->color_textures[i].ctx->back_end_->textures);

            GLCHECK(glBindTexture(tex.second->target, tex.second->texture));
            GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, tex.second->internal_format, d.resolution.x, d.resolution.y, 0, tex.second->format, tex.second->type, nullptr));
            tex.first->info.width = d.resolution.x;
            tex.first->info.height = d.resolution.y;
          }
          if (RenderContext::CheckValidResource(fb.first->depth_texture.id, &d.framebuffer.ctx->textures_))
          {
            auto tex = RenderContext::GetResource(fb.first->depth_texture.id, &fb.first->depth_texture.ctx->textures_, &fb.first->depth_texture.ctx->back_end_->textures);
            GLCHECK(glBindTexture(tex.second->target, tex.second->texture));
            GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, tex.second->internal_format, d.resolution.x, d.resolution.y, 0, tex.second->format, tex.second->type, nullptr));
            tex.first->info.width = d.resolution.x;
            tex.first->info.height = d.resolution.y;
          }
          fb.first->info.size = d.resolution;
        }
      }
      else 
      {
        GLCHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
      }
      if (d.viewport.width != 0 && d.viewport.height != 0)
      {
        GLCHECK(glViewport(d.viewport.x, d.viewport.y, d.viewport.width, d.viewport.height));
      }
      GLCHECK(glDisable(GL_SCISSOR_TEST));
    }

    void Render(DisplayList::RenderData& d)
    {
      auto buf = RenderContext::GetResource(d.index_buffer.id, &d.index_buffer.ctx->buffers_, &d.index_buffer.ctx->back_end_->buffers);
      auto mat = RenderContext::GetResource(d.index_buffer.ctx->main_material.material.id, &d.index_buffer.ctx->materials_, &d.index_buffer.ctx->back_end_->materials);

      if (mat.first->info.render_mode == RenderMode::Disabled)
      {
        return;
      }

      if (!buf.second->buffer)
      {
        OnError("Invalid Index buffer...");
      }

      auto &main_material = d.index_buffer.ctx->main_material;

      size_t tex_unit = 0;
      for (auto i = 0; i < kMaxTextureUnits; ++i)
      {
        if (mat.second->texture_uniforms_location[i] >= 0)
        {
          auto tex = RenderContext::GetResource(main_material.texture[i].id, &d.index_buffer.ctx->textures_, &d.index_buffer.ctx->back_end_->textures);
          if (!tex.first)
          {
            OnError("Missing texture.");
            return;
          }

          GLCHECK(glActiveTexture(GL_TEXTURE0 + tex_unit));
          switch (tex.first->info.type) {
          case TextureType::T1D: GLCHECK(glBindTexture(GL_TEXTURE_1D, tex.second->texture)); break;
          case TextureType::T2D: GLCHECK(glBindTexture(GL_TEXTURE_2D, tex.second->texture)); break;
          case TextureType::T3D: GLCHECK(glBindTexture(GL_TEXTURE_3D, tex.second->texture)); break;
          default:
            OnError("Invalid texture type...");
            return;
          }

          GLCHECK(glUniform1i(mat.second->texture_uniforms_location[i], tex_unit));
          tex_unit++;
        }
      }

      for (auto i = 0; i < kMaxVertexAttribs; ++i)
      {
        uint32 attrib_format = mat.first->info.attribs[i].format;
        if (attrib_format)
        {
          size_t buffer_index = mat.first->info.attribs[i].buffer_index;
          size_t buffer_id = main_material.buffer[buffer_index].id;
          if (!buffer_id)
          {
            OnError("Expected Valid buffer (see pipeline declaration)");
            return;
          }
          auto buffer = RenderContext::GetResource(buffer_id, &d.index_buffer.ctx->buffers_, &d.index_buffer.ctx->back_end_->buffers);
          if (buffer.second->buffer == 0)
          {
            OnError("Invalid OpenGL-buffer (vertex data)");
          }

          GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer.second->buffer));
          GLint a_size = (attrib_format & VertexFormat::NumComponentsMask) >> VertexFormat::NumComponentsShift;
          GLenum a_type = TranslateVertexType(attrib_format);
          GLboolean a_normalized = (attrib_format & VertexFormat::Normalized) ? GL_TRUE : GL_FALSE;
          GLsizei a_stride = mat.first->info.attribs[i].stride;
          GLsizei a_offset = mat.first->info.attribs[i].offset;
          GLCHECK(glVertexAttribPointer(i, a_size, a_type, a_normalized, a_stride, (void*)a_offset));
          glEnableVertexAttribArray(i);
        }
        else
        {
          break;
        }
      }

      switch (mat.first->info.render_mode)
      {
      case RenderMode::Disabled:
        break;
      case RenderMode::Solid:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(Translate(mat.first->info.primitive), d.count, Translate(d.type), (void*)d.offset, d.instances));
        break;
      case RenderMode::Wireframe:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(Translate(mat.first->info.primitive), d.count, Translate(d.type), (void*)d.offset, d.instances));
        break;
      case RenderMode::SolidWireframe:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(Translate(mat.first->info.primitive), d.count, Translate(d.type), (void*)d.offset, d.instances));
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf.second->buffer));
        GLCHECK(glDrawElementsInstanced(Translate(mat.first->info.primitive), d.count, Translate(d.type), (void*)d.offset, d.instances));
        break;
      }
    }

    static GLenum TranslateVertexType(uint32 format)
    {
      format = format & VertexFormat::TypeMask;
      switch (format)
      {
      case VertexFormat::Float:  return GL_FLOAT; break;
      case VertexFormat::Int8:   return GL_BYTE; break;
      case VertexFormat::UInt8:  return GL_UNSIGNED_BYTE; break;
      case VertexFormat::Int16:  return GL_SHORT; break;
      case VertexFormat::UInt16: return GL_UNSIGNED_SHORT; break;
      case VertexFormat::Int32:  return GL_INT; break;
      case VertexFormat::UInt32: return GL_UNSIGNED_INT; break;
      default:
        // ERROR
        assert(!"Invalid Vertex Type");
        return 0;
      }
    }

    static GLenum Translate(Usage::Enum e)
    {
      switch (e)
      {
      case Usage::Static:  return GL_STATIC_DRAW;
      case Usage::Dynamic: return GL_DYNAMIC_DRAW;
      case Usage::Stream:  return GL_STREAM_DRAW;
      }
      // ERROR!
      assert(!"Invalid Usage");
      return 0;
    }

    static GLenum Translate(IndexFormat::Enum e)
    {
      switch (e)
      {
      case IndexFormat::UInt8:  return GL_UNSIGNED_BYTE;
      case IndexFormat::UInt16: return GL_UNSIGNED_SHORT;
      case IndexFormat::UInt32: return GL_UNSIGNED_INT;
      }
      // ERROR!
      assert(!"Invalid IndexFormat");
      return 0;
    }

    static GLenum Translate(SamplerFiltering::Enum e)
    {
      switch (e)
      {
      case SamplerFiltering::Nearest:              return GL_NEAREST;
      case SamplerFiltering::Linear:               return GL_LINEAR;
      case SamplerFiltering::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
      case SamplerFiltering::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
      case SamplerFiltering::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
      case SamplerFiltering::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
      }
      // ERROR !
      assert(!"Invalid Sampler Filtering");
      return 0;
    }

    static GLenum Translate(SamplerWrapping::Enum e)
    {
      switch (e)
      {
      case SamplerWrapping::Repeat:         return GL_REPEAT;
      case SamplerWrapping::MirroredRepeat: return GL_MIRRORED_REPEAT;
      case SamplerWrapping::Clamp:          return GL_CLAMP_TO_EDGE;
      }
      // ERROR !
      assert(!"Invalid Sampler Wrapping");
      return 0;
    }

    static GLenum Translate(Primitive::Enum e)
    {
      switch (e)
      {
      case Primitive::Lines:     return GL_LINES;
      case Primitive::Triangles: return GL_TRIANGLES;
      case Primitive::Points:    return GL_POINTS;
      }
      // ERROR!
      assert(!"Invalid Primitive");
      return 0;
    }

    static GLenum Translate(BlendFactor::Enum e)
    {
      switch (e)
      {
      case BlendFactor::Zero: return GL_ZERO;
      case BlendFactor::One: return GL_ONE;
      case BlendFactor::SrcColor: return GL_SRC_COLOR;
      case BlendFactor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
      case BlendFactor::SrcAlpha: return GL_SRC_ALPHA;
      case BlendFactor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
      case BlendFactor::DstColor: return GL_DST_COLOR;
      case BlendFactor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
      case BlendFactor::DstAlpha: return GL_DST_ALPHA;
      case BlendFactor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
      case BlendFactor::SrcAlphaSaturated: return GL_SRC_ALPHA_SATURATE;
      case BlendFactor::BlendColor: return GL_BLEND_COLOR;
      case BlendFactor::OneMinusBlendColor: return GL_ONE_MINUS_CONSTANT_COLOR;
      case BlendFactor::BlendAlpha: return GL_CONSTANT_ALPHA;
      case BlendFactor::OneMinusBlendAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
      }
      // ERROR!
      assert(!"Invalid BlendFactor");
      return 0;
    }

    static GLenum Translate(BlendOp::Enum e)
    {
      switch (e)
      {
      case BlendOp::Add: return GL_FUNC_ADD;
      case BlendOp::Substract: return GL_FUNC_SUBTRACT;
      case BlendOp::ReverseSubstract: return GL_FUNC_REVERSE_SUBTRACT;
      case BlendOp::Min: return GL_MIN;
      case BlendOp::Max: return GL_MAX;
      }
      // ERROR!
      assert(!"Invalid BlendOp");
      return 0;
    }

    static GLenum Translate(CompareFunc::Enum e)
    {
      switch (e)
      {
      case CompareFunc::Never:        return GL_NEVER;
      case CompareFunc::Less:         return GL_LESS;
      case CompareFunc::LessEqual:    return GL_LEQUAL;
      case CompareFunc::Equal:        return GL_EQUAL;
      case CompareFunc::NotEqual:     return GL_NOTEQUAL;
      case CompareFunc::GreaterEqual: return GL_GEQUAL;
      case CompareFunc::Greater:      return GL_GREATER;
      case CompareFunc::Always:       return GL_ALWAYS;
      }
      // ERROR!
      assert(!"Invalid CompareFunc");
      return 0;
    }

#undef GLCHECK
#undef GLCHECK_STR
#undef GLCHECK_STR_STR
  }
}

#endif