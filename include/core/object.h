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

#include "../memory/referenced.h"
#include "../engine/types.h"
#include "../utils/color.h"
#include "../utils/math.h"

/**
* \file object.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Base VXR Object containing basic information such as class name and parent class name, specific object naming, virtual onGUI() function and id tracking in-engine.
*
*/
namespace vxr 
{

#define VXR_OBJECT(type_name, base_type_name) \
   public: \
    using ClassName = type_name; \
    using BaseClassName = base_type_name; \
    virtual string type() const override { return #type_name; } \
    virtual string base_type() const override { return #base_type_name; } \


	class Object : public Referenced 
  {
	public:
		Object();
		virtual ~Object();

    virtual void onGUI();
    
    virtual string type() const = 0;
    virtual string base_type() const = 0;

    void set_name(string name);
    string name() const;

    uint32 id() const;

  protected:
    string uiText(string label);

  private:
    string name_;
    uint32 id_;
	};

} /* end of vxr namespace */