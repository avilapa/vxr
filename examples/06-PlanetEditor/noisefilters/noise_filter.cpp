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
#include "simple.h"
#include "ridgid.h"

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