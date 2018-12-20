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
#include "../../include/core/gameobject.h"

namespace vxr 
{

  Component::Component()
  {
    set_name("Component");
  }

  Component::~Component()
  {

  }

  void Component::onGUI()
  {
    ImGui::PushStyleColor(0, ImVec4(Color::Crimson.r, Color::Crimson.g, Color::Crimson.b, 1.0f));
    ImGui::TextWrapped("Override 'void onGUI()' function to show custom content");
    ImGui::PopStyleColor();
  }

  ref_ptr<GameObject> Component::gameObject()
  {
    return obj_.get();
  }

  ref_ptr<Transform> Component::transform()
  {
    return transform_.get();
  }

  System::ComponentSystem::ComponentSystem()
  {

  }

  System::ComponentSystem::~ComponentSystem()
  {

  }

}