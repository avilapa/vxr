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

#include "hit.h"
#include "../../include/core/object.h"

/**
* \file collider_shape.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief 
*
*/
namespace vxr 
{
  
  class Transform;

  class ColliderShape : public Object
  {
    VXR_OBJECT(ColliderShape, Object);
  public:
    virtual ~ColliderShape();

    struct Type
    {
      enum Enum
      {
        Undefined = 0,
        Sphere,
        HeightMap,
      };
    };

    const Type::Enum& colType() const;

    virtual void checkCollision(Hit& hit) const;
    virtual void getAabb(ref_ptr<Transform> transform, vec3& aabb_min, vec3& aabb_max) const = 0;

  protected:
    ColliderShape();

  protected:
    Type::Enum type_;
  };

} /* end of vxr namespace */