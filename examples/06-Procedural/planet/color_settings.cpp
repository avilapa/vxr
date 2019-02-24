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

namespace vxr
{

  ColorSettings::ColorSettings()
  {
    ref_ptr<BiomeColorSettings::Biome> b;
    b.alloc()->biome_id = 0;
    b->gradient.addKey({ 0.00f, vec4(65.0f / 255.0f, 105.0f / 255.0f, 255.0f / 255.0f, 1.0f) });
    b->gradient.addKey({ 0.01f, vec4(30.0f / 255.0f, 144.0f / 255.0f, 255.0f / 255.0f, 1.0f) });
    b->gradient.addKey({ 0.02f, vec4(240.0f / 255.0f, 230.0f / 255.0f, 140.0f / 255.0f, 1.0f) });
    b->gradient.addKey({ 0.20f, vec4(34.0f / 255.0f, 139.0f / 255.0f, 34.0f / 255.0f, 1.0f) });
    b->gradient.addKey({ 0.40f, vec4(160.0f / 255.0f, 82.0f / 255.0f, 45.0f / 255.0f, 1.0f) });
    b->gradient.addKey({ 0.60f, vec4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f) });
    b->gradient.addKey({ 1.00f, Color::White });
    biome_color_settings.alloc()->biomes.push_back(b);

