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

#include "../../include/components/collider.h"
#include "../../include/components/rigidbody.h"
#include "../../include/engine/engine.h"
#include "../../include/core/gameobject.h"
#include "../../include/core/scene.h"

namespace vxr 
{

  Collider::Collider()
  {
    set_name("Collider");
  }

  Collider::~Collider()
  {
  }

  void Collider::onGUI()
  {
    Component::onGUI();
  }

  void Collider::set_shape(ref_ptr<ColliderShape> shape)
  {
    shape_ = shape;
  }

  ref_ptr<ColliderShape> Collider::shape() const
  {
    return shape_;
  }

  System::Collider::Collider()
  {
    set_name("Collider System");
  }

  System::Collider::~Collider()
  {
  }

  ref_ptr<System::Collider> System::Getter<Collider>::get()
  {
    return Engine::ref().collider();
  }
}