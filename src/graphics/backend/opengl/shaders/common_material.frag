// -------------------------------------------------------------------------------
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
// -------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// MaterialInput
//--------------------------------------------------------------------------------

MaterialInput initMaterial()
{
  MaterialInput material;
  material.baseColor = vec4(1.0);
  material.roughness = 1.0;
  material.metallic = 0.0;
  material.reflectance = 0.5;
  material.ambientOcclusion = 1.0;
#if MAT_HAS_EMISSIVE
  material.emissive = vec4(0.0);
#endif
#if MAT_HAS_ANISOTROPY
  material.anisotropy = 0.0;
  material.anisotropyDirection = vec3(1.0, 0.0, 0.0);
#endif
#if MAT_HAS_NORMAL_MAP
  material.normal = vec3(0.0, 0.0, 1.0);
#endif
#if MAT_HAS_CLEAR_COAT
  material.clearCoat = 1.0;
  material.clearCoatRoughness = 0.0;
#if MAT_HAS_CLEAR_COAT_NORMAL_MAP
  material.clearCoatNormal = vec3(0.0, 0.0, 1.0);
#endif
#endif
#if MAT_HAS_IRIDESCENCE
  material.iridescenceMask = 0.0;
  material.filmThickness = 1.0;
  material.baseIor = 1.33;
  material.kExtinction = 0.33;
#endif
  return material;
}

ShadingParameters prepareMaterial(const MaterialInput material)
{
  ShadingParameters shading;
  shading.V = normalize(getViewPosition() - getWorldPosition());
  vec3 N = normalize(getWorldNormal());
#if MAT_HAS_ANISOTROPY || MAT_HAS_NORMAL_MAP || MAT_HAS_CLEAR_COAT_NORMAL_MAP
#if MESH_HAS_PRECOMPUTED_TANGENTS
  vec3 T = normalize(in_tangent);
  vec3 B = normalize(in_bitangent);
#else
  // Get edge vectors of the pixel triangle.
  vec3 edge1  = dFdx(getWorldPosition());
  vec3 edge2  = dFdy(getWorldPosition());
  vec2 uv1 = dFdx(getUV());
  vec2 uv2 = dFdy(getUV());
#if   MAT_COMPUTE_TBN_MODE == COMPUTE_TBN_FAST
  vec3 T = normalize(edge1 * uv2.y - edge2 * uv1.y);
  vec3 B = normalize(cross(N, T));
#elif MAT_COMPUTE_TBN_MODE == COMPUTE_TBN_MESH
  float r = 1.0f / (uv1.x * uv2.y - uv1.y * uv2.x);
  vec3 T = normalize((edge1 * uv2.y - edge2 * uv1.y) * r);
//vec3 B = normalize((edge1 * uv2.x - edge2 * uv1.x) * r);
  vec3 B = normalize(cross(N, T));
#elif MAT_COMPUTE_TBN_MODE == COMPUTE_TBN_INVSQRT
  vec3 dp2perp = cross(edge2, N);
  vec3 dp1perp = cross(N, edge1);
  vec3 T = dp2perp * uv1.x + dp1perp * uv2.x;
  vec3 B = dp2perp * uv1.y + dp1perp * uv2.y;
  // Construct a scale-invariant frame 
  float invmax = inversesqrt(max(dot(T,T), dot(B,B)));
  T *= invmax;
  B *= invmax;
#endif
  // Gram Schmidt process
  T = normalize(T - N * dot(N, T));
  B = normalize(B - N * dot(N, B));
#endif
  shading.TBN = mat3(T, B, N);
#else
  shading.TBN[2] = N;
#endif

#if MAT_HAS_NORMAL_MAP
  shading.N = normalize(shading.TBN * material.normal);
#else
  shading.N = shading.TBN[2];
#endif

#if MAT_HAS_CLEAR_COAT && MAT_HAS_CLEAR_COAT_NORMAL_MAP
  shading.N_CC = normalize(shading.TBN * material.clearCoatNormal);
#else
  shading.N_CC = shading.TBN[2];
#endif
  shading.R = reflect(-shading.V, shading.N);
  shading.NoV = abs(dot(shading.N, shading.V)) + 1e-4;

  return shading;
}

