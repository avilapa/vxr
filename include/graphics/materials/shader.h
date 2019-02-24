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

#include "../../engine/types.h"

/**
* \file shader.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Shader Uniform Structures. 
*
*/
namespace vxr
{

  namespace Shader
  {

    string Load(const char* file);

    static const char* u_model = "u_model";

    struct CommonData
    {
      mat4 u_proj;
      mat4 u_view;
      vec2 u_resolution;
      vec2 u_xy;
      vec4 u_clear_color;
      vec4 u_view_pos_num_lights;
      vec4 u_time;
    };

    struct LightData
    {
      vec4 position_falloff[kMaxLightSources];
      vec4 color_intensity[kMaxLightSources];
      vec4 direction_ambient[kMaxLightSources];
    };

    union UniformData
    {
      // Add here specific uniform structures for any new shaders (only float x2/vec2/vec4).
      struct Unlit
      {
        vec4 color;
      } unlit;
      struct Standard
      {
        vec4 albedo;
        vec4 emissive;
        vec4 metallic_roughness_reflectance_ao;
        vec4 clear_coat_clear_coat_roughness;
        vec4 iridescence_mask_thickness_ior_k;
      } standard;
      struct CubemapConvolution
      {
        mat4 conv_proj;
        mat4 conv_view;
        vec4 aux;
      } cubemap_convolution;
      struct Planet
      {
        vec2 elevationMinMax;
      } planet;
    };

  } /* end of Shader namespace */

} /* end of vxr namespace */