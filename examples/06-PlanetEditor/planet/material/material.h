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

#include "../../../include/core/gameobject.h"
#include "../../../include/graphics/materials/default_materials.h"

namespace vxr 
{

  class PlanetMaterial : public Material
  {
    VXR_OBJECT(PlanetMaterial, Material);
  public:
    PlanetMaterial();

    class Instance : public MaterialInstance
    {
      VXR_OBJECT(Instance, MaterialInstance);
    public:
      Instance();

      void set_elevation_min_max(vec2 elevation_min_max);
      vec2 elevation_min_max() const;

      void set_gradient_texture(ref_ptr<Texture> texture);
      ref_ptr<Texture> gradient_texture() const;

    protected:
      ref_ptr<Texture> gradient_texture_;
    };

  };

} /* end of vxr namespace */