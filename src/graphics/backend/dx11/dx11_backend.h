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

#ifdef VXR_DX11

#include "../../../include/graphics/display_list.h"

#include "dx11.h"

/**
* \file dx11_backend.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief
*
*/
namespace vxr
{

  namespace gpu
  {
    
    struct BackEnd : public Object
    {

      VXR_OBJECT(BackEnd, Object);

      struct Buffer
      {

      };

      struct Texture
      {

      };

      struct Material
      {

      };

      struct Framebuffer 
      {

      };

      scoped_array<Buffer>      buffers;
      scoped_array<Texture>     textures;
      scoped_array<Material>    materials;
      scoped_array<Framebuffer> framebuffers;
    };

    void InitBackEnd(BackEnd** back_end, const Params::GPU &params = Params::GPU());
    void DestroyBackEnd(BackEnd** back_end);

    void ClearScreen(const DisplayList::ClearData& d);
    void FillBuffer(DisplayList::FillBufferData& d);
    void FillTexture(DisplayList::FillTextureData& d);
    void SetupMaterial(DisplayList::SetupMaterialData& d);
    void SetupView(DisplayList::SetupViewData& d);
    void Render(DisplayList::RenderData& d);

  } /* end of gpu namespace */

} /* end of vxr namespace */

#endif