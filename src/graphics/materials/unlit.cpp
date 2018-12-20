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

#include "../../../include/graphics/materials/unlit.h"

#include "../../../include/engine/engine.h"

namespace vxr
{

  namespace mat
  {

    Unlit::Unlit()
    {
      set_name("Unlit/Unlit");
      set_shaders("unlit.vert", "unlit.frag");

      set_num_textures(0);
      set_uniforms_enabled(true);
      set_uniforms_name("Unlit");
    }

    Unlit::Instance::Instance()
    {
      init("Unlit/Unlit");
    }

    void Unlit::Instance::set_color(Color color)
    {
      uniforms_.unlit.color = color.rgba();
    }

    Color Unlit::Instance::color() const
    {
      return Color(uniforms_.unlit.color);
    }

    void Unlit::Instance::onGUI()
    {
      MaterialInstance::onGUI();
      ImGui::ColorEdit4("Color", (float*)&uniforms_.unlit.color);
    }

    Wireframe::Wireframe()
    {
      set_name("Unlit/Wireframe");
      set_shaders("unlit.vert", "unlit.frag");

      set_num_textures(0);
      set_uniforms_enabled(true);
      set_uniforms_name("Unlit");
      set_render_mode(RenderMode::Wireframe);
    }

    Wireframe::Instance::Instance()
    {
      init("Unlit/Wireframe");
    }

    void Wireframe::Instance::set_color(Color color)
    {
      uniforms_.unlit.color = color.rgba();
    }

    Color Wireframe::Instance::color() const
    {
      return Color(uniforms_.unlit.color);
    }

    void Wireframe::Instance::onGUI()
    {
      MaterialInstance::onGUI();
      ImGui::ColorEdit4("Color", (float*)&uniforms_.unlit.color);
    }

  }

}