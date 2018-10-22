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


#include "../../include/graphics/window.h"

#if defined (VXR_OPENGL)
#  include "opengl/gl_window.h"
#elif defined (VXR_DX11)
#  include "dx11/dx11_window.h"
#else
#  error Backend must be defined on GENie.lua (e.g. --gl OR --dx11).
#endif

#ifdef VXR_UI
#  include "../../include/graphics/ui.h"
#endif

namespace vxr 
{
	
	Window::Window() 
  {
    set_name("Window");
		data_ = new Data();
    data_->name = "VXR Window";
	}
	
	Window::~Window() 
  {
		delete data_;
	}
	
	void Window::init() 
  {
    data_->width = params_.size.x;
    data_->height = params_.size.y;

    window::init(data_);
#ifdef VXR_UI
    ui::Init(data_);
    ui::Update(data_); /// Doing a first frame for synchronization (instead of having to pass a "is first time" bool to update).
#endif
	}

	bool Window::is_exiting() 
  {
    return window::is_exiting(data_);
	}

	void Window::forceExit()
  {
    window::forceExit(data_);
	}

  void Window::update(bool *swap)
  {
#ifdef VXR_UI
    ui::Draw();
#endif
    if (*swap)
    {
      VXR_TRACE_BEGIN("VXR", "Window Swap");
      window::swap(data_);
      *swap = false;
      VXR_TRACE_END("VXR", "Window Swap");
    }
#  ifdef VXR_THREADING
    ///set_title(data_); /// This is quite expensive
#  endif
    VXR_TRACE_BEGIN("VXR", "Window Events");
    window::events(data_);
    VXR_TRACE_END("VXR", "Window Events");
#ifdef VXR_UI
    ui::Update(data_);
#endif
  }

	void Window::stop() 
  {
#ifdef VXR_UI
    ui::Draw();
    ui::Stop(data_);
#endif
    window::stop(data_);
	}

  double Window::uptime()
  {
    return window::uptime(data_);
  }

  void Window::set_title(string title)
  {
    data_->name = title;
#ifndef VXR_THREADING
    window::set_title(data_);
#endif
  }
}