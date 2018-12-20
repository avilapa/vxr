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

// ----------------------------------------------------------------------------------------
// Engine Includes
// ----------------------------------------------------------------------------------------

#include "../memory/ref_ptr.h"
#include "../memory/scoped_ptr.h"
#include "../memory/scoped_array.h"

#include "../../deps/imgui/imgui.h"
#include "../../deps/imgui/imgui_stl.h"

// ----------------------------------------------------------------------------------------
// Debug
// ----------------------------------------------------------------------------------------

#ifndef VXR_DEBUG_LEVEL
#  define VXR_DEBUG_LEVEL_ERROR   100
#  define VXR_DEBUG_LEVEL_WARNING 200
#  define VXR_DEBUG_LEVEL_INFO    500
#  define VXR_DEBUG_LEVEL_DEBUG   1000

#  define VXR_DEBUG_LEVEL         VXR_DEBUG_LEVEL_INFO
#endif

#ifdef VXR_DEBUG_TRACING
#include "../../deps/minitrace/minitrace.h"

#define VXR_TRACE_SCOPE(c, n)         MTR_SCOPE(c, n)
#define VXR_TRACE_BEGIN(c, n)         MTR_BEGIN(c, n)
#define VXR_TRACE_END(c, n)           MTR_END(c, n)
#define VXR_TRACE_META_THREAD_NAME(n) MTR_META_THREAD_NAME(n)

#else

#define VXR_TRACE_SCOPE(c, n)
#define VXR_TRACE_BEGIN(c, n)
#define VXR_TRACE_END(c, n)
#define VXR_TRACE_META_THREAD_NAME(n)

#endif

#ifndef VXR_DEBUG_FUNC
#  ifdef VXR_DEBUG
#    ifdef VXR_UI
#      define VXR_DEBUG_FUNC(LEVEL,...) {if(LEVEL<=VXR_DEBUG_LEVEL) {fprintf(stdout, __VA_ARGS__);fflush(stdout);}}
#    else
#      define VXR_DEBUG_FUNC(LEVEL,...) {if(LEVEL<=VXR_DEBUG_LEVEL) {fprintf(stdout, __VA_ARGS__);fflush(stdout);}}
#    endif
#  else
#    define VXR_DEBUG_FUNC(...) /*nothing*/
#  endif
#endif

// ----------------------------------------------------------------------------------------
// Platform
// ----------------------------------------------------------------------------------------

#ifdef _WIN32

# include <windows.h>

# include <conio.h>
# include <stdint.h>
# include <cstddef>
# include <cstdarg>
# include <string>
# include <fstream>
# include <sstream>
# include <vector>
# include <atomic>
# include <iterator>

# define GLM_ENABLE_EXPERIMENTAL
# include "../../deps/glm/glm.hpp"
# include "../../deps/glm/gtc/matrix_transform.hpp"
# include "../../deps/glm/gtx/transform.hpp"
# include "../../deps/glm//gtc/quaternion.hpp"
# include "../../deps/glm//gtx/matrix_decompose.hpp"

// ----------------------------------------------------------------------------------------
// Engine Types
// ----------------------------------------------------------------------------------------


namespace vxr
{

  typedef ::int8_t			int8;
  typedef ::int16_t			int16;
  typedef ::int32_t			int32;

  typedef ::uint8_t			uint8;
  typedef ::uint16_t		uint16;
  typedef ::uint32_t		uint32;

  typedef glm::vec2			vec2;
  typedef glm::vec3			vec3;
  typedef glm::vec4			vec4;

  typedef glm::dvec2		dvec2;
  typedef glm::dvec3		dvec3;
  typedef glm::dvec4		dvec4;

  typedef glm::ivec2		ivec2;
  typedef glm::ivec3		ivec3;
  typedef glm::ivec4		ivec4;

  typedef glm::uvec2		uvec2;
  typedef glm::uvec3		uvec3;
  typedef glm::uvec4		uvec4;

  typedef glm::mat3			mat3;
  typedef glm::mat4			mat4;

  typedef std::string   string;
}

#else
#error Missing Platform
#endif

namespace vxr
{
  // ----------------------------------------------------------------------------------------
  // Engine Constants
  // ----------------------------------------------------------------------------------------

  const size_t kMaxVertexAttribs            = 16;
  const size_t kMaxTextureUnits             = 16;
  const size_t kMaxFramebufferColorTextures = 16;

  const size_t kMaxUniformBuffers           = 15;

  const size_t kMaxLightSources             = 30;

#ifdef VXR_OPENGL
  const size_t kGLShaderVersion             = 330;
#endif

  // ----------------------------------------------------------------------------------------
  // Engine Structures
  // ----------------------------------------------------------------------------------------

