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

#include "color_settings.h"

#include "../../../include/graphics/ui.h"

namespace vxr
{

  bool ColorSettings::equals(ref_ptr<ColorSettings> o)
  {
    return this->wireframe_color == o->wireframe_color;
  }

  void ColorSettings::assign(ref_ptr<ColorSettings> o)
  {
    this->wireframe_color = o->wireframe_color;
  }

  void ColorGenerator::init(ref_ptr<ColorSettings> settings)
  {
    this->settings = settings;
    uiSettings.alloc();

    gradient.addKey({ 0.00f, vec4(65.0f / 255.0f, 105.0f / 255.0f, 255.0f / 255.0f, 1.0f)});
    gradient.addKey({ 0.01f, vec4(30.0f / 255.0f, 144.0f / 255.0f, 255.0f / 255.0f, 1.0f)});
    gradient.addKey({ 0.02f, vec4(240.0f / 255.0f, 230.0f / 255.0f, 140.0f / 255.0f, 1.0f) });
    gradient.addKey({ 0.20f, vec4(34.0f / 255.0f, 139.0f / 255.0f, 34.0f / 255.0f, 1.0f) });
    gradient.addKey({ 0.40f, vec4(160.0f / 255.0f, 82.0f / 255.0f, 45.0f / 255.0f, 1.0f) });
    gradient.addKey({ 0.60f, vec4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f) });
    gradient.addKey({ 1.00f, Color::White });

    gradientTexture.alloc();
    mat.alloc()->addTexture(gradientTexture);

    gradientTexture->set_data(gradient.textureData());
    gradientTexture->set_size(255, 1);
    gradientTexture->set_texels_format(TexelsFormat::RGB_U8);

    wireframe_mat.alloc()->set_render_mode(RenderMode::Wireframe);
  }

  void ColorGenerator::updateColors()
  {
    gradientTexture->set_data(gradient.textureData());
    gradientTexture->set_size(255, 1);
    gradientTexture->set_texels_format(TexelsFormat::RGB_U8);
    mat->set_uniforms(data);

    wireframe_data.u.specific.std.color = settings->wireframe_color.rgba();
    wireframe_mat->set_uniforms(wireframe_data);

    dirty_ = false;
  }

  void ColorGenerator::updateElevation(vec2 elevationMinMax)
  {
    data.u.specific.planet.elevationMinMax = elevationMinMax;
  }

  bool ColorGenerator::hasChanged()
  {
    if (!settings->equals(uiSettings) || dirty_)
    {
      settings->assign(uiSettings);
      return true;
    }
    return false;
  }

  void ColorGenerator::onGUI()
  {
    gradient.onGUI();
    ImGui::Spacing();
    ImGui::Spacing();
    if (ImGui::Button("Regenerate Gradient Texture"))
    {
      dirty_ = true;
    }
  }

} /* end of vxr namespace */