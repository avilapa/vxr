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

#include "../../include/components/transform.h"

#include "../../include/engine/engine.h"
#include "../../include/core/gameobject.h"
#include "../../include/core/scene.h"

namespace vxr 
{

  Transform::Transform()
  {
    set_name("Transform");
    dirty_ = true;
    parent_ = nullptr;

    position_ = vec3(0.0f);
    rotation_ = quat(1.0f, 0.0f, 0.0f, 0.0f);
    scale_ = vec3(1.0f);

    world_position_ = vec3(0.0f);
    world_rotation_ = quat(1.0f, 0.0f, 0.0f, 0.0f);
    world_scale_ = vec3(1.0f);

    world_transform_ = mat4(1.0f);

    euler_angles_ = vec3(0.0f);
  }

  Transform::~Transform()
  {

  }

  void Transform::onGUI()
  {
    ImGui::Spacing();
    ImGui::Text("Position"); ImGui::SameLine();
    if (ImGui::DragFloat3(uiText("##P").c_str(), (float*)&position_, 0.01f, -FLT_MAX, FLT_MAX)) set_local_position(position_);
    /// TODO: Fix the euler angles display and rotation from UI for all cases.
    ImGui::Text("Rotation"); ImGui::SameLine();
    if (ImGui::DragFloat3(uiText("##R").c_str(), (float*)&euler_angles_, 1.0f, -FLT_MAX, FLT_MAX)) set_local_rotation(euler_angles_); else euler_angles_ = local_rotation_angles();
    ImGui::Text("Scale   "); ImGui::SameLine();
    if (ImGui::DragFloat3(uiText("##S").c_str(), (float*)&scale_, 0.01f, -FLT_MAX, FLT_MAX)) set_local_scale(scale_);
  }

  void Transform::set_local_position(const vec3& position)
  {
    position_ = position;
    markForUpdate();
  }

  void Transform::set_local_position_x(const float& v)
  {
    position_.x = v;
    markForUpdate();
  }

  void Transform::set_local_position_y(const float& v)
  {
    position_.y = v;
    markForUpdate();
  }

  void Transform::set_local_position_z(const float& v)
  {
    position_.z = v;
    markForUpdate();
  }

  void Transform::set_local_rotation(const quat& rotation)
  {
    rotation_ = rotation;
    markForUpdate();
  }

  void Transform::set_local_rotation(const vec3& rotation)
  {
    set_local_rotation(quat(glm::radians(rotation)));
  }

  void Transform::set_local_forward(const vec3& direction)
  {
    set_local_rotation(quat(kWorldForward, direction));
  }

  void Transform::set_local_scale(const float& scale)
  {
    set_local_scale(vec3(scale));
  }

  void Transform::set_local_scale(const vec3& scale)
  {
    scale_ = scale;
    markForUpdate();

    if (scale_.x == 0.0f)
    {
      scale_.x = glm::epsilon<float>();
    }
    if (scale_.y == 0.0f)
    {
      scale_.y = glm::epsilon<float>();
    }
    if (scale_.z == 0.0f)
    {
      scale_.z = glm::epsilon<float>();
    }
  }


  void Transform::set_transform(const vec3& position, const quat& rotation, const vec3& scale)
  {
    position_ = position;
    rotation_ = rotation;
    scale_ = scale;
    markForUpdate();
  }

  void Transform::set_transform(const vec3& position, const vec3& rotation, const vec3& scale)
  {
    set_transform(position, quat(glm::radians(rotation)), scale);
  }

  void Transform::set_transform(const vec3& position, const quat& rotation)
  {
    position_ = position;
    rotation_ = rotation;
    markForUpdate();
  }

  void Transform::set_transform(const vec3& position, const vec3& rotation)
  {
    set_transform(position, quat(glm::radians(rotation)));
  }

