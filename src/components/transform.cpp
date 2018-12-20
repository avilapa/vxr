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
    world_position_ = { 0.0f, 0.0f, 0.0f };
    world_rotation_ = { 0.0f, 0.0f, 0.0f };
    world_scale_ = { 1.0f, 1.0f, 1.0f };

    local_position_ = { 0.0f, 0.0f, 0.0f };
    local_rotation_ = { 0.0f, 0.0f, 0.0f };
    local_scale_ = { 1.0f, 1.0f, 1.0f };

    forward_ = { 0.0f, 0.0f, 1.0f };
    right_ = { 1.0f, 0.0f, 0.0f };
    up_ = { 0.0f, 1.0f, 0.0f };

    mat4 rot_matrix = glm::rotate(local_rotation_.x, vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(local_rotation_.y, vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(local_rotation_.z, vec3(0.0f, 0.0f, 1.0f));
    local_model_ = glm::translate(local_position_) * glm::scale(local_scale_) * rot_matrix;
    model_ = local_model_;

    parent_ = nullptr;
  }

  Transform::~Transform()
  {

  }

  void Transform::onGUI()
  {
    ImGui::Spacing();
    ImGui::Text("Position"); ImGui::SameLine();
    if (ImGui::DragFloat3(uiText("##Position").c_str(), (float*)&local_position_, 0.01f, -FLT_MAX, FLT_MAX)) markForUpdate();
    ImGui::Text("Rotation"); ImGui::SameLine();
    if (ImGui::DragFloat3(uiText("##Rotation").c_str(), (float*)&local_rotation_, 0.01f, -FLT_MAX, FLT_MAX)) markForUpdate();
    ImGui::Text("Scale   "); ImGui::SameLine();
    if (ImGui::DragFloat3(uiText("##Scale").c_str(), (float*)&local_scale_, 0.01f, -FLT_MAX, FLT_MAX)) markForUpdate();
  }

  mat4 Transform::worldMatrix() const
  {
    mat4 m = local_model_;
    if (parent_.get())
    {
      m = parent_->worldMatrix() * m;
    }
    return m;
  }

  void Transform::set_parent(ref_ptr<Transform> parent)
  {
    if (parent_ != parent)
    {
      /// TODO: Reparenting
      parent_ = parent;
      parent_->children_.push_back(this);
      markForUpdate();
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

  ref_ptr<Transform> Transform::child(uint32 index /* = 0 */) const
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

  void Transform::computeTransformations()
  {
    VXR_TRACE_SCOPE("VXR", "Compute Transformations");

    computeLocal();
    computeWorld();
  }

  void Transform::computeLocal()
  {
    mat4 rot_matrix = glm::rotate(local_rotation_.x, vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(local_rotation_.y, vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(local_rotation_.z, vec3(0.0f, 0.0f, 1.0f));
    local_model_ = glm::translate(local_position_) * glm::scale(local_scale_) * rot_matrix;
    forward_ = glm::normalize(local_model_ * vec4(0.0f, 0.0f, -1.0f, 0.0f));
    right_ = glm::normalize(glm::cross(forward_, vec3(0.0f, 1.0f, 0.0f)));
    up_ = glm::normalize(glm::cross(right_, forward_));
  }

  void Transform::computeWorld()
  {
    vec3 skew; vec4 perspective; glm::quat rotation;
    glm::decompose(worldMatrix(), world_scale_, rotation, world_position_, skew, perspective);
    rotation = glm::conjugate(rotation);
    world_rotation_ = glm::eulerAngles(rotation);
  }

  void Transform::markForUpdate()
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

  void System::Transform::init()
  {

  }

  void System::Transform::update()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
      // Scene changed
    }
  }

  void System::Transform::renderUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Transform Render Update");
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
      if (c->hasChanged())
      {
        c->computeTransformations();
      }
    }

    /// Not needed if rotation is embedded in shader
    if (scene_->skybox() != nullptr)
    {
      ref_ptr<vxr::Transform> skybox = scene_->skybox()->getComponent<vxr::Transform>();
      if (skybox->hasChanged())
      {
        skybox->computeTransformations();
      }
    }
  }

  void System::Transform::renderPostUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Transform Render Post Update");
    if (!scene_)
    {
      return;
    }

    for (auto &c : components_)
    {
      c->dirty_ = false;
    }
  }

  ref_ptr<System::Transform> System::Getter<Transform>::get()
  {
    return Engine::ref().transform();
  }
}