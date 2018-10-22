#pragma once

// ----------------------------------------------------------------------------------------
//
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