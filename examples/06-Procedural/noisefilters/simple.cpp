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

#include "simple.h"

#include <algorithm>

namespace vxr
{
  void SimpleNoiseFilter::init(ref_ptr<NoiseSettings> noiseSettings, uint32 layer_id)
  {
    NoiseFilter::init(noiseSettings, layer_id);
    s_ = (SimpleNoiseSettings*)settings.get();
    ui_s_ = (SimpleNoiseSettings*)uiSettings.get();
  }

  float SimpleNoiseFilter::evaluate(vec3 point)
  {
    float noiseValue = 0;
    float frequency = s_->baseRoughness;
    float amplitude = 1;

    for (uint32 i = 0; i < s_->numLayers; i++)
    {
      float v = noise->evaluate(point * frequency + s_->center);
      noiseValue += (v + 1) * .5f * amplitude;
      frequency *= s_->roughness;
      amplitude *= s_->persistence;
    }

    noiseValue = std::max(0.0f, noiseValue - s_->minValue);
    return noiseValue * s_->strength;
  }

  void SimpleNoiseFilter::onGUI()
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
      | ImGuiTreeNodeFlags_Selected
      | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    std::string layer = "Layer ";
    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)layer_id, node_flags, "%s", (layer + std::to_string(layer_id)).c_str());

    if (node_open)
    {
      ImGui::Checkbox(label("Enabled").c_str(), &ui_s_->enabled);
      if (layer_id != 0)
      {
        ImGui::SameLine();
        ImGui::Checkbox(label("Use mask (1st layer)").c_str(), &ui_s_->useFirstLayerAsMask);
      }
      ImGui::DragFloat(label("Strength").c_str(), &ui_s_->strength, 0.01f, 0.01f, 5.0f);
      ImGui::SliderInt(label("Layers").c_str(), (int*)&ui_s_->numLayers, 1, 8);
      ImGui::DragFloat(label("Base Roughness").c_str(), &ui_s_->baseRoughness, 0.01f, 0.01f, 10.0f);
      ImGui::DragFloat(label("Roughness").c_str(), &ui_s_->roughness, 0.01f, 0.01f, 10.0f);
      ImGui::DragFloat(label("Persistence").c_str(), &ui_s_->persistence, 0.01f, 0.01f, 10.0f);
      ImGui::DragFloat3(label("Noise Center").c_str(), (float*)&ui_s_->center, 0.01f, -100.0f, 100.0f);
      ImGui::DragFloat(label("Min Value").c_str(), &ui_s_->minValue, 0.01f, 0.01f, 10.0f);
      ImGui::TreePop();
    }
    ImGui::Spacing();
  }

  bool SimpleNoiseSettings::equals(ref_ptr<NoiseSettings> o)
  {
    ref_ptr<SimpleNoiseSettings> other = (SimpleNoiseSettings*)o.get();
    if (o != nullptr)
    {
      return NoiseSettings::equals(o)
        && this->strength == other->strength
        && this->numLayers == other->numLayers
        && this->baseRoughness == other->baseRoughness
        && this->roughness == other->roughness
        && this->persistence == other->persistence
        && this->center == other->center
        && this->minValue == other->minValue;
    }
    return false;
  }

  void SimpleNoiseSettings::assign(ref_ptr<NoiseSettings> o)
  {
    ref_ptr<SimpleNoiseSettings> other = (SimpleNoiseSettings*)o.get();
    NoiseSettings::assign(o);
    this->strength = other->strength;
    this->numLayers = other->numLayers;
    this->baseRoughness = other->baseRoughness;
    this->roughness = other->roughness;
    this->persistence = other->persistence;
    this->center = other->center;
    this->minValue = other->minValue;
  }

} /* end of vxr namespace */