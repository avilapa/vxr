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

#include "../../include/components/custom.h"

#include "../../include/engine/engine.h"
#include "../../include/core/scene.h"

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

  void System::Custom::init()
  {
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
          c->init();
          c->initialized_ = true;
        }
      }
    }
  }

  void System::Custom::start()
  {
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
          c->init();
          c->initialized_ = true;
        }
        c->start();
      }
    }
  }

  void System::Custom::preUpdate()
  {
    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id())
      {
        continue;
      }
      if (c->enabled())
      {
        c->preUpdate();
      }
    }
  }

  void System::Custom::update(float dt)
  {
    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id())
      {
        continue;
      }
      if (c->enabled()) 
      {
        c->update(dt);
      }
    }
  }

  void System::Custom::postUpdate()
  {
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

  void System::Custom::renderPreUpdate()
  {
    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id())
      {
        continue;
      }
      if (c->enabled())
      {
        c->renderPreUpdate();
      }
    }
  }

  void System::Custom::renderUpdate()
  {
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
    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id())
      {
        continue;
      }
      if (c->enabled())
      {
        if (c->initialized_)
        {
          c->stop();
        }
      }
    }
  }

  ref_ptr<System::Custom> System::Getter<Custom>::get()
  {
    return Engine::ref().custom();
  }
}