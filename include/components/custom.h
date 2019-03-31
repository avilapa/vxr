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

#include "../core/component.h"

/**
* \file custom.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Custom Component and Custom System classes. Inherit from this class in order to create a Custom Component. Adding your component to a GameObject requires to use the syntax 'obj->addComponent<YourComponent, Custom>();'.
*
*/
namespace vxr 
{

  namespace System { class Custom; }

	class Custom : public Component
  {
    VXR_OBJECT(Custom, Component);
    friend class System::Custom;
	public:
    Custom();
		virtual ~Custom();

    virtual void init() {}
    virtual void start() {}
    virtual void preUpdate() {}
    virtual void update(float dt) {}
    virtual void postUpdate() {}
    virtual void renderPreUpdate() {}
    virtual void renderUpdate() {}
    virtual void renderPostUpdate() {}
    virtual void stop() {}

    virtual void onGUI() {}

    virtual void onEnable() {}
    virtual void onDisable() {}
    virtual void onSceneChanged() {}

    void set_enabled(const bool enabled) { enabled_ = enabled; }
    bool enabled() const { return enabled_; }

  protected:
    bool enabled_ = true;
    bool initialized_ = false;
	};

  class Scene;

  namespace System 
  {
    class Custom : public ComponentSystem
    {
      VXR_COMPONENT_SYSTEM(Custom, ComponentSystem);
    public:
      Custom();
      virtual ~Custom();

      void init() override;
      void start() override;
      void preUpdate() override;
      void update(float dt) override;
      void postUpdate() override;
      void renderPreUpdate() override;
      void renderUpdate() override;
      void renderPostUpdate() override;
      void stop() override;

    private:
      std::vector<uint32> new_components_;
    };

    template<> class Getter<vxr::Custom>
    {
    public:
      static ref_ptr<System::Custom> get();
    };

  }

} /* end of vxr namespace */