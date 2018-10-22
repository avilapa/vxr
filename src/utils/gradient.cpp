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

#include "../../include/utils/gradient.h"

#include "../../include/graphics/ui.h"

namespace vxr 
{

  Gradient::Gradient()
  {
    set_name("Gradient");
  }

  Gradient::~Gradient()
  {

  }

  void Gradient::onGUI()
  {
    ImGui::Text("Gradient Settings:");
    for (uint32 i = 0; i < keys_.size(); ++i)
    {
      ImGui::Value("", i);
      ImGui::SameLine();
      ImGui::ColorEdit3(uiText(("##Gradient" + std::to_string(i))).c_str(), (float*)&keys_[i].color);
    }
  }

  void Gradient::addKey(Gradient::Key key)
  {
    key.value = glm::clamp(key.value, 0.0f, 1.0f);
    if (keys_.size() == 0)
    {
      keys_.push_back(key);
      return;
    }
    for (uint32 i = 0; i < keys_.size(); ++i)
    {
      if (key.value == keys_[i].value)
      {
        keys_[i] = key;
        return;
      }
      if (key.value < keys_[i].value)
      {
        keys_.insert(keys_.begin() + i, key);
        return;
      }
    }
    keys_.push_back(key);
  }

  static vec4 vec4_lerp(vec4 a, vec4 b, float t) /// Move to utils
  {
    float ax = a[0],
      ay = a[1],
      az = a[2],
      aw = a[3];
    vec4 out;
    out[0] = ax + t * (b[0] - ax);
    out[1] = ay + t * (b[1] - ay);
    out[2] = az + t * (b[2] - az);
    out[3] = aw + t * (b[3] - aw);
    return out;
  }

  Color Gradient::evaluate(float value)
  {
    value = glm::clamp(value, 0.0f, 1.0f);
    int16 next_index = -1;
    for (uint32 i = 0; i < keys_.size(); ++i)
    {
      if (value == keys_[i].value)
      {
        return keys_[i].color;
      }
      if (value < keys_[i].value)
      {
        next_index = i;
        break;
      }
    }

    if (next_index < 0)
    {
      return Color::Black;
    }

    if (next_index == 0 || next_index == (int16)keys_.size())
    {
      return keys_[next_index].color;
    }

    float frac = (value - keys_[next_index - 1].value) / (keys_[next_index].value - keys_[next_index - 1].value);
    return Color({ vec4_lerp(keys_[next_index - 1].color.rgba(), keys_[next_index].color.rgba(), frac) });
  }

  unsigned char* Gradient::textureData(const int texture_1d_resolution, TexelsFormat::Enum format)/// add formats
  {
    unsigned char* data = (unsigned char*)malloc(3 * texture_1d_resolution * sizeof(unsigned char));
    for (int i = 0; i < texture_1d_resolution; i++)
    {
      vec3 c = evaluate(i / (texture_1d_resolution - 1.0f)).rgb();
      data[i * 3 + 0] = c[0] * 255.0f;
      data[i * 3 + 1] = c[1] * 255.0f;
      data[i * 3 + 2] = c[2] * 255.0f;
    }
    return data;
  }
}