  struct Params
  {
    //bool headless
    //uint32 log level
    //string log name
    //string assets_path

    //string window_icon
    //uvec2 w_size
    //uvec2 w_pos
    //bool w_resizable
    //bool w_fullscreen
    //bool w_borderless
    //bool w_triple_buffer
    //bool w_vsync

    struct Window
    {
      uvec2 size = { 1280, 720 };
    } window;

    struct GPU
    {
      uint32 max_buffers = 128;
      uint32 max_textures = 128;
      uint32 max_materials = 128;
      uint32 max_framebuffers = 128;
    } gpu;
  };

// ----------------------------------------------------------------------------------------
//  The following structures as well as part of its usage in the engine have been extracted 
//  from px_render.h by Jose L. Hidalgo (PpluX).
//  Link: https://github.com/pplux/px/blob/master/px_render.h

  struct Usage
  {
    enum Enum
    {
      Static,
      Dynamic,
      Stream,
    };
  };

  struct BufferType
  {
    enum Enum
    {
      Invalid = 0,
      Vertex,
      Index,
      Uniform,
    };
  };

  struct TextureType 
  {
    enum Enum 
    {
      Invalid = 0,
      T1D,
      T2D,
      T3D,
      CubeMap,
    };
  };

  struct TexelsFormat 
  {
    enum Enum 
    {
      None = 0,
      R_U8,
      RG_U8,
      RGB_U8,
      RGBA_U8,
      Depth_U16,
      DepthStencil_U16,
      Depth_U24,
      DepthStencil_U24,
    };
  };

  struct SamplerWrapping 
  {
    enum Enum 
    {
      Repeat,
      MirroredRepeat,
      Clamp,
    };
  };

  struct SamplerFiltering 
  {
    enum Enum 
    {
      Nearest,
      Linear,
      NearestMipmapNearest,
      NearestMipmapLinear,
      LinearMipmapNearest,
      LinearMipmapLinear,
    };
  };

  struct Primitive 
  {
    enum Enum 
    {
      Lines,
      Triangles,
      Points,
    };
  };

  struct Cull 
  {
    enum Enum 
    {
      Disabled,
      Front,
      Back,
    };
  };

  struct RenderMode
  {
    enum Enum
    {
      Disabled,
      Solid,
      Wireframe,
      SolidWireframe,
    };
  };

  struct BlendFactor 
  {
    enum Enum 
    {
      Zero,
      One,
      SrcColor,
      OneMinusSrcColor,
      SrcAlpha,
      OneMinusSrcAlpha,
      DstColor,
      OneMinusDstColor,
      DstAlpha,
      OneMinusDstAlpha,
      SrcAlphaSaturated,
      BlendColor,
      OneMinusBlendColor,
      BlendAlpha,
      OneMinusBlendAlpha,
    };
  };

  struct BlendOp 
  {
    enum Enum 
    {
      Add,
      Substract,
      ReverseSubstract,
      Min,
      Max
    };
  };

  struct CompareFunc 
  {
    enum Enum 
    {
      Disabled,
      Never,
      Less,
      LessEqual,
      Equal,
      NotEqual,
      GreaterEqual,
      Greater,
      Always,
    };
  };

  struct VertexFormat 
  {
    enum Enum 
    {
      //-------------------------------
      Undefined = 0,
      Float = 0x1,
      Int8 = 0x2,
      UInt8 = 0x3,
      Int16 = 0x4,
      UInt16 = 0x5,
      Int32 = 0x6,
      UInt32 = 0x7,
      //-------------------------------
      NumComponents1 = 0x10,
      NumComponents2 = 0x20,
      NumComponents3 = 0x30,
      NumComponents4 = 0x40,
      //-------------------------------
      Normalized = 0x100,
      //-------------------------------
      Float1 = Float | NumComponents1,
      Float2 = Float | NumComponents2,
      Float3 = Float | NumComponents3,
      Float4 = Float | NumComponents4,
      //-------------------------------
      TypeMask = 0xF,
      TypeShift = 0,
      NumComponentsMask = 0xF0,
      NumComponentsShift = 4,
      FlagsMask = 0xF00,
      FlagsShift = 8,
    };
  };

  struct VertexStep 
  {
    enum Enum 
    {
      PerVertex,
      PerInstance,
    };
  };

  struct VertexDeclaration 
  {
    const char *name; // name is not needed if you are using attribute layout qualifiers
    uint32 format;
    uint32 buffer_index;
    VertexStep::Enum vertex_step;

    uint32 offset;
    uint32 stride;
  };

  struct IndexFormat 
  {
    enum Enum 
    {
      UInt8,
      UInt16,
      UInt32,
    };
  };

// ----------------------------------------------------------------------------------------
}