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

    void set_local_position(const vec3& position);
    void set_local_position_x(const float& v);
    void set_local_position_y(const float& v);
    void set_local_position_z(const float& v);
    void set_local_rotation(const quat& rotation);
    void set_local_rotation(const vec3& rotation);
    void set_local_scale(const float& scale);
    void set_local_scale(const vec3& scale);
    void set_local_forward(const vec3& direction);

    void set_transform(const vec3& position, const quat& rotation, const vec3& scale);
    void set_transform(const vec3& position, const vec3& rotation, const vec3& scale);
    void set_transform(const vec3& position, const quat& rotation);
    void set_transform(const vec3& position, const vec3& rotation);
    void set_transform(const mat4& matrix);

    void set_world_position(const vec3& world_position);
    void set_world_rotation(const quat& world_rotation);
    void set_world_scale(const float& world_scale);
    void set_world_scale(const vec3& world_scale);
    void set_world_forward(const vec3& world_direction);

    void set_world_transform(const vec3& position, const quat& rotation, const vec3& scale);
    void set_world_transform(const vec3& position, const vec3& rotation, const vec3& scale);
    void set_world_transform(const vec3& position, const quat& rotation);
    void set_world_transform(const vec3& position, const vec3& rotation);

    void translate(const vec3& delta, TransformSpace::Enum space = TransformSpace::Local);
    void translateX(const float& delta, TransformSpace::Enum space = TransformSpace::Local);
    void translateY(const float& delta, TransformSpace::Enum space = TransformSpace::Local);
    void translateZ(const float& delta, TransformSpace::Enum space = TransformSpace::Local);
    void rotate(const quat& delta, TransformSpace::Enum space = TransformSpace::Local);
    void rotate(const vec3& delta, TransformSpace::Enum space = TransformSpace::Local);
    void rotateX(const float& angle, TransformSpace::Enum space = TransformSpace::Local);
    void rotateY(const float& angle, TransformSpace::Enum space = TransformSpace::Local);
    void rotateZ(const float& angle, TransformSpace::Enum space = TransformSpace::Local);
    void rotateAround(const vec3& point, const quat& delta, TransformSpace::Enum space = TransformSpace::Local);
    void rotateAround(const vec3& point, const vec3& delta, TransformSpace::Enum space = TransformSpace::Local);
    void rotateAround(ref_ptr<GameObject> obj, const quat& delta, TransformSpace::Enum space = TransformSpace::Local);
    void rotateAround(ref_ptr<GameObject> obj, const vec3& delta, TransformSpace::Enum space = TransformSpace::Local);

    /// TODO: Fix lookAt function.
    void lookAt(const vec3& target, const vec3& up = vec3(0.0f, 1.0f, 0.0f), TransformSpace::Enum space = TransformSpace::World);

    const quat& local_rotation() const;
    const vec3& local_position() const;
    vec3 local_rotation_angles() const;
    const vec3& local_scale() const;
    vec3 local_forward() const;
    vec3 local_right() const;
    vec3 local_up() const;

    mat4 local_transform() const;

    const vec3& world_position() const;
    const quat& world_rotation() const;
    vec3 world_rotation_angles() const;
    const vec3& world_scale() const;
    vec3 world_forward() const;
    vec3 world_right() const;
    vec3 world_up() const;

    mat4 world_transform() const;

    /// TODO: Re parenting.
    void set_parent(ref_ptr<Transform> parent);
    ref_ptr<Transform> parent() const;
    ref_ptr<Transform> child(uint32 index = 0) const;
    uint32 num_children() const;

    void updateWorldTransform() const;

    void markForUpdate() const;
    bool hasChanged() const;
	private:
    mutable bool dirty_ = true;

    ref_ptr<Transform> parent_;
    std::vector<ref_ptr<Transform>> children_;

    vec3 position_;
    quat rotation_;
    vec3 scale_;

    mutable vec3 world_position_;
    mutable quat world_rotation_;
    mutable vec3 world_scale_;

    mutable mat4 world_transform_;

    // Euler Angles is used internally just to display quaternions in UI properly.
    vec3 euler_angles_;
	};

  namespace System 
  {
    class Transform : public ComponentSystem
    {
      VXR_COMPONENT_SYSTEM(Transform, ComponentSystem);
    public:
      Transform();
      ~Transform();

      void renderUpdate() override;
    };

    template<> class Getter<vxr::Transform>
    {
    public:
      static ref_ptr<System::Transform> get();
    };

  }

} /* end of vxr namespace */