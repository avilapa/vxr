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

#include "../../include/components/Custom.h"

#include "../../include/engine/engine.h"

namespace vxr 
{

  Custom::Custom()
  {
    set_name("Custom");
  }

  Custom::~Custom()
  {

  }

  System::Custom::Custom()
  {

  }

  System::Custom::~Custom()
  {

  }

  void System::Custom::start()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
    }
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
      if (c->enabled())
      {
        if (!c->initialized_)
        {
          c->start();
          c->initialized_ = true;
        }
      }
    }
  }

  void System::Custom::update()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
      // Scene changed
    }
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
      if (c->enabled()) 
      {
        if (!c->initialized_)
        {
          c->start();
          c->initialized_ = true;
        }

        c->update();
      }
    }
  }

  void System::Custom::postUpdate()
  {
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
      if (c->enabled()) 
      {
        c->postUpdate();
      }
    }
  }

  void System::Custom::renderUpdate()
  {
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
      if (c->enabled())
      {
        c->renderUpdate();
      }
    }
  }

  void System::Custom::renderPostUpdate()
  {
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
      if (c->enabled())
      {
        c->renderPostUpdate();
      }
    }
  }

  void System::Custom::stop()
  {
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
      if (c->enabled())
      {
        c->stop();
      }
    }
  }

  ref_ptr<System::Custom> System::Getter<Custom>::get()
  {
    return Engine::ref().custom();
  }
}