PixelParameters getPixelParameters(const MaterialInput material, const ShadingParameters shading)
{
  PixelParameters pixel;

  vec4 baseColor = material.baseColor;
  /*if (baseColor.a <= 0.0)
  {
    discard;
  }*/
  float metallic = clamp(material.metallic, 0.0, 1.0);
  float reflectance = clamp(material.reflectance, 0.0, 1.0);

  pixel.diffuseColor = (1.0 - metallic) * baseColor.rgb;
  // Assumes an interface from air to an IOR of 1.5 for dielectrics
  pixel.f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + baseColor.rgb * metallic;

  // Remaps the roughness to a perceptually linear roughness (roughness^2).
  float roughness = clamp(material.roughness, MIN_ROUGHNESS, 1.0);

#if MAT_HAS_IRIDESCENCE
  // Remaps the iridescence mask to a perceptually linear mask (1-(1-iridescence)^3).
  pixel.iridescenceMask = clamp(1.0 - pow(1.0 - material.iridescenceMask, 3.0), 0.0, 1.0);
  pixel.filmThickness = clamp(material.filmThickness, 0.0, 1.0);
  pixel.baseIor = material.baseIor;
  pixel.kExtinction = material.kExtinction;
  
  if (pixel.iridescenceMask > 0.0)
  {
    pixel.iridescenceFresnel = fresnelIridescence(pixel, shading.NoV);
  }
#endif

#if MAT_HAS_CLEAR_COAT
  pixel.clearCoat = material.clearCoat;
  float clearCoatRoughness = material.clearCoatRoughness;
  // Remapped to 0.0..0.6 matches the fact that clear coat layers are almost always glossy.
  clearCoatRoughness = mix(MIN_ROUGHNESS, MAX_CLEAR_COAT_ROUGHNESS, clearCoatRoughness);
  pixel.clearCoatRoughness = clearCoatRoughness;
  pixel.clearCoatLinearRoughness = clearCoatRoughness * clearCoatRoughness;
  // The base layer's f0 is computed assuming an interface from air to an IOR
  // of 1.5, but the clear coat layer forms an interface from IOR 1.5 to IOR
  // 1.5. We recompute f0 by first computing its IOR, then reconverting to f0
  // by using the correct interface.
  pixel.f0 = mix(pixel.f0, f0ClearCoatToSurface(pixel.f0), pixel.clearCoat);
  // The base layer must be at least as rough as the clear coat layer to take 
  // into account possible diffusion by the top layer.
  roughness = mix(roughness, max(roughness, pixel.clearCoatRoughness), pixel.clearCoat);
#endif

  pixel.roughness = roughness;
  pixel.linearRoughness = roughness * roughness;

#if MAT_HAS_ANISOTROPY
  vec3 direction = material.anisotropyDirection;
  pixel.anisotropy = clamp(material.anisotropy, -1.0, 1.0);
  pixel.anisotropicT = normalize(shading.TBN * direction);
  pixel.anisotropicB = normalize(cross(shading.TBN[2], pixel.anisotropicT));
#endif

  // Pre-filtered DFG term used for image-based lighting
  pixel.dfg = prefilteredDFG(pixel.roughness, shading.NoV);

#if MULTIPLE_SCATTERING
  // Energy compensation for multiple scattering in a microfacet model
  // See "Multiple-Scattering Microfacet BSDFs with the Smith Model"
  pixel.energyCompensation = 1.0 + pixel.f0 * (1.0 / pixel.dfg.y - 1.0);
#else
  pixel.energyCompensation = vec3(1.0);
#endif

  return pixel;
}

vec4 evaluateMaterial(const MaterialInput material)
{
  ShadingParameters shading = prepareMaterial(material);
  PixelParameters pixel = getPixelParameters(material, shading);
  
  vec3 color = vec3(0.0);
  color += evaluateIBL(material, pixel, shading);
  color += evaluateLights(material, pixel, shading);
  // + vec4((vec3(0.03) * material.baseColor.xyz), 0.0); // Fake ambient

  color *= material.baseColor.a;

#if MAT_HAS_EMISSIVE
  vec4 emissive = material.emissive;
  float attenuation = 1.0; /// *= exposure 
  color.rgb += emissive.rgb * attenuation;
#endif
  return vec4(applyGammaCorrection(color), material.baseColor.a); /// diffuse alpha
}
// cutout = alpha that makes transparent, =/= transparency