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

#include "../../include/physics/hit.h"
#include "../../include/core/component.h"
#include "../../include/graphics/materials/material_instance.h"

/**
* \file rigidbody.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief 
*
*/
namespace vxr 
{
  
	class Rigidbody : public Component
  {
    VXR_OBJECT(Rigidbody, Component);
	public:
    Rigidbody();
		~Rigidbody();

    virtual void onGUI() override;

    void set_use_gravity(const bool use_gravity);
    void set_velocity(const vec3& velocity);
    void addVelocity(const vec3& velocity);
    void set_mass(const float mass);
    void set_restitution(const float restitution_coeff);

    const bool use_gravity() const;
    const vec3 velocity() const;
    const float mass() const;
    const float inverseMass() const;
    const float restitution() const;

    void applyCentralForce(const vec3& force);

    void applyGravity();
    void integrateVelocities(float dt);
    void applyDamping(float dt);

    void clearForces();

  private:
    bool use_gravity_ = true;

    vec3 velocity_ = vec3(0.0f);
    vec3 linear_factor_ = vec3(1.0f);

    float mass_ = 1.0f; 
    float inv_mass_;
    float restitution_ = 0.5f; // Elasticity/Bounciness

    vec3 total_force_ = vec3(0.0f);
	};

  class Collider;

  namespace System 
  {
    class Rigidbody : public ComponentSystem
    {
      VXR_COMPONENT_SYSTEM(Rigidbody, ComponentSystem);
    public:
      Rigidbody();
      ~Rigidbody();

      void preUpdate() override;
      void update(float dt) override;

      void set_debug_update(bool enabled, double ratio = 1.0, double step = 1.0 / 60.0);

      const vec3& gravity() const;
      const float& linearDamping() const;

    private:
      void performCollisionDetection();
      void clearForces();

      void resolveImpulse(Hit& hit);
      void positionCorrection(Hit& hit);

    private:
      struct AABB 
      {
        vec3 min, max; 
        uint32 scene_index_; 
      };

      bool System::Rigidbody::aabbVsAabb(const AABB& a, const AABB& b);
      uint32 sorting_axis_ = 0;

      std::vector<ref_ptr<vxr::Rigidbody>> scene_components_;
      std::vector<Hit> scene_collision_pairs_;
      std::vector<AABB> scene_aabb_;

      vec3 gravity_ = vec3(0.0f, -9.8f, 0.0f);
      float damping_linear_ = 0.01f;

      // Debug
      bool debug_mode_ = false;
      double debug_ratio_ = 1.0;
      double debug_step_ = 1.0 / 60.0;
      double debug_seconds_counter_ = 0;
    };

    template<> class Getter<vxr::Rigidbody>
    {
    public:
      static ref_ptr<System::Rigidbody> get();
    };

  }



} /* end of vxr namespace */