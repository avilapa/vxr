#include "noise_filter.h"
#include "simple.h"
#include "ridgid.h"

#include "../../../include/graphics/ui.h"

namespace vxr
{

  NoiseFilter::NoiseFilter()
  {
    noise.alloc();
  }

  void NoiseFilter::init(ref_ptr<NoiseSettings> settings, uint32 layer_id)
  {
    noise->init();
    this->settings = settings;
    this->layer_id = layer_id;
  }

  bool NoiseFilter::hasChanged()
  {
    if (!settings->equals(uiSettings))
    {
      settings->assign(uiSettings);
      return true;
    }
    return false;
  }

  std::string NoiseFilter::label(string label)
  {
    string id = std::to_string(layer_id);
    string separator = "##";
    return (label + separator + id);
  }

  ref_ptr<NoiseFilter> NoiseFilter::CreateNoiseFilter(ref_ptr<NoiseSettings> settings, uint32 layer_id)
  {
    switch (settings->filterType)
    {
    case FilterType::Simple: 
    {
      ref_ptr<SimpleNoiseFilter> f;
      f.alloc()->uiSettings.allocT<SimpleNoiseSettings>()->filterType = FilterType::Simple;
      f->init(settings, layer_id);
      return f.get();
    }
    case FilterType::Ridgid: 
    {
      ref_ptr<RidgidNoiseFilter> f;
      f.alloc()->uiSettings.allocT<RidgidNoiseSettings>()->filterType = FilterType::Ridgid;
      f->init(settings, layer_id);
      return f.get();
    }
    default:
      return nullptr;
    }
  }

  bool NoiseSettings::equals(ref_ptr<NoiseSettings> o)
  {
    return this->filterType == o->filterType
      && this->useFirstLayerAsMask == o->useFirstLayerAsMask
      && this->enabled == o->enabled;
  }

  void NoiseSettings::assign(ref_ptr<NoiseSettings> o)
  {
    this->filterType = o->filterType;
    this->useFirstLayerAsMask = o->useFirstLayerAsMask;
    this->enabled = o->enabled;
  }

} /* end of vxr namespace */