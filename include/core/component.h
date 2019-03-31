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

#include "../core/object.h"

/**
* \file component.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Base class for Components and Component Systems.
*
*/
namespace vxr 
{
  class GameObject;
  class Transform;

	class Component : public Object
  {
    VXR_OBJECT(Component, Object);
    friend class GameObject;
	public:
		Component();
		virtual ~Component();

    virtual void onGUI() override;

    virtual ref_ptr<GameObject> gameObject();
    virtual ref_ptr<Transform> transform();

	private:
    ref_ptr<GameObject> obj_;
    ref_ptr<Transform> transform_;

  public:
    template<class T> ref_ptr<T> getComponent()
    {
      ref_ptr<T> component;
      for (auto &c : obj_->getComponents())
      {
        if ((component = dynamic_cast<T*>(c.get())) != nullptr)
        {
          return component;
        }
      }
      return nullptr;
    }
	};

  class Scene;

  namespace System 
  {
    class ComponentSystem : public Object
    {
      VXR_OBJECT(ComponentSystem, Object);
    public:
      ComponentSystem();
      virtual ~ComponentSystem();

      virtual void init() {};
      virtual void start() {};
      virtual void preUpdate() {};
      virtual void update(float dt) {};
      virtual void postUpdate() {};
      virtual void renderPreUpdate() {};
      virtual void renderUpdate() {};
      virtual void renderPostUpdate() {};
      virtual void stop() {};

      virtual void onSceneChanged();

    protected:
      ref_ptr<Scene> scene_;
    };

    template<class T> class Getter
    {
    };
  }

#define VXR_COMPONENT_SYSTEM(type_name, base_type_name)       \
  VXR_OBJECT(System::##type_name, System::##base_type_name);  \
  template<typename T> ref_ptr<T> createInstance()            \
  {                                                           \
    ref_ptr<T> c;                                             \
    c.alloc();                                                \
    components_.push_back(c.get());                           \
    return c.get();                                           \
  }                                                           \
 private:                                                     \
  std::vector<ref_ptr<vxr::##type_name>> components_;

} /* end of vxr namespace */