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

#include "../../include/graphics/default_materials.h"

#include "../../include/graphics/ui.h"

namespace vxr
{

  Screen::Screen()
  {
    set_shaders("screen_standard.vert", "screen_standard.frag");
    set_uniforms_name("Screen");
  }

  void Screen::onGUI()
  {
    Material::onGUI();
  }

#define UNIFORM_ACCESS uniforms_.u.specific.unlit

  Unlit::Unlit()
  {
    set_shaders("unlit.vert", "unlit.frag");
    set_uniforms_name("Unlit");

    UNIFORM_ACCESS.color = vec4(1);
  }

  void Unlit::onGUI()
  {
    Material::onGUI();
    ImGui::ColorEdit4("Color", (float*)&UNIFORM_ACCESS.color);
  }

  void Unlit::set_color(Color color)
  {
    UNIFORM_ACCESS.color = color.rgba();
  }

  Color Unlit::color() const
  {
    return Color(UNIFORM_ACCESS.color);
  }

#undef UNIFORM_ACCESS

#define UNIFORM_ACCESS uniforms_.u.specific.standard

  Standard::Standard()
  {
    set_shaders("standard.vert", "standard.frag");
    set_uniforms_name("Standard");

    UNIFORM_ACCESS.color = vec4(1);
  }

  void Standard::onGUI()
  {
    Material::onGUI();
    ImGui::ColorEdit4("Color", (float*)&UNIFORM_ACCESS.color);
  }

  void Standard::set_color(Color color)
  {
    UNIFORM_ACCESS.color = color.rgba();
  }

  Color Standard::color() const
  {
    return Color(UNIFORM_ACCESS.color);
  }

#undef UNIFORM_ACCESS
}