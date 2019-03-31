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

#include "collider_shape.h"

/**
* \file collider_height_map.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief 
*
*/
namespace vxr 
{
  
  namespace mesh { class HeightMap; }

  class ColliderHeightMap : public ColliderShape
  {
    VXR_OBJECT(ColliderHeightMap, ColliderShape);
  public:
    ColliderHeightMap();
    ~ColliderHeightMap();

    void checkCollision(Hit& hit) const override;
    void getAabb(ref_ptr<Transform> transform, vec3& aabb_min, vec3& aabb_max) const override;
    void setAabbExtents(vec3 min, vec3 max);

    void set_height_map(ref_ptr<mesh::HeightMap> mesh);
    ref_ptr<mesh::HeightMap> height_map() const;

  private:
    ref_ptr<mesh::HeightMap> height_map_;
    vec3 local_aabb_min_;
    vec3 local_aabb_max_;
  };

} /* end of vxr namespace */