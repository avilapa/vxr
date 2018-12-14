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

#include "../../../../include/graphics/window.h"
#include "../../../../deps/imgui/imgui.h"

/**
* \file gl_window_struct.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief OpenGL (GLFW) window structure.
*
*/
namespace vxr 
{

	struct Window::Data 
  {
		GLFWwindow *window;
    GLFWcursor *mouse_cursors[ImGuiMouseCursor_COUNT] = { 0 };
    string name;
    std::atomic<uint16> width;
    std::atomic<uint16> height;

    double time = 0.0;
	};

  static bool mouse_just_pressed[5] = { false, false, false, false, false }; /// Don't try Anakin, I have the global ground.
	
} /* end of vxr namespace */

#endif