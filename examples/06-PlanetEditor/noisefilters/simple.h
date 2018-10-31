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

#include "noise_filter.h"

namespace vxr {

  class SimpleNoiseSettings;

  class SimpleNoiseFilter : public NoiseFilter
  {
    VXR_OBJECT(SimpleNoiseFilter, NoiseFilter);
  public:
    virtual void init(ref_ptr<NoiseSettings> noiseSettings, uint32 layer_id) override;
    virtual float evaluate(vec3 point) override;

    virtual void onGUI() override;

  private:
    ref_ptr<SimpleNoiseSettings> s_;
    ref_ptr<SimpleNoiseSettings> ui_s_;
  };

  class SimpleNoiseSettings : public NoiseSettings
  {
    VXR_OBJECT(SimpleNoiseSettings, NoiseSettings);
  public:

    float strength = 1;
    uint32 numLayers = 1;
    float baseRoughness = 1;
    float roughness = 2;
    float persistence = .5f;
    vec3 center = vec3();
    float minValue = 1;

    virtual bool equals(ref_ptr<NoiseSettings> o);
    virtual void assign(ref_ptr<NoiseSettings> o);
  };

} /* end of vxr namespace */