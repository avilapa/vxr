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

#include "gpu_resources.h"

// ----------------------------------------------------------------------------------------
//  The following structures have been partially extracted from px_render.h 
//  by Jose L. Hidalgo (PpluX), and later modified to fit vxr needs.
//  Link: https://github.com/pplux/px/blob/master/px_render.h
// ----------------------------------------------------------------------------------------

/**
* \file gpu_isntances.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief GPU Buffer, Texture, Material and Framebuffer instances to be stored within the GPU.
*
*/
namespace vxr
{

  namespace gpu
  {

    struct InstanceBase
    {
      uint32 version = 0;
      std::atomic<uint32> state = { 0 };

      bool acquire()
      {
        uint32 v = (version + 1);
        if (!v) v = 1;
        uint32 e = 0;
        if (state.compare_exchange_weak(e, v))
        {
          version = v;
          return true;
        }
        return false;
      }

      void release()
      {
        state = 0;
      }
    };

    struct BufferInstance : public InstanceBase
    {
      Buffer::Info info;
    };

    struct TextureInstance : public InstanceBase
    {
      Texture::Info info;
      size_t bytes_per_pixel = 0;
      uint32 id;
    };

    struct MaterialInstance : public InstanceBase
    {
      Material::Info info;
      string vert_shader;
      string frag_shader;
      string attributes[kMaxVertexAttribs];
    };

    struct FramebufferInstance : public InstanceBase {
      Framebuffer::Info info;
      Texture color_textures[kMaxFramebufferColorTextures];
      Texture depth_texture;
    };

  } /* end of gpu namespace */

} /* end of vxr namespace */