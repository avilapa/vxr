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

#include "dx11_backend.h"

#ifdef VXR_DX11

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
  int32_t error;// = glGetError();
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

    /*static GLenum TranslateVertexType(uint32 format);
    static GLenum Translate(Usage::Enum e);
    static GLenum Translate(IndexFormat::Enum e);
    static GLenum Translate(SamplerFiltering::Enum e);
    static GLenum Translate(SamplerWrapping::Enum e);
    static GLenum Translate(Primitive::Enum e);
    static GLenum Translate(Cull::Enum e);
    static GLenum Translate(BlendFactor::Enum e);
    static GLenum Translate(BlendOp::Enum e);
    static GLenum Translate(CompareFunc::Enum e);*/

    void ClearScreen(const DisplayList::ClearData& d)
    {

    }

    void FillBuffer(DisplayList::FillBufferData& d)
    {
      
    }
    /*
    static void InitTextureParams(GLenum target, const Texture::Info &info)
    {
      
    }
    */
    static void InitTexture(std::pair<TextureInstance*, BackEnd::Texture*> t)
    {
      
    }

    void FillTexture(DisplayList::FillTextureData& d)
    {
      
    }
    /*
    static uint32 CompileShader(GLenum type, const char *src)
    {

    }
    */
    void SetupMaterial(DisplayList::SetupMaterialData& d)
    {
      
    }

    void SetupView(DisplayList::SetupViewData& d)
    {
      
    }

    void Render(DisplayList::RenderData& d)
    {
      
    }
    /*
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

    static GLenum Translate(Cull::Enum e)
    {
      switch (e)
      {
      case Cull::Disabled: return GL_FRONT_AND_BACK;
      case Cull::Front:    return GL_FRONT;
      case Cull::Back:     return GL_BACK;
      }
      // ERROR!
      assert(!"Invalid Cull");
      return 0;
    }

    static GLenum Translate(RenderMode::Enum e)
    {
      switch (e)
      {
      case RenderMode::Solid:          return GL_FRONT;
      case RenderMode::Wireframe:      return GL_BACK;
      case RenderMode::SolidWireframe: return GL_BACK;
      }
      // ERROR!
      assert(!"Invalid Render Mode");
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
    */
#undef GLCHECK
#undef GLCHECK_STR
#undef GLCHECK_STR_STR
  }
}

#endif