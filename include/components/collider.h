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

#include "../../include/core/component.h"
#include "../../include/graphics/materials/material_instance.h"
#include "../../include/physics/collider_shape.h"

/**
* \file collider.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief 
*
*/
namespace vxr 
{
  
	class Collider : public Component
  {
    VXR_OBJECT(Collider, Component);
	public:
    Collider();
		~Collider();

    virtual void onGUI() override;

    void set_shape(ref_ptr<ColliderShape> shape);
    ref_ptr<ColliderShape> shape() const;

  private:
    ref_ptr<ColliderShape> shape_;
	};

  namespace System 
  {
    class Collider : public ComponentSystem
    {
      VXR_COMPONENT_SYSTEM(Collider, ComponentSystem);
    public:
      Collider();
      ~Collider();
    };

    template<> class Getter<vxr::Collider>
    {
    public:
      static ref_ptr<System::Collider> get();
    };

  }

} /* end of vxr namespace */