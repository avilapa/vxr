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

#include "../core/object.h"

#include <functional>

/**
* \file window.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Window class.
*
*/
namespace vxr
{

	class Window : public Object 
  {
    VXR_OBJECT(Window, Object)
    friend class GPU;
    friend class Engine;
	public:
		Window();
		~Window();

    Params::Window params() { return params_; }

    void forceExit();

    void set_title(string title);

    bool is_exiting();
    double uptime();

    struct Data;
  protected:
    void set_preinit_params(const Params::Window& params) { params_ = params; };

    void init();
    void events();
    void swap();
    void stop();

	protected:
    Data* data_;

#ifdef VXR_UI
      std::function<void()> ui_ = []() {};
#endif

    Params::Window params_ = { { 1280, 720 } };
	};

} /* end of vxr namespace */