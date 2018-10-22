#pragma once

// ----------------------------------------------------------------------------------------
//
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