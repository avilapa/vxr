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

#ifdef VXR_OPENGL

#include "../../../deps/imgui/imgui.h"
#include "../../../deps/imgui/imgui_stl.h"
#include "gl_window.h"

/**
* \file gl_imgui.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief OpenGL implementation of ImGui example functions.
*
*/
namespace vxr
{

  namespace ui
  {

    namespace impl
    {

      bool Init(Window::Data* data);
      void Update(Window::Data* data);
      void Draw(ImDrawData* draw_data);
      void Stop(Window::Data* data);

      bool createFontsTexture();
      void destroyFontsTexture();
      bool createDeviceObjects();
      void destroyDeviceObjects();

    }
  }
}
#endif