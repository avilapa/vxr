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
#include "../../../include/utils/noise.h"

namespace vxr {

  class NoiseSettings;

  struct FilterType
  {
    enum Enum
    {
      Simple,
      Ridgid,
    };
  };

  class NoiseFilter : public Object 
  {
    
    VXR_OBJECT(NoiseFilter, Object);

  public:
    NoiseFilter();
    virtual void init(ref_ptr<NoiseSettings> noiseSettings, uint32 layer_id);
    virtual bool hasChanged();

    virtual float evaluate(vec3 point) = 0;

    static ref_ptr<NoiseFilter> CreateNoiseFilter(ref_ptr<NoiseSettings> settings, uint32 layer_id);

    ref_ptr<NoiseSettings> settings;
    ref_ptr<NoiseSettings> uiSettings;
  protected:
    ref_ptr<Noise> noise;

    uint32 layer_id;

    string label(string label);
  };

  class NoiseSettings : public Object
  {
    VXR_OBJECT(NoiseSettings, Object);

    FilterType::Enum filterType;
    bool useFirstLayerAsMask = true;
    bool enabled = true;

    virtual bool equals(ref_ptr<NoiseSettings> o);
    virtual void assign(ref_ptr<NoiseSettings> o);
  };


} /* end of vxr namespace */