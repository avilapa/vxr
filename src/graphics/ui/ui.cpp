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

#include "../../../include/graphics/ui/ui.h"
#include "../../../include/engine/engine.h"

#if defined (VXR_OPENGL)
#  include "../backend/opengl/gl_imgui.h"
#elif defined (VXR_DX11)
#  include "../backend/dx11/dx11_imgui.h"
#else
#  error Backend must be defined on GENie.lua (e.g. try parameters --gl OR --dx11).
#endif

namespace vxr
{

  bool ui::Init(Window::Data* data)
  {
    VXR_TRACE_SCOPE("VXR", "UI Init");
    ui::impl::Init(data);
    return true;
  }

  void ui::Update(Window::Data* data)
  {
    VXR_TRACE_SCOPE("VXR", "UI Update");
    ui::impl::Update(data);
    ImGui::NewFrame();
  }

  void ui::Draw()
  {
    VXR_TRACE_SCOPE("VXR", "UI Draw");
    ImGui::Render();
    ui::impl::Draw(ImGui::GetDrawData());
  }

  void ui::Stop(Window::Data* data)
  {
    VXR_TRACE_SCOPE("VXR", "UI Stop");
    ui::impl::Stop(data);
    ImGui::DestroyContext();
  }

} /* end of vxr namespace */