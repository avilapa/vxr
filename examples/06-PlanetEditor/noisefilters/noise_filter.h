#pragma once

// ----------------------------------------------------------------------------------------
//
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