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

/// WIP: This class is empty!

/**
* \file light.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Light Component and Light System classes.
*
*/
namespace vxr 
{
  namespace System { class Light; }

	class Light : public Component
  {

    VXR_OBJECT(Light, Component);

    friend class System::Light;
	public:
    Light();
		virtual ~Light();

    virtual void onGUI() override;

	private:
	};

  class Scene;

  namespace System 
  {
    class Light : public ComponentSystem
    {

      VXR_OBJECT(System::Light, ComponentSystem);

    public:
      Light();
      virtual ~Light();

      virtual void init() override;
      virtual void update() override;
      virtual void renderUpdate() override;
      virtual void renderPostUpdate() override;

    private:
      std::vector<ref_ptr<vxr::Light>> components_;
      ref_ptr<Scene> scene_;

    public:
      template<typename T> ref_ptr<T> createInstance()
      {
        ref_ptr<T> c;
        c.alloc();
        components_.push_back(c.get());
        return c.get();
      }
    };

    template<> class Getter<vxr::Light>
    {
    public:
      static ref_ptr<System::Light> get();
    };

  }

} /* end of vxr namespace */