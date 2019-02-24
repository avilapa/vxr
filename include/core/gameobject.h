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

#include "../components/transform.h"

/**
* \file gameobject.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief GameObject class is a container of Components and scene information.
*
*/
namespace vxr 
{

	class GameObject : public Object 
  {
    VXR_OBJECT(GameObject, Object);
    friend class Transform;
    friend class Scene;
	public:
		GameObject();
		virtual ~GameObject();

    virtual void onGUI() override;

    ref_ptr<Transform> transform();

    void set_active(bool enabled);

    bool active();

    uint32 scene_id();

  private:
    ref_ptr<Transform> transform_;
    std::vector<ref_ptr<Component>> components_;

    uint32 scene_id_ = 0;
    
    bool active_ = true;

  public:
    std::vector<ref_ptr<Component>> getComponents();
    template<class T> ref_ptr<T> getComponent()
    {
      ref_ptr<T> component;
      for (auto &c : components_)
      {
        if ((component = dynamic_cast<T*>(c.get())) != nullptr)
        {
          return component;
        }
      }
      return nullptr;
    }

    template<class T> ref_ptr<T> getComponentInChildren()
    {
      ref_ptr<T> component;
      for (uint32 i = 0; i < transform_->num_children(); ++i)
      {
        component = transform_->child(i)->getComponent<T>();
        if (component.get())
        {
          return component;
        }
        
        component = transform_->child(i)->gameObject()->getComponentInChildren<T>();
        if (component.get())
        {
          return component;
        }
      }
      return nullptr;
    }

    template<class T, class L = T> ref_ptr<T> addComponent()
    {
      for (auto &c : components_)
      {
        auto other = dynamic_cast<T*>(c.get());
        if (other != nullptr)
        {
          return other;
        }
      }
      ref_ptr<T> component = System::Getter<L>::get()->createInstance<T>();
      component->obj_ = this;
      component->transform_ = transform_;
      components_.push_back(component.get());
      return component;
    }
	};

} /* end of vxr namespace */