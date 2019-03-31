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

#include "../../include/components/rigidbody.h"
#include "../../include/components/collider.h"
#include "../../include/engine/engine.h"
#include "../../include/graphics/window.h"
#include "../../include/core/gameobject.h"
#include "../../include/core/scene.h"

namespace vxr 
{

  Rigidbody::Rigidbody()
  {
    set_name("Rigidbody");
    set_mass(mass_);
  }

  Rigidbody::~Rigidbody()
  {
  }

  void Rigidbody::onGUI()
  {
    ImGui::Text("Use Gravity "); ImGui::SameLine();
    ImGui::Checkbox(uiText("##UseGravity").c_str(), &use_gravity_);
    ImGui::Text("Mass        "); ImGui::SameLine();
    ImGui::DragFloat(uiText("##Mass").c_str(), (float*)&mass_, 0.01f, -FLT_MAX, FLT_MAX);
    ImGui::Text("Velocity    "); ImGui::SameLine();
    ImGui::DragFloat3(uiText("##Velocity").c_str(), (float*)&velocity_, 0.01f, -FLT_MAX, FLT_MAX);
  }

  void Rigidbody::set_use_gravity(const bool use_gravity)
  {
    use_gravity_ = use_gravity;
  }

  void Rigidbody::set_velocity(const vec3& velocity)
  {
    velocity_ = velocity;
  }

  void Rigidbody::addVelocity(const vec3& velocity)
  {
    velocity_ += velocity;
  }

  void Rigidbody::set_mass(const float mass)
  {
    mass_ = mass;
    inv_mass_ = (mass == 0.0f) ? 0.0f : 1 / mass_;
  }

  void Rigidbody::set_restitution(const float restitution_coeff)
  {
    restitution_ = restitution_coeff;
  }

  const bool Rigidbody::use_gravity() const
  {
    return use_gravity_;
  }

  const vec3 Rigidbody::velocity() const
  {
    return velocity_;
  }

  const float Rigidbody::mass() const
  {
    return mass_;
  }

  const float Rigidbody::inverseMass() const
  {
    return inv_mass_;
  }

  const float Rigidbody::restitution() const
  {
    return restitution_;
  }

  void Rigidbody::applyCentralForce(const vec3& force)
  {
    total_force_ += force * linear_factor_;
  }

  void Rigidbody::applyGravity()
  {
    if (use_gravity_)
    {
      applyCentralForce(Engine::ref().rigidbody()->gravity());
    }
  }

  void Rigidbody::integrateVelocities(float dt)
  {
    velocity_ += total_force_ * inv_mass_ * dt;
    /// Update angular velocity.
  }

  void Rigidbody::applyDamping(float dt)
  {
    velocity_ *= glm::pow(1.0f - Engine::ref().rigidbody()->linearDamping(), dt);
  }

  void Rigidbody::clearForces()
  {
    total_force_ = vec3(0.0f);
  }

  System::Rigidbody::Rigidbody()
  {
  }

  System::Rigidbody::~Rigidbody()
  {
  }

  void System::Rigidbody::preUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Rigidbody Pre Update");

    scene_components_.clear();
    scene_collision_pairs_.clear();
    scene_aabb_.clear();

#ifdef VXR_PHYSICS_NO_BROADPHASE 
    for (uint32 i = 0; i < components_.size(); ++i)
    {
      if (scene_->id() != components_[i]->gameObject()->scene_id() || !components_[i]->gameObject()->active())
      {
        continue;
      }

      scene_components_.push_back(components_[i]);

      ref_ptr<vxr::Rigidbody> rg1 = components_[i];
      ref_ptr<vxr::Collider> col1 = rg1->getComponent<vxr::Collider>();

      if (!col1)
      {
        continue;
      }

      for (uint32 j = i + 1; j < components_.size(); ++j)
      {
        if (scene_->id() != components_[j]->gameObject()->scene_id() || !components_[j]->gameObject()->active())
        {
          continue;
        }

        ref_ptr<vxr::Rigidbody> rg2 = components_[j];
        ref_ptr<vxr::Collider> col2 = rg2->getComponent<vxr::Collider>();
        if (!col2 || col1 == col2)
        {
          continue;
        }

        scene_collision_pairs_.push_back(Hit(col1, col2, rg1, rg2));
      }
    }
#else
    for (uint32 i = 0; i < components_.size(); ++i)
    {
      if (scene_->id() != components_[i]->gameObject()->scene_id() || !components_[i]->gameObject()->active())
      {
        continue;
      }

      scene_components_.push_back(components_[i]);

      ref_ptr<vxr::Rigidbody> rg1 = components_[i];
      ref_ptr<vxr::Collider> col1 = rg1->getComponent<vxr::Collider>();

      if (!col1)
      {
        continue;
      }

      AABB aabb;
      col1->shape()->getAabb(rg1->transform(), aabb.min, aabb.max);
      aabb.scene_index_ = i;
      scene_aabb_.push_back(aabb);
    }

    std::sort(scene_aabb_.begin(), scene_aabb_.end(), [this](const AABB& a, const AABB& b) -> int { return a.min[sorting_axis_] > b.min[sorting_axis_]; });
    
