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

#include "../../include/physics/collider_sphere.h"
#include "../../include/physics/collider_height_map.h"
#include "../../include/graphics/height_map.h"
#include "../../include/components/collider.h"
#include "../../include/components/rigidbody.h"
#include "../../include/components/transform.h"

namespace vxr 
{

  ColliderSphere::ColliderSphere()
  {
    set_name("Collider Sphere");
    type_ = Type::Sphere;
  }

  ColliderSphere::~ColliderSphere()
  {
  }

  void ColliderSphere::getAabb(ref_ptr<Transform> transform, vec3& aabb_min, vec3& aabb_max) const
  {
    aabb_min = transform->local_position() - vec3(radius_) - 0.5f;
    aabb_max = transform->local_position() + vec3(radius_) + 0.5f;
  }

  void ColliderSphere::checkCollision(Hit& hit) const
  {
    switch (hit.col2->shape()->colType())
    {
    case ColliderShape::Type::Sphere:
    {
      //VXR_TRACE_SCOPE("VXR", "Sphere VS Sphere");
      const vec3 posA = hit.col1->transform()->local_position();
      const vec3 posB = hit.col2->transform()->local_position();
      const float radiusA = ((ColliderSphere*)hit.col1->shape().get())->radius();
      const float radiusB = ((ColliderSphere*)hit.col2->shape().get())->radius();

      const float distSquared = glm::length2(posA - posB);
      const float radiusSumSquared = (radiusA + radiusB) * (radiusA + radiusB);
      if (distSquared > radiusSumSquared)
      {
        hit.hit = false;
        break;
      }

      const float distance = sqrtf(distSquared);

      if (distance == 0.0f)
      {
        hit.normal = vec3(0.0f, 1.0f, 0.0f);
        hit.penetration_depth = radiusA;
      }
      else
      {
        hit.normal = (posB - posA) / distance;
        hit.penetration_depth = (radiusA + radiusB) - distance;
      }

      hit.hit = true;
      break;
    }
    case ColliderShape::Type::HeightMap:
    {
      VXR_TRACE_SCOPE("VXR", "Sphere VS HeightMap");
      ref_ptr<mesh::HeightMap> c = ((ColliderHeightMap*)hit.col2->shape().get())->height_map();
      vec3 sphere_pos = hit.rg1->transform()->local_position();
      for (uint32 i = 0; i < c->face_count(); ++i)
      {
        vec3 v0 = c->vertices()[i * 3 + 0];
        vec3 v1 = c->vertices()[i * 3 + 1];
        vec3 v2 = c->vertices()[i * 3 + 2];
        vec3 n = c->normals()[i * 3];

        // Find point P on triangle ABC closest to sphere center
        vec3 p = closestPtPointTriangle(sphere_pos, v0, v1, v2);

        // Sphere and triangle intersect if the (squared) distance from sphere
        // center to point p is less than the (squared) sphere radius
        vec3 v = p - sphere_pos;
        float vDot = glm::dot(v, v);
        if (vDot <= radius_ * radius_)
        {
          hit.normal = n;
          hit.penetration_depth = radius_ - sqrtf(vDot);
          hit.hit = true;
          break;
        }
      }
      break;
    }
    default:
      break;
    }
  }

  void ColliderSphere::set_radius(const float radius)
  {
    radius_ = radius;
  }

  float ColliderSphere::radius() const
  {
    return radius_;
  }
}