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

#include "gl_window.h"

#include "../../../../deps/stb/stb_image.h"

#ifdef VXR_OPENGL

namespace vxr
{

  void window::init(Window::Data* data)
  {
    if (!glfwInit())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Window failed at startup.\n");
      return;
    }

    data->window = glfwCreateWindow(data->width, data->height, data->name.c_str(), NULL, NULL);

    if (data->window == nullptr)
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not allocate window.\n");
      return;
    }

    glfwMakeContextCurrent(data->window);

#ifdef VXR_GLEW_EXPERIMENTAL
    glewExperimental = GL_TRUE;
#endif
    if (glewInit() != GLEW_OK)
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not initializer backend.\n");
      return;
    }

    glfwSwapInterval(true);

    GLFWimage images[1]; 
    images[0].pixels = stbi_load("../../assets/textures/avilapa.png", &images[0].width, &images[0].height, 0, 4); 
    glfwSetWindowIcon(data->window, 1, images); 
    stbi_image_free(images[0].pixels);

    /// TODO: Make own callbacks instead of ImGui ones.
  }

  void window::swap(Window::Data* data)
  {
    glfwSwapBuffers(data->window);
  }

  void window::events(Window::Data* data)
  {
    glfwPollEvents();
  }

  void window::stop(Window::Data* data)
  {
    glfwTerminate();
  }

  bool window::is_exiting(Window::Data* data)
  {
    return glfwWindowShouldClose(data->window);
  }

  double window::uptime(Window::Data* data)
  {
    return glfwGetTime();
  }

  void window::forceExit(Window::Data* data)
  {
    glfwSetWindowShouldClose(data->window, GLFW_TRUE);
  }

  void window::set_title(Window::Data* data)
  {
    glfwSetWindowTitle(data->window, data->name.c_str());
  }

}

#endif