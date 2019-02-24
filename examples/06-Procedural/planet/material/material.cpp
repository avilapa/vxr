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

#include "../../../include/engine/engine.h"
#include "../../../include/core/assets.h"
#include "../../../include/components/ibl.h"

namespace vxr
{

  namespace mat
  {

    Planet::Planet()
    {
      set_name("PlanetMaterial");
      set_shaders("planet/material/planet.vert", "planet/material/planet.frag");

      set_num_textures(4);
      set_uniforms_enabled(true);
      set_uniforms_name("Planet");

      set_common_texture(0, Engine::ref().assetManager()->default_cubemap()); // Irradiance
      set_common_texture(1, Engine::ref().assetManager()->default_cubemap()); // Prefiltered
      set_common_texture(2, Engine::ref().assetManager()->default_texture_white()); // BRDF LUT

      Engine::ref().ibl()->subscribeMaterialForIBLTextures(name());
    }

    Planet::Instance::Instance()
    {
      init("PlanetMaterial");

      set_texture(3, Engine::ref().assetManager()->default_texture_white()); // Gradient

      uniforms_.planet.elevationMinMax = vec2(0);
    }

    void Planet::Instance::set_elevation_min_max(vec2 elevation_min_max)
    {
      uniforms_.planet.elevationMinMax = elevation_min_max;
    }

    vec2 Planet::Instance::elevation_min_max() const
    {
      return uniforms_.planet.elevationMinMax;
    }

    void Planet::Instance::set_gradient_texture(ref_ptr<Texture> texture)
    {
      set_texture(3, texture);
    }

    ref_ptr<Texture> Planet::Instance::gradient_texture() const
    {
      return texture(3);
    }

  } /* end of mat namespace */

} /* end of vxr namespace */