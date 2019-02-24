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

#include "simple.h"

namespace vxr {

  class RidgidNoiseSettings;

  class RidgidNoiseFilter : public NoiseFilter
  {
    VXR_OBJECT(RidgidNoiseFilter, NoiseFilter);
  public:
    virtual void init(ref_ptr<NoiseSettings> noiseSettings, uint32 layer_id) override;
    virtual float evaluate(vec3 point) override;

    virtual void onGUI() override;

  private:
    ref_ptr<RidgidNoiseSettings> s_;
    ref_ptr<RidgidNoiseSettings> ui_s_;
  };

  class RidgidNoiseSettings : public SimpleNoiseSettings
  {
    VXR_OBJECT(RidgidNoiseSettings, NoiseSettings);
  public:

    float weightMultiplier = 0.8f;

    virtual bool equals(ref_ptr<NoiseSettings> o);
    virtual void assign(ref_ptr<NoiseSettings> o);
  };

} /* end of vxr namespace */