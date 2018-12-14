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

#include "../../include/core/object.h"

#include "../../include/graphics/ui.h"

namespace vxr 
{

	Object::Object()
  {
    static uint32 id = 0;
    id_ = id++;
    name_ = "Object";
	}

	Object::~Object() {}

  void Object::onGUI() 
  {
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll;
    ImGui::InputText(uiText("##Name").c_str(), &name_, flags);
    ImGui::SameLine();
    ImGui::Text("(%d)", id_);
  }

  void Object::set_name(string name)
  {
    name_ = name;
  }

  string Object::name() const
  {
    return name_;
  }

  uint32 Object::id() const
  {
    return id_;
  }

  string Object::uiText(string label)
  {
    string id = std::to_string(id_);
    string separator = "##";
    return (label + separator + id);
  }
	
} /* end of vxr namespace */