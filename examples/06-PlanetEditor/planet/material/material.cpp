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

#include "material.h"

#include "../../../include/graphics/ui.h"

namespace vxr
{

  PlanetMaterial::PlanetMaterial()
  {
    set_name("PlanetMaterial");
    set_shaders("planet/material/planet.vert", "planet/material/planet.frag");

    set_num_textures(1);
    set_uniforms_enabled(true);
    set_uniforms_name("Planet");
  }

  PlanetMaterial::Instance::Instance()
  {
    init("PlanetMaterial");
  }

  void PlanetMaterial::Instance::set_elevation_min_max(vec2 elevation_min_max)
  {
    uniforms_.planet.elevationMinMax = elevation_min_max;
  }

  vec2 PlanetMaterial::Instance::elevation_min_max() const
  {
    return uniforms_.planet.elevationMinMax;
  }

  void PlanetMaterial::Instance::set_gradient_texture(ref_ptr<Texture> texture)
  {
    gradient_texture_ = texture;
    set_texture(0, gradient_texture_);
  }

  ref_ptr<Texture> PlanetMaterial::Instance::gradient_texture() const
  {
    return gradient_texture_;
  }

} /* end of vxr namespace */