  void Transform::set_transform(const mat4& matrix)
  {
    vec3 position, scale, skew;
    vec4 perspective;
    quat rotation;
    glm::decompose(matrix, scale, rotation, position, skew, perspective);

    set_transform(position, rotation, scale);
  }

  void Transform::set_world_position(const vec3& world_position)
  {
    if (!parent_)
    {
      set_local_position(world_position);
    }
    else
    {
      set_local_position(glm::inverse(parent_->world_transform()) * vec4(world_position, 0.0f));
    }
  }

  void Transform::set_world_rotation(const quat& world_rotation)
  {
    if (!parent_)
    {
      set_local_rotation(world_rotation);
    }
    else
    {
      set_local_rotation(glm::inverse(parent_->world_rotation()) * world_rotation);
    }
  }

  void Transform::set_world_forward(const vec3& world_direction)
  {
    if (!parent_)
    {
      set_local_rotation(quat(kWorldForward, world_direction));
    }
    else
    {
      set_local_rotation(quat(kWorldForward, glm::inverse(parent_->world_rotation()) * world_direction));
    }
  }

  void Transform::set_world_scale(const float& world_scale)
  {
    set_world_scale(vec3(world_scale));
  }

  void Transform::set_world_scale(const vec3& world_scale)
  {
    if (!parent_)
    {
      set_local_scale(world_scale);
    }
    else
    {
      set_local_scale(world_scale / parent_->world_scale());
    }
  }

  void Transform::set_world_transform(const vec3& position, const quat& rotation, const vec3& scale)
  {
    set_world_position(position);
    set_world_rotation(rotation);
    set_world_scale(scale);
  }

  void Transform::set_world_transform(const vec3& position, const vec3& rotation, const vec3& scale)
  {
    set_world_transform(position, quat(glm::radians(rotation)), scale);
  }

  void Transform::set_world_transform(const vec3& position, const quat& rotation)
  {
    set_world_position(position);
    set_world_rotation(rotation);
  }

  void Transform::set_world_transform(const vec3& position, const vec3& rotation)
  {
    set_world_transform(position, quat(glm::radians(rotation)));
  }

  void Transform::translate(const vec3& delta, TransformSpace::Enum space)
  {
    switch (space)
    {
    case TransformSpace::Local:
      position_ += rotation_ * delta;
      break;
    case TransformSpace::Parent:
      position_ += delta;
      break;
    case TransformSpace::World:
      if (!parent_)
      {
        position_ += delta;
      }
      else
      {
        position_ += vec3(glm::inverse(parent_->world_transform()) * vec4(delta, 0.0f));
      }
      break;
    }
    markForUpdate();
  }

  void Transform::translateX(const float& delta, TransformSpace::Enum space)
  {
    translate(delta * vec3(1.0f, 0.0f, 0.0f), space);
  }

  void Transform::translateY(const float& delta, TransformSpace::Enum space)
  {
    translate(delta * vec3(0.0f, 1.0f, 0.0f), space);
  }

  void Transform::translateZ(const float& delta, TransformSpace::Enum space)
  {
    translate(delta * vec3(0.0f, 0.0f, 1.0f), space);
  }

  void Transform::rotate(const quat& delta, TransformSpace::Enum space)
  {
    switch (space)
    {
    case TransformSpace::Local:
      rotation_ = glm::normalize(rotation_ * delta);
      break;
    case TransformSpace::Parent:
      rotation_ = glm::normalize(delta * rotation_);
      break;
    case TransformSpace::World:
      if (!parent_)
      {
        rotation_ = glm::normalize(delta * rotation_);
      }
      else
      {
        quat world_rotation = parent_->world_rotation();
        rotation_ *= glm::inverse(world_rotation) * delta * world_rotation;
      }
      break;
    }
    markForUpdate();
  }

  void Transform::rotate(const vec3& delta, TransformSpace::Enum space)
  {
    rotate(quat(glm::radians(delta)), space);
  }

  void Transform::rotateX(const float& angle, TransformSpace::Enum space)
  {
    rotate(quat(glm::radians(angle) * vec3(1.0f, 0.0f, 0.0f)), space);
  }

