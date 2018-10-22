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

#include "dx11.h"

#include "dx11_window_struct.h"

/**
* \file dx11_window.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief
*
*/
namespace vxr
{

  class Window;

  namespace window
  {

    void init(Window::Data* data);
    void swap(Window::Data* data);
    void events(Window::Data* data);
    void stop(Window::Data* data);

    bool is_exiting(Window::Data* data);
    double uptime(Window::Data* data);
    
    void set_title(Window::Data* data);
    void forceExit(Window::Data* data);

  } /* end of window namespace */

} /* end of vxr namespace */

#endif