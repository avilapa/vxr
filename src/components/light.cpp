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

#include "../../include/components/light.h"

#include "../../include/engine/engine.h"
#include "../../include/core/gameobject.h"
#include "../../include/graphics/ui.h"

namespace vxr 
{

  Light::Light()
  {
    set_name("Light");
  }

  Light::~Light()
  {

  }

  void Light::onGUI()
  {
    ImGui::Spacing();
  }

  System::Light::Light()
  {

  }

  System::Light::~Light()
  {

  }

  void System::Light::init()
  {

  }

  void System::Light::update()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
      // Scene changed
    }
  }

  void System::Light::renderUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Light Render Update");
    if (!scene_)
    {
      return;
    }

    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id())
      {
        continue;
      }
      /*if (c->hasChanged())
      {
        c->computeTransformations();
      }
      ///c->gameObject()->set_active(false);*/
    }
  }

  void System::Light::renderPostUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Light Render Post Update");
    if (!scene_)
    {
      return;
    }

    for (auto &c : components_)
    {
      //c->dirty_ = false;
    }
  }

  ref_ptr<System::Light> System::Getter<Light>::get()
  {
    return Engine::ref().light();
  }
}