    biome_color_settings->noise.allocT<SimpleNoiseSettings>()->filterType = FilterType::Simple;
  }

  void ColorSettings::BiomeColorSettings::Biome::onGUI()
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
      | ImGuiTreeNodeFlags_Selected
      | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    std::string biome = "Biome ";
    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)biome_id, node_flags, "%s", (biome + std::to_string(biome_id)).c_str());

    if (node_open)
    {
      gradient.onGUI();
      ImGui::Spacing();
      ImGui::DragFloat(uiText("Start Height").c_str(), &start_height, 0.01f, 0.0f, 1.0f);
      ImGui::Spacing();
      ImGui::ColorEdit3("Tint", (float*)&tint);
      ImGui::Spacing();
      ImGui::DragFloat(uiText("Tint Percent").c_str(), &tint_percent, 0.01f, 0.0f, 1.0f);
      
      ImGui::TreePop();
    }
    ImGui::Spacing();
  }

  bool ColorSettings::BiomeColorSettings::Biome::equals(ref_ptr<Biome> o)
  {
    return this->start_height == o->start_height
      && this->gradient == o->gradient
      && this->tint == o->tint
      && this->tint_percent == o->tint_percent
      && this->biome_id == o->biome_id;
  }

  void ColorSettings::BiomeColorSettings::Biome::assign(ref_ptr<Biome> o)
  {
    this->gradient = o->gradient;
    this->start_height = o->start_height;
    this->tint = o->tint;
    this->tint_percent = o->tint_percent;
    this->biome_id = o->biome_id;
  }

  bool ColorSettings::equals(ref_ptr<ColorSettings> o)
  {
    if (biome_color_settings->biomes.size() != o->biome_color_settings->biomes.size())
    {
      return false;
    }
    if (biome_color_settings->noise_offset != o->biome_color_settings->noise_offset || biome_color_settings->noise_strength != o->biome_color_settings->noise_strength || biome_color_settings->blend_amount != o->biome_color_settings->blend_amount)
    {
      return false;
    }
    for (uint32 i = 0; i < o->biome_color_settings->biomes.size(); ++i)
    {
      if (!biome_color_settings->biomes[i]->equals(o->biome_color_settings->biomes[i]))
      {
        return false;
      }
    }
    return this->wireframe_color == o->wireframe_color;
  }

  void ColorSettings::assign(ref_ptr<ColorSettings> o)
  {
    this->wireframe_color = o->wireframe_color;
    this->biome_color_settings->noise_offset = o->biome_color_settings->noise_offset;
    this->biome_color_settings->noise_strength = o->biome_color_settings->noise_strength;
    this->biome_color_settings->blend_amount = o->biome_color_settings->blend_amount;
    this->biome_color_settings->biomes.clear();
    for (uint32 i = 0; i < o->biome_color_settings->biomes.size(); ++i)
    {
      ref_ptr<BiomeColorSettings::Biome> b;
      b.alloc()->assign(o->biome_color_settings->biomes[i]);
      this->biome_color_settings->biomes.push_back(b);
    }
  }

  void ColorGenerator::init(ref_ptr<ColorSettings> settings)
  {
    this->settings = settings;
    uiSettings.alloc();

    gradient_texture.alloc();
    mat.alloc()->set_gradient_texture(gradient_texture);
    wireframe_mat.alloc();

    biome_noise_filter = NoiseFilter::CreateNoiseFilter(settings->biome_color_settings->noise.get(), 0);

    updateColors();
  }

  void ColorGenerator::updateColors()
  {
    const int texture_resolution = 255;
    unsigned char* color_data = (unsigned char*)malloc(3 * texture_resolution * settings->biome_color_settings->biomes.size() * sizeof(unsigned char));

    for (uint32 i = 0; i < settings->biome_color_settings->biomes.size(); ++i)
    {
      auto biome = settings->biome_color_settings->biomes[i];
      unsigned char* gradient_data = biome->gradient.textureData(texture_resolution);
      
      for (uint32 j = 0; j < texture_resolution * 3; j+=3)
      {
        unsigned int index = i * texture_resolution * 3 + j;
        Color gradient_color = Color(gradient_data[j + 0], gradient_data[j + 1], gradient_data[j + 2], 1.0f);
        gradient_color = gradient_color * (1 - biome->tint_percent) + biome->tint * 255.0f * biome->tint_percent;
        color_data[index + 0] = gradient_color.r;
        color_data[index + 1] = gradient_color.g;
        color_data[index + 2] = gradient_color.b;
      }
      free(gradient_data);
    }
    gradient_texture->set_data(color_data);
    gradient_texture->set_size(texture_resolution, (uint16)settings->biome_color_settings->biomes.size());
    gradient_texture->set_texels_format(TexelsFormat::RGB_U8);
    gradient_texture->set_filtering(SamplerFiltering::Nearest, SamplerFiltering::Nearest);
    gradient_texture->set_usage(Usage::Dynamic);

    wireframe_mat->set_color(settings->wireframe_color.rgba());
  }

  void ColorGenerator::updateElevation(vec2 elevationMinMax)
  {
    mat->set_elevation_min_max(elevationMinMax);
  }

  float ColorGenerator::percentFromPoint(vec3 point)
  {
    float height_percent = (point.y + 1.0f) * 0.5f;
    height_percent += (biome_noise_filter->evaluate(point) - settings->biome_color_settings->noise_offset) * settings->biome_color_settings->noise_strength;
    float biome_index = 0;
    float blendRange = settings->biome_color_settings->blend_amount * 0.5f + 0.001f;

    for (uint32 i = 0; i < settings->biome_color_settings->biomes.size(); ++i)
    {
      /// TODO: Biome blending Issue
      /*float distance = height_percent - settings->biome_color_settings->biomes[i]->start_height;
      float weight = inverseLerp(-blendRange, blendRange, distance);
      biome_index *= (1 - weight);
      biome_index += (float)i * weight;*/

      if (settings->biome_color_settings->biomes[i]->start_height < height_percent)
      {
        biome_index = i;
      }
      else
      {
        break;
      }
    }

    return biome_index / glm::max(1.0f, settings->biome_color_settings->biomes.size() - 1.0f);
  }

  bool ColorGenerator::hasChanged()
  {
    if (!settings->equals(uiSettings) || biome_noise_filter->hasChanged())
    {
      settings->assign(uiSettings);
      return true;
    }
    return false;
  }

  void ColorGenerator::onGUI()
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
      | ImGuiTreeNodeFlags_Selected
      | ImGuiTreeNodeFlags_OpenOnDoubleClick
      | ImGuiTreeNodeFlags_DefaultOpen;

    ImGui::Spacing();
    if (ImGui::TreeNodeEx(uiText("Color Settings").c_str(), node_flags))
    {
      ImGui::Spacing();
      ImGui::Text("Biome");
      /// TODO: Biome blending Issue
      /*ImGui::SameLine();
      if (ImGui::SmallButton(uiText("+ New").c_str()))
      {
        ref_ptr<ColorSettings::BiomeColorSettings::Biome> b;
        b.alloc();
        b->biome_id = uiSettings->biome_color_settings->biomes.size();
        uiSettings->biome_color_settings->biomes.push_back(b);
      }
      ImGui::Spacing();
      for (uint32 i = 0; i < uiSettings->biome_color_settings->biomes.size(); ++i)
      {
        uiSettings->biome_color_settings->biomes[i]->onGUI();
      }
      ImGui::Spacing();
      if (ImGui::TreeNode(uiText("Biome Noise Settings").c_str()))
      {
        ImGui::Spacing();
        ImGui::DragFloat(uiText("Noise Offset").c_str(), &uiSettings->biome_color_settings->noise_offset, 0.01f);
        ImGui::DragFloat(uiText("Noise Strength").c_str(), &uiSettings->biome_color_settings->noise_strength, 0.01f);
        ImGui::DragFloat(uiText("Blend Amount").c_str(), &uiSettings->biome_color_settings->blend_amount, 0.01f, 0.00001f, 1.0f);
        biome_noise_filter->onGUI();
        ImGui::TreePop();
      }*/
      uiSettings->biome_color_settings->biomes[0]->onGUI();
      ImGui::TreePop();
    }
  }

} /* end of vxr namespace */