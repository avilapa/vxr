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

#include "../../include/physics/collider_height_map.h"
#include "../../include/physics/collider_sphere.h"
#include "../../include/graphics/height_map.h"
#include "../../include/components/collider.h"
#include "../../include/components/rigidbody.h"
#include "../../include/components/transform.h"

namespace vxr 
{

  ColliderHeightMap::ColliderHeightMap()
  {
    set_name("Collider HeightMap");
    type_ = Type::HeightMap;
  }

  ColliderHeightMap::~ColliderHeightMap()
  {
  }

  void ColliderHeightMap::checkCollision(Hit& hit) const
  {
    switch (hit.col2->shape()->colType())
    {
    case ColliderShape::Type::Sphere:
    {
      VXR_TRACE_SCOPE("VXR", "HeightMap VS Sphere");
      ref_ptr<ColliderSphere> c = (ColliderSphere*)hit.col2->shape().get();
      vec3 sphere_pos = hit.rg2->transform()->local_position();
      for (uint32 i = 0; i < height_map_->face_count(); ++i)
      {
        vec3 v0 = height_map_->vertices()[i * 3 + 0];
        vec3 v1 = height_map_->vertices()[i * 3 + 1];
        vec3 v2 = height_map_->vertices()[i * 3 + 2];
        vec3 n = height_map_->normals()[i * 3];

        // Find point P on triangle ABC closest to sphere center
        vec3 p = closestPtPointTriangle(sphere_pos, v0, v1, v2);

        // Sphere and triangle intersect if the (squared) distance from sphere
        // center to point p is less than the (squared) sphere radius
        vec3 v = p - sphere_pos;
        float vDot = glm::dot(v, v);
        if (vDot <= c->radius() * c->radius())
        {
          hit.normal = n;
          hit.penetration_depth = c->radius() - sqrtf(vDot);
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

  void ColliderHeightMap::getAabb(ref_ptr<Transform> transform, vec3& aabb_min, vec3& aabb_max) const
  {
    aabb_min = transform->local_position() - vec3(local_aabb_min_);
    aabb_max = transform->local_position() + vec3(local_aabb_max_);
  }

  void ColliderHeightMap::setAabbExtents(vec3 min, vec3 max)
  {
    local_aabb_min_ = min;
    local_aabb_max_ = max;
  }

  void ColliderHeightMap::set_height_map(ref_ptr<mesh::HeightMap> mesh)
  {
    height_map_ = mesh;
  }

  ref_ptr<mesh::HeightMap> ColliderHeightMap::height_map() const
  {
    return height_map_;
  }
}