    vec3 s1 = vec3(0.0f);
    vec3 s2 = vec3(0.0f);
    scene_collision_pairs_.clear();
    for (uint32 i = 0; i < scene_aabb_.size(); ++i)
    {
      AABB aabb_i = scene_aabb_[i];
      ref_ptr<vxr::Rigidbody> rg1 = components_[aabb_i.scene_index_];
      ref_ptr<vxr::Collider> col1 = rg1->getComponent<vxr::Collider>();

      if (!col1)
      {
        continue;
      }

      vec3 p = 0.5f * (aabb_i.min + aabb_i.max);

      s1 += p;
      s2 += p * p;

      for (uint32 j = i + 1; j < scene_aabb_.size(); ++j)
      {
        AABB aabb_j = scene_aabb_[j];
        ref_ptr<vxr::Rigidbody> rg2 = components_[aabb_j.scene_index_];
        ref_ptr<vxr::Collider> col2 = rg2->getComponent<vxr::Collider>();
        if (!col2 || col1 == col2)
        {
          continue;
        }

        if (col2->shape()->colType() == ColliderShape::Type::HeightMap || col1->shape()->colType() == ColliderShape::Type::HeightMap)
        {
          scene_collision_pairs_.push_back(Hit(col1, col2, rg1, rg2));
          continue;
        }

        if (aabb_j.min[sorting_axis_] > aabb_i.max[sorting_axis_])
        {
          break;
        }

        if (aabbVsAabb(aabb_i, aabb_j))
        {
          scene_collision_pairs_.push_back(Hit(col1, col2, rg1, rg2));
        }
      }
    }
#endif
  }

  void System::Rigidbody::update(float dt)
  {
    VXR_TRACE_SCOPE("VXR", "Rigidbody Update");
    if (debug_mode_)
    {
      if (Engine::ref().window()->uptime() - debug_seconds_counter_ >= debug_ratio_)
      {
        debug_seconds_counter_ += debug_ratio_;
        dt = debug_step_;
      }
      else
      {
        return;
      }
    }

    for (auto &c : scene_components_)
    {
      c->transform()->translate(c->velocity() * dt);
      c->applyGravity();
      c->integrateVelocities(dt);
      c->applyDamping(dt);
    }

    performCollisionDetection();

    clearForces();
  }

  void System::Rigidbody::set_debug_update(bool enabled, double ratio /* = 1.0 */, double step /* = 1.0 / 60.0 */)
  {
    debug_mode_ = enabled;
    debug_ratio_ = ratio;
    debug_step_ = step;
  }

  const vec3& System::Rigidbody::gravity() const
  {
    return gravity_;
  }

  const float& System::Rigidbody::linearDamping() const
  {
    return damping_linear_;
  }

  void System::Rigidbody::performCollisionDetection()
  {
    VXR_TRACE_SCOPE("VXR", "Perform Collision Detection");
    for (auto &hit : scene_collision_pairs_)
    {

      hit.col1->shape()->checkCollision(hit);

      if (hit.hit)
      {
        resolveImpulse(hit);
        positionCorrection(hit);
      }
    }
  }

  void System::Rigidbody::clearForces()
  {
    for (auto &c : scene_components_)
    {
      c->clearForces();
    }
  }

  void System::Rigidbody::resolveImpulse(Hit& hit)
  {
    vec3 relative_vel = hit.rg2->velocity() - hit.rg1->velocity();
    const float normal_vel = glm::dot(relative_vel, hit.normal);

    // Do not resolve if velocities are separating
    if (normal_vel > 0)
    {
      return;
    }

    const float inv_mass_sum = hit.rg1->inverseMass() + hit.rg2->inverseMass();
    const float e = glm::min(hit.rg1->restitution(), hit.rg2->restitution());

    float j = -(1 + e) * normal_vel;
    j /= inv_mass_sum;

    // Apply impulse
    vec3 impulse = j * hit.normal;
    //const float ratio1 = hit.rg1->mass() * inv_mass_sum;
    //const float ratio2 = hit.rg2->mass() * inv_mass_sum;
    //hit.rg1->addVelocity(-ratio1 * impulse);
    //hit.rg2->addVelocity(+ratio2 * impulse);
    hit.rg1->addVelocity(-hit.rg1->inverseMass() * impulse);
    hit.rg2->addVelocity(+hit.rg2->inverseMass() * impulse);
  }

  void System::Rigidbody::positionCorrection(Hit& hit)
  {
    const float percent = 0.99f; // Penetration percentage to correct.
    const float k_slop = 0.001f; // Penetration allowance.
    const float inv_mass_sum = hit.rg1->inverseMass() + hit.rg2->inverseMass();
    vec3 correction = glm::max(hit.penetration_depth - k_slop, 0.0f) / inv_mass_sum * percent * hit.normal;
    hit.rg1->transform()->translate(-hit.rg1->inverseMass() * correction);
    hit.rg2->transform()->translate(+hit.rg2->inverseMass() * correction);
  }

  bool System::Rigidbody::aabbVsAabb(const AABB& a, const AABB& b)
  {
    if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
    if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
    if (a.max.z < b.min.z || a.min.z > b.max.z) return false;
    return true;
  }

  ref_ptr<System::Rigidbody> System::Getter<Rigidbody>::get()
  {
    return Engine::ref().rigidbody();
  }
}