  void Transform::rotateY(const float& angle, TransformSpace::Enum space)
  {
    rotate(quat(glm::radians(angle) * vec3(0.0f, 1.0f, 0.0f)), space);
  }

  void Transform::rotateZ(const float& angle, TransformSpace::Enum space)
  {
    rotate(quat(glm::radians(angle) * vec3(0.0f, 0.0f, 1.0f)), space);
  }

  void Transform::rotateAround(const vec3& point, const quat& delta, TransformSpace::Enum space)
  {
    vec3 parent_space_point;
    quat prev_rotation = rotation_;
    switch (space)
    {
    case TransformSpace::Local:
      parent_space_point = local_transform() * vec4(point, 0.0f);
      rotation_ = glm::normalize(rotation_ * delta);
      break;

    case TransformSpace::Parent:
      parent_space_point = point;
      rotation_ = glm::normalize(delta * rotation_);
      break;

    case TransformSpace::World:
      if (!parent_)
      {
        parent_space_point = point;
        rotation_ = glm::normalize(delta * rotation_);
      }
      else
      {
        parent_space_point = glm::inverse(parent_->world_transform()) * vec4(point, 0.0f);
        quat world_rotation = parent_->world_rotation();
        rotation_ = rotation_ * glm::inverse(world_rotation) * delta * world_rotation;
      }
      break;
    }

    vec3 prev_relative_position = glm::inverse(prev_rotation) * (position_ - parent_space_point);
    position_ = rotation_ * prev_relative_position + parent_space_point;

    markForUpdate();
  }

  void Transform::rotateAround(const vec3& point, const vec3& delta, TransformSpace::Enum space)
  {
    rotateAround(point, quat(glm::radians(delta)), space);
  }

  void Transform::rotateAround(ref_ptr<GameObject> obj, const quat& delta, TransformSpace::Enum space)
  {
    rotateAround(obj->transform()->world_position(), delta, space);
  }

  void Transform::rotateAround(ref_ptr<GameObject> obj, const vec3& delta, TransformSpace::Enum space)
  {
    rotateAround(obj->transform()->world_position(), quat(glm::radians(delta)), space);
  }

  void Transform::lookAt(const vec3& target, const vec3& up, TransformSpace::Enum space)
  {
    vec3 target_world_space;

    switch (space)
    {
    case TransformSpace::Local:
      target_world_space = world_transform() * vec4(target, 0.0f);
      break;
    case TransformSpace::Parent:
      if (!parent_)
      {
        target_world_space = target;
      }
      else
      {
        target_world_space = parent_->world_transform() * vec4(target, 0.0f);
      }
      break;
    case TransformSpace::World:
      target_world_space = target;
      break;
    }

    vec3 look_direction = target_world_space - world_position();
    quat new_rotation;

    //new_rotation.FromLookRotation(look_direction, up)

    set_world_rotation(new_rotation);
  }

  const vec3& Transform::local_position() const
  {
    return position_;
  }

  const quat& Transform::local_rotation() const
  {
    return rotation_;
  }

  vec3 Transform::local_rotation_angles() const
  {
    return glm::degrees(glm::eulerAngles((rotation_)));
  }

  const vec3& Transform::local_scale() const
  {
    return scale_;
  }

  vec3 Transform::local_forward() const
  {
    return rotation_ * kWorldForward;
  }

  vec3 Transform::local_right() const
  {
    return rotation_ * kWorldRight;
  }

  vec3 Transform::local_up() const
  {
    return rotation_ * kWorldUp;
  }

  mat4 Transform::local_transform() const
  {
    return glm::translate(position_) * glm::scale(scale_) * glm::toMat4(rotation_);
  }

  const vec3& Transform::world_position() const
  {
    if (hasChanged())
    {
      updateWorldTransform();
    }

    return world_position_;
  }

