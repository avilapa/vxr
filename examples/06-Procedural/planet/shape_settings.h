#pragma once

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

#include "../../../include/core/gameobject.h"
#include "../../../include/components/mesh_filter.h"
#include "../../../include/utils/minmax.h"
#include "../noisefilters/noise_filter.h"

namespace vxr 
{

  class ShapeSettings : public Object
  {
    VXR_OBJECT(ShapeSettings, Object);
  public:

    float radius = 1.0f;

    virtual bool equals(ref_ptr<ShapeSettings> o);
    virtual void assign(ref_ptr<ShapeSettings> o);

    struct NoiseLayer : public Object
    {
      VXR_OBJECT(NoiseLayer, Object);

      void init(FilterType::Enum type);
      ref_ptr<NoiseSettings> noiseSettings;
    };

    std::vector<ref_ptr<NoiseLayer>> noiseLayers;
  };

  class ShapeGenerator : public Object 
  {
    VXR_OBJECT(ShapeGenerator, Object);
  public:
    void init(ref_ptr<ShapeSettings> shapeSettings);

    bool hasChanged();

    virtual void onGUI() override;

    vec3 calculatePointOnPlanet(vec3 pointOnUnitSphere);

    ref_ptr<ShapeSettings> settings;
    ref_ptr<ShapeSettings> uiSettings;

    std::vector<ref_ptr<NoiseFilter>> noiseFilters;

    MinMax elevationMinMax;
  };

} /* end of vxr namespace */