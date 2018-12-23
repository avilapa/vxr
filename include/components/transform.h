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

/// WIP: World position/rotation/scale & forward/right/up are not settable yet.

/**
* \file transform.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Transform Component and Transform System classes. Transform Component contains the matrices for each GameObject as well as children hierarchy information. Transform System performs updates on its components when needed.
*
*/
namespace vxr 
{
  namespace System { class Transform; }

	class Transform : public Component
  {
    VXR_OBJECT(Transform, Component);
	public:
    Transform();
		~Transform();

    virtual void onGUI() override;

#define PROPERTY(type, name, fname, ...) \
     private:\
      type name = __VA_ARGS__;\
     public:\
      void set_##fname(const type &value) { name = value; markForUpdate(); }\
      type fname() const { return name; }

    PROPERTY(vec3, local_position_, local_position, vec3());
    PROPERTY(vec3, local_rotation_, local_rotation, vec3());
    PROPERTY(vec3, local_scale_,    local_scale, vec3());

    PROPERTY(vec3, world_position_, world_position, vec3());
    PROPERTY(vec3, world_rotation_, world_rotation, vec3());
    PROPERTY(vec3, world_scale_,    world_scale,    vec3());

    PROPERTY(vec3, forward_, forward, vec3());
    PROPERTY(vec3, right_,   right,   vec3());
    PROPERTY(vec3, up_,      up,      vec3());
#undef PROPERTY

    mat4 worldMatrix() const;

    void set_parent(ref_ptr<Transform> parent);
    ref_ptr<Transform> parent() const;
    ref_ptr<Transform> child(uint32 index = 0) const;
    uint32 num_children() const;

  public:
    void computeTransformations();
    void computeLocal();
    void computeWorld();

    void markForUpdate();
    bool hasChanged() const;

	private:
    bool dirty_ = true; 

    mat4 local_model_;
    mat4 model_;

    ref_ptr<Transform> parent_;
    std::vector<ref_ptr<Transform>> children_;
	};

  class Scene;

  namespace System 
  {
    class Transform : public ComponentSystem
    {
      VXR_OBJECT(System::Transform, ComponentSystem);
    public:
      Transform();
      ~Transform();

      void init() override;
      void update() override;
      void renderUpdate() override;
      void renderPostUpdate() override;

    private:
      std::vector<ref_ptr<vxr::Transform>> components_;
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

    template<> class Getter<vxr::Transform>
    {
    public:
      static ref_ptr<System::Transform> get();
    };

  }

} /* end of vxr namespace */