  const quat& Transform::world_rotation() const
  {
    if (hasChanged())
    {
      updateWorldTransform();
    }

    return world_rotation_;
  }

  vec3 Transform::world_rotation_angles() const
  {
    const glm::vec3 radians = glm::eulerAngles((world_rotation()));
    return glm::degrees(radians);
  }

  const vec3& Transform::world_scale() const
  {
    if (hasChanged())
    {
      updateWorldTransform();
    }

    return world_scale_;
  }

  vec3 Transform::world_forward() const
  {
    if (hasChanged())
    {
      updateWorldTransform();
    }

    return world_rotation_ * kWorldForward;
  }

  vec3 Transform::world_right() const
  {
    if (hasChanged())
    {
      updateWorldTransform();
    }

    return world_rotation_ * kWorldRight;
  }

  vec3 Transform::world_up() const
  {
    if (hasChanged())
    {
      updateWorldTransform();
    }

    return world_rotation_ * kWorldUp;
  }

  mat4 Transform::world_transform() const
  {
    if (hasChanged())
    {
      updateWorldTransform();
    }

    return world_transform_;
  }

  void Transform::set_parent(ref_ptr<Transform> parent)
  {
    if (!parent)
    {
      return;
    }

    if (parent_ != parent)
    {
      mat4 prev_world_transform = world_transform();
      vec3 position, scale, skew;
      vec4 perspective;
      quat rotation;
      glm::decompose(prev_world_transform, scale, rotation, position, skew, perspective);

      parent_ = parent;
      parent_->children_.push_back(this);

      set_transform(position, rotation, scale);

      gameObject()->scene_id_ = parent->gameObject()->scene_id_;
      for (auto &i : transform()->children_)
      {
        i->gameObject()->scene_id_ = parent->gameObject()->scene_id_;
      }
    }
  }

  ref_ptr<Transform> Transform::parent() const
  {
    return parent_;
  }

  ref_ptr<Transform> Transform::child(uint32 index) const
  {
    if (index > children_.size() - 1)
    {
      VXR_LOG(VXR_DEBUG_LEVEL_WARNING, "[WARNING]: Could not find child with index %d. Returning NULL.\n", index);
      return nullptr;
    }
    return children_[index];
  }

  uint32 Transform::num_children() const
  {
    return children_.size();
  }

  void Transform::updateWorldTransform() const
  {
    VXR_TRACE_SCOPE("VXR", "Compute Transformations");

    mat4 transform = local_transform();

    if (!parent_)
    {
      world_transform_ = transform;
      world_position_ = position_;
      world_rotation_ = rotation_;
      world_scale_ = scale_;
    }
    else
    {
      world_transform_ = parent_->world_transform() * transform;
      world_position_ = world_transform_[3];
      world_rotation_ = parent_->world_rotation() * rotation_;
      world_scale_ = parent_->world_scale() * scale_;
    }

    dirty_ = false;
  }

  void Transform::markForUpdate() const
  {
    if (dirty_)
    {
      return;
    }

    dirty_ = true;
    for (uint32 i = 0; i < children_.size(); ++i)
    {
      children_[i]->markForUpdate();
    }
  }

  bool Transform::hasChanged() const
  {
    return dirty_;
  }

  System::Transform::Transform()
  {
  }

  System::Transform::~Transform()
  {
  }

  void System::Transform::renderUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Transform Render Update");
    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id())
      {
        continue;
      }
      if (c->hasChanged())
      {
        c->updateWorldTransform();
      }
    }

    /// TODO: Remove this and embed rotation in shader
    if (scene_->skybox() != nullptr)
    {
      ref_ptr<vxr::Transform> skybox = scene_->skybox()->getComponent<vxr::Transform>();
      if (skybox->hasChanged())
      {
        skybox->updateWorldTransform();
      }
    }
  }

  ref_ptr<System::Transform> System::Getter<Transform>::get()
  {
    return Engine::ref().transform();
  }
}