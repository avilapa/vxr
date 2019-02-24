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

// min roughness such that (MIN_ROUGHNESS^4) > 0 in fp16 (i.e. 2^(-14/4), slightly rounded up)
#define MIN_ROUGHNESS              0.045
#define MIN_LINEAR_ROUGHNESS       0.002025
#define MAX_CLEAR_COAT_ROUGHNESS   0.6

MaterialInput initMaterial()
{
  MaterialInput material;
  material.baseColor = vec4(1.0);
  material.roughness = 1.0;
  material.metallic = 0.0;
  material.reflectance = 0.5;
  material.ambientOcclusion = 1.0;

  material.emissive = vec4(0.0);

  material.clearCoat = 1.0;
  material.clearCoatRoughness = 0.0;

  material.anisotropy = 0.0;
  material.anisotropyDirection = vec3(1.0, 0.0, 0.0);

  material.normal = vec3(0.5, 0.5, 1.0);
  material.clearCoatNormal = vec3(0.5, 0.5, 1.0);

  // Default iridescence values

  return material;
}

vec3 getNormalFromMap(vec3 normal)
{
    vec3 tangentNormal = normal * 2.0 - 1.0;

    vec3 Q1  = dFdx(getWorldPosition());
    vec3 Q2  = dFdy(getWorldPosition());
    vec2 st1 = dFdx(getUV());
    vec2 st2 = dFdy(getUV());

    vec3 N   = normalize(getWorldNormal());
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

ShadingParameters prepareMaterial(const MaterialInput material)
{
  ShadingParameters shading;

  shading.V = normalize(getViewPosition() - getWorldPosition());
  shading.N = normalize(getWorldNormal() * material.normal);
  shading.N_CC = normalize(getWorldNormal() * material.clearCoatNormal);
  //shading.N = getNormalFromMap(material.normal);
  shading.R = reflect(-shading.V, shading.N);
  shading.NoV = abs(dot(shading.N, shading.V)) + 1e-4;

  return shading;
}

PixelParameters getPixelParameters(const MaterialInput material, const ShadingParameters shading)
{
  PixelParameters pixel;

  vec4 baseColor = material.baseColor;
  float metallic = clamp(material.metallic, 0.0, 1.0);
  float reflectance = clamp(material.reflectance, 0.0, 1.0);

  pixel.diffuseColor = (1.0 - metallic) * baseColor.rgb;
  // Assumes an interface from air to an IOR of 1.5 for dielectrics
  pixel.f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + baseColor.rgb * metallic;

  // Remaps the roughness to a perceptually linear roughness (roughness^2).
  pixel.roughness = clamp(material.roughness, MIN_ROUGHNESS, 1.0);
  pixel.linearRoughness = pixel.roughness * pixel.roughness;

#if USE_IRIDESCENCE
  // Remaps the iridescence mask to a perceptually linear mask (1-(1-roughness)^3).
  pixel.iridescenceMask = clamp(1.0 - pow(1.0 - material.iridescenceMask, 3.0), 0.0, 1.0);
  pixel.filmThickness = clamp(material.filmThickness, 0.0, 1.0);
  pixel.baseIor = material.baseIor;
  pixel.kExtinction = material.kExtinction;
  
  if (pixel.iridescenceMask > 0.0)
  {
    pixel.iridescenceFresnel = fresnelIridescence(pixel, shading.NoV);
  }
#endif

#if USE_CLEAR_COAT
  pixel.clearCoat = material.clearCoat;
  float clearCoatRoughness = material.clearCoatRoughness;
  // Remapped to 0.0..0.6 matches the fact that clear coat layers are almost always glossy.
  clearCoatRoughness = mix(MIN_ROUGHNESS, MAX_CLEAR_COAT_ROUGHNESS, clearCoatRoughness);
  pixel.clearCoatRoughness = clearCoatRoughness;
  pixel.clearCoatLinearRoughness = clearCoatRoughness * clearCoatRoughness;
  // The base layer's f0 is computed assuming an interface from air to an IOR
  // of 1.5, but the clear coat layer forms an interface from IOR 1.5 to IOR
  // 1.5. We recompute f0 by first computing its IOR, then reconverting to f0
  // by using the correct interface
  pixel.f0 = mix(pixel.f0, f0ClearCoatToSurface(pixel.f0), pixel.clearCoat);
#endif

  // Pre-filtered DFG term used for image-based lighting
  pixel.dfg = prefilteredDFG(pixel.roughness, shading.NoV);

#if USE_MULTIPLE_SCATTERING
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

#if defined(MATERIAL_HAS_EMISSIVE)
  //highp
  vec4 emissive = material.emissive;
  float attenuation = 1.0; /// *= exposure 
  color.rgb += emissive.rgb * attenuation;
#endif
  return vec4(applyGammaCorrection(color), material.baseColor.a); /// diffuse alpha
}

// Cutout = alpha that makes transparent, =/= transparency