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

#include "gl.h"

#ifndef _GLFW_WIN32
#  define _GLFW_WIN32
#endif

#include "../../../../deps/glfw/include/GLFW/glfw3.h"

#ifdef _WIN32 /// Belongs to ImGui
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../../../../deps/glfw/include/GLFW/glfw3native.h"
#endif
#define GLFW_HAS_WINDOW_TOPMOST     (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED     (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA       (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI    (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN             (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3200) // 3.2+ glfwCreateWindowSurface

#include "gl_window_struct.h"

/**
* \file backend_opengl.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief OpenGL (GLFW) window functions.
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