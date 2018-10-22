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

#include "../../include/components/mesh_filter.h"

#include "../../include/engine/engine.h"
#include "../../include/graphics/ui.h"

namespace vxr 
{

  MeshFilter::MeshFilter()
  {
    set_name("Mesh Filter");
  }

  MeshFilter::~MeshFilter()
  {

  }

  void MeshFilter::onGUI()
  {
    if (ImGui::TreeNodeEx((void*)(intptr_t)id(), 0, "Mesh"))
    {
      ImGui::Spacing();
      mesh->onGUI();
      ImGui::TreePop();
    }
  }

  System::MeshFilter::MeshFilter()
  {

  }

  System::MeshFilter::~MeshFilter()
  {

  }

  void System::MeshFilter::init()
  {

  }

  ref_ptr<System::MeshFilter> System::Getter<MeshFilter>::get()
  {
    return Engine::ref().meshFilter();
  }
}