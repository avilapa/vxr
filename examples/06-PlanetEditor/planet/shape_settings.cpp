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

#include "shape_settings.h"
#include "../noisefilters/simple.h"
#include "../noisefilters/ridgid.h"

#include "../../../include/graphics/ui.h"

namespace vxr
{

  bool ShapeSettings::equals(ref_ptr<ShapeSettings> o)
  {
    return this->radius == o->radius;
  }

  void ShapeSettings::assign(ref_ptr<ShapeSettings> o)
  {
    this->radius = o->radius;
  }

  void ShapeSettings::NoiseLayer::init(FilterType::Enum type)
  {
    switch (type)
    {
    case FilterType::Simple:
      noiseSettings.allocT<SimpleNoiseSettings>();
      noiseSettings->filterType = FilterType::Simple;
      break;
    case FilterType::Ridgid:
      noiseSettings.allocT<RidgidNoiseSettings>();
      noiseSettings->filterType = FilterType::Ridgid;
      break;
    default:
      break;
    }
  }

  void ShapeGenerator::init(ref_ptr<ShapeSettings> settings)
  {
    this->settings = settings;
    uiSettings.alloc();
  }

  bool ShapeGenerator::hasChanged()
  {
    bool filtersHaveChanged = false;
    for (uint32 i = 0; i < noiseFilters.size(); ++i)
    {
      if (noiseFilters[i]->hasChanged())
      {
        filtersHaveChanged = true;
      }
    }
    if (filtersHaveChanged || !settings->equals(uiSettings))
    {
      settings->assign(uiSettings);
      return true;
    }
    return false;
  }

  void ShapeGenerator::onGUI()
  {
    ImGui::SliderFloat("Radius", &uiSettings->radius, 0.1f, 3.0f);

    ImGui::Spacing();
    ImGui::Text("Noise Layers");
    ImGui::SameLine();
    if (ImGui::SmallButton("+Simple"))
    {
      ref_ptr<ShapeSettings::NoiseLayer> nl;
      nl.alloc()->init(FilterType::Simple);
      settings->noiseLayers.push_back(nl);
      noiseFilters.push_back(NoiseFilter::CreateNoiseFilter(nl->noiseSettings, noiseFilters.size()));
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("+Ridgid"))
    {
      ref_ptr<ShapeSettings::NoiseLayer> nl;
      nl.alloc()->init(FilterType::Ridgid);
      settings->noiseLayers.push_back(nl);
      noiseFilters.push_back(NoiseFilter::CreateNoiseFilter(nl->noiseSettings, noiseFilters.size()));
    }
    for (uint32 i = 0; i < noiseFilters.size(); ++i)
    {
      noiseFilters[i]->onGUI();
    }
  }

  vec3 ShapeGenerator::calculatePointOnPlanet(vec3 pointOnUnitSphere)
  {
    float firstLayerValue = 0;
    float elevation = 0;

    if (noiseFilters.size() > 0)
    {
      firstLayerValue = noiseFilters[0]->evaluate(pointOnUnitSphere);
      if (settings->noiseLayers[0]->noiseSettings->enabled)
      {
        elevation = firstLayerValue;
      }
    }

    for (int i = 1; i < noiseFilters.size(); i++)
    {
      if (settings->noiseLayers[i]->noiseSettings->enabled)
      {
        float mask = (settings->noiseLayers[i]->noiseSettings->useFirstLayerAsMask) ? firstLayerValue : 1;
        elevation += noiseFilters[i]->evaluate(pointOnUnitSphere) * mask;
      }
    }
    elevation = settings->radius * (1 + elevation);
    elevationMinMax.addValue(elevation);
    return pointOnUnitSphere * elevation;
  }

} /* end of vxr namespace */