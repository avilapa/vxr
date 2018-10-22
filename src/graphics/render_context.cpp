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

#if defined (VXR_OPENGL)
#  include "../graphics/opengl/gl_backend.h"
#elif defined (VXR_DX11)
#  include "../graphics/dx11/dx11_backend.h"
#else
#  error Backend must be defined on GENie.lua (e.g. try parameters --gl OR --dx11).
#endif

namespace vxr
{

  RenderContext::RenderContext()
  { 
  }

  RenderContext::~RenderContext()
  {
    gpu::DestroyBackEnd(&back_end_);
  }

  void RenderContext::init(const Params::GPU& params)
  {
    gpu::InitBackEnd(&back_end_, params);

    buffers_.alloc(params.max_buffers);
    textures_.alloc(params.max_textures);
    materials_.alloc(params.max_materials);
    framebuffers_.alloc(params.max_framebuffers);

    VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_INFO, "[INFO]: Initialized Render Context with params: \n    - max_buffers (%u), max_textures (%u), max_materials (%u), max_framebuffers (%u)\n", params.max_buffers, params.max_textures, params.max_materials, params.max_framebuffers);
  }

// ----------------------------------------------------------------------------------------
//  The following functions have been extracted from px_render.h by Jose L. Hidalgo (PpluX).
//  Link: https://github.com/pplux/px/blob/master/px_render.h
// ----------------------------------------------------------------------------------------

  uint32 RenderContext::index(uint32 id) {
    return id & 0x000FFFFF;
  }

  std::pair<uint32, uint32> RenderContext::indexAndVersion(uint32 id) {
    uint32 pos = id & 0x000FFFFF;
    uint32 version = (id & 0xFFF00000) >> 20;
    return{ pos,version };
  }
}