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

// Normal distribution functions
#define SPECULAR_D_GGX_UPYDK        0
#define SPECULAR_D_GGX              1

// Visibility functions
#define SPECULAR_V_SMITH_GGX_UPYDK  0
#define SPECULAR_V_SMITH_GGX        1
#define SPECULAR_V_SMITH_GGX_FAST   2
#define SPECULAR_V_KELEMEN          3

// Fresnel functions
#define SPECULAR_F_SCHLICK          0


#define BRDF_SPECULAR_D             SPECULAR_D_GGX
#define BRDF_SPECULAR_V             SPECULAR_V_SMITH_GGX_UPYDK
#define BRDF_SPECULAR_F             SPECULAR_F_SCHLICK

#define BRDF_CLEAR_COAT_D           SPECULAR_D_GGX
#define BRDF_CLEAR_COAT_V           SPECULAR_V_KELEMEN
#define BRDF_CLEAR_COAT_F           SPECULAR_F_SCHLICK
#define BRDF_CLEAR_COAT_IOR         1.5 
#define BRDF_CLEAR_COAT_F0          0.04 // iorToF0(BRDF_CLEAR_COAT_IOR)

#define DIFFUSE_LAMBERT             0
#define DIFFUSE_BURLEY              1


#define BRDF_DIFFUSE                DIFFUSE_LAMBERT


#define USE_MULTIPLE_SCATTERING     1
#define USE_CLEAR_COAT              1
#define USE_IRIDESCENCE             1


// XYZ to CIE 1931 RGB color space (using neutral E illuminant)
const mat3 XYZ_TO_RGB = mat3(2.3706743, -0.5138850, 0.0052982, -0.9000405, 1.4253036, -0.0146949, -0.4706338, 0.0885814, 1.0093968);

//------------------------------------------------------------------------------
// Index of refraction (IOR)
//------------------------------------------------------------------------------

float iorToF0(float transmittedIor, float incidentIor) 
{
  return sq((transmittedIor - incidentIor) / (transmittedIor + incidentIor));
}

float f0ToIor(float f0) 
{
  float r = sqrt(f0);
  return (1.0 + r) / (1.0 - r);
}

vec3 f0ToIor(vec3 f0)
{
  return vec3(f0ToIor(f0.x), f0ToIor(f0.y), f0ToIor(f0.z));
}

vec3 f0ClearCoatToSurface(const vec3 f0) 
{
  // Approximation of iorToF0(f0ToIor(f0), 1.5). Assumes IOR of 1.5 (4% reflectance)
  return clamp(f0 * (f0 * (0.941892 - 0.263008 * f0) + 0.346479) - 0.0285998, 0.0, 1.0);
}

//------------------------------------------------------------------------------
// Hammersley
//------------------------------------------------------------------------------

// Efficient VanDerCorpus calculation retrieved from:
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
    
vec2 Hammersley(uint i, float invN)
{
    return vec2(invN * float(i), RadicalInverse_VdC(i));
}

float Depol (vec2 polV) 
{ 
  return 0.5 * (polV.x + polV.y); 
}

vec3 DepolColor (vec3 colS, vec3 colP)
{ 
  return 0.5 * (colS + colP); 
}

// Belcour & Barla 2017, "A practical extension to microfacet theory for the modeling of varying iridescence"
// Evaluation XYZ sensitivity curves in Fourier space
vec3 evaluateSensitivity(float opd, float shift)
{
  // OPD: Optical Path Difference
  // Use Gaussian fits, given by 3 parameters: val, pos and var
  float phase = 2*PI * opd * 1.0e-6;
  vec3 val = vec3(5.4856e-13, 4.4201e-13, 5.2481e-13);
  vec3 pos = vec3(1.6810e+6, 1.7953e+6, 2.2084e+6);
  vec3 var = vec3(4.3278e+9, 9.3046e+9, 6.6121e+9);
  vec3 xyz = val * sqrt(2*PI * var) * cos(pos * phase + shift) * exp(-var * phase*phase);
  xyz.x   += (9.7470e-14 * sqrt(2*PI * 4.5282e+9) * cos(2.2399e+6 * phase + shift) * exp(-4.5282e+9 * phase*phase));
  return xyz / 1.0685e-7;
}

//--------------------------------------------------------------------------------
// Specular BRDF Implementation
//--------------------------------------------------------------------------------

float D_GGX_UPYDK(float linearRoughness, float NoH) 
{
  float NoH2 = NoH * NoH;

  float a     = linearRoughness * linearRoughness;
  float a2    = a * a;

  float nom   = a2;
  float denom = (NoH2 * (a2 - 1.0) + 1.0);
  denom       = PI * denom * denom;

  return nom / max(denom, 0.001);
}

float D_GGX(float linearRoughness, float NoH)
{
  // Walter et al. 2007, "Microfacet Models for Refraction through Rough Surfaces"
  float NoH2 = NoH * NoH;

  float a = NoH * linearRoughness;
  float k = linearRoughness / ((1.0 - NoH2) + a * a);
  float d = k * k * (1.0 / PI);
  return clamp(d, 0.0, 1.0);
}

vec3 D_GGX_HemisphereImportanceSample(float roughness, vec2 U, vec3 N)
{
    float a = roughness * roughness;
    
    float phi = 2.0 * PI * U.x;
    float cosTheta = sqrt((1.0 - U.y) / (1.0 + (a*a - 1.0) * U.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    
    // Spherical to cartesian coordinates - halfway vector
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
    
    // Tangent to world space
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
    
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

float V_GeometrySchlickGGX_UPYDK(float linearRoughness, float NoV) 
{
  float r = (linearRoughness + 1.0);
  float k = (r * r) / 8.0;

  float nom   = NoV;
  float denom = NoV * (1.0 - k) + k;

  return nom / denom;
}

float V_SmithGGX_UPYDK(float linearRoughness, float NoV, float NoL) 
{
  float ggx2 = V_GeometrySchlickGGX_UPYDK(linearRoughness, NoV);
  float ggx1 = V_GeometrySchlickGGX_UPYDK(linearRoughness, NoL);
  return ggx1 * ggx2;
}

float V_SmithGGXCorrelated(float linearRoughness, float NoV, float NoL) 
{
  // Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"
  float a2 = linearRoughness * linearRoughness;
  // TODO: lambdaV can be pre-computed for all the lights, it should be moved out of 
  // this function
  float lambdaV = NoL * sqrt((NoV - NoV * a2) * NoV + a2);
  float lambdaL = NoV * sqrt((NoL - NoL * a2) * NoL + a2);
  float V = 0.5 / (lambdaV + lambdaL);
  // a2=0 => v = 1 / 4*NoL*NoV   => min=1/4, max=+inf
  // a2=1 => v = 1 / 2*(NoL+NoV) => min=1/4, max=+inf
  return clamp(V, 0.0, 1.0);
}

float V_SmithGGXCorrelated_Fast(float linearRoughness, float NoV, float NoL) 
{
  // Hammon 2017, "PBR Diffuse Lighting for GGX+Smith Microsurfaces"
  float v = 0.5 / mix(2.0 * NoL * NoV, NoL + NoV, linearRoughness);
  return clamp(v, 0.0, 1.0);
}

float V_Kelemen(float LoH) 
{
    // Kelemen 2001, "A Microfacet Based Coupled Specular-Matte BRDF Model with Importance Sampling"
    return clamp(0.25 / (LoH * LoH), 0.0, 1.0);
}

float V_GeometrySchlickGGX_DFG(float roughness, float NoV)
{
    // Note that we use a different k for IBL
    float a = roughness;
    float k = (a * a) * 0.5;

    float nom   = NoV;
    float denom = NoV * (1.0 - k) + k;

    return nom / denom;
}

float V_SmithGGX_DFG(float roughness, float NoV, float NoL)
{
    float ggx1 = V_GeometrySchlickGGX_DFG(roughness, NoV);
    float ggx2 = V_GeometrySchlickGGX_DFG(roughness, NoL);

    return ggx1 * ggx2;
}

vec3 F_Schlick(const vec3 f0, float f90, float VoH)
{
  // Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"
  return f0 + (f90 - f0) * pow5(1.0 - VoH);

  //float x5 = pow5(1.0 - VoH);
  //return f0 * (1.0 - x5) + (f90 * x5);
}

float F_Schlick(float f0, float f90, float VoH) 
{
  return f0 + (f90 - f0) * pow5(1.0 - VoH);
}


// Belcour & Barla 2017, "A practical extension to microfacet theory for the modeling of varying iridescence"
// Fresnel equations for dielectric/dielectric interfaces.
void F_Dielectric(in float ct1, in float n1, in float n2, out vec2 R, out vec2 phi) 
{
  // Sinus theta1 'squared'
  float st1 = (1 - ct1*ct1);
  float nr = n1/n2;

  if(sq(nr) * st1 > 1.0) // Total Internal Reflection
  {
    R = vec2(1.0);
    phi = 2.0 * atan(vec2(-sq(nr) *  sqrt(st1 - 1.0/sq(nr)) / ct1,-sqrt(st1 - 1.0/sq(nr)) / ct1));
  } 
  else // Transmission & Reflection
  {   
  
    float ct2 = sqrt(1 - sq(nr) * st1);
    vec2 r = vec2((n2*ct1 - n1*ct2) / (n2*ct1 + n1*ct2),
            (n1*ct1 - n2*ct2) / (n1*ct1 + n2*ct2));
    phi.x = (r.x < 1e-5) ? PI : 0.0;
    phi.y = (r.y < 1e-5) ? PI : 0.0;
    R = sq(r);
  }
}

// Belcour & Barla 2017, "A practical extension to microfacet theory for the modeling of varying iridescence"
// Fresnel equations for dielectric/conductor interfaces.
void F_Conductor(in float ct1, in float n1, in float n2, in float k, out vec2 R, out vec2 phi)
{
  if (k < 1e-5) 
  {
    // Use dielectric formula to avoid numerical issues
    F_Dielectric(ct1, n1, n2, R, phi);
    return;
  }

  float A = sq(n2) * (1-sq(k)) - sq(n1) * (1-sq(ct1));
  float B = sqrt( sq(A) + sq(2*sq(n2)*k) );
  float U = sqrt((A+B)/2.0);
  float V = sqrt((B-A)/2.0);

  R.y = (sq(n1*ct1 - U) + sq(V)) / (sq(n1*ct1 + U) + sq(V));
  phi.y = atan(sq(U)+sq(V)-sq(n1*ct1), 2*n1 * V*ct1) + PI;

  R.x = (sq(sq(n2)*(1-sq(k))*ct1 - n1*U) + sq(2*sq(n2)*k*ct1 - n1*V)) 
    / (sq(sq(n2)*(1-sq(k))*ct1 + n1*U) + sq(2*sq(n2)*k*ct1 + n1*V));
  phi.x = atan(sq(sq(n2)*(1+sq(k))*ct1) - sq(n1)*(sq(U)+sq(V)), 2*n1*sq(n2)*ct1 * (2*k*U - (1-sq(k))*V));
}

vec3 F_Airy(float ct1, float ct2, float Dinc, float eta_1, float eta_2, float eta_3, float k)
{
  // First interface
  vec2 R12, phi12;
  F_Dielectric(ct1, eta_1, eta_2, R12, phi12);
  vec2 R21  = vec2(R12);
  vec2 T121 = vec2(1.0) - R12;
  vec2 phi21 = vec2(PI, PI) - phi12;

  // Second interface
  vec2 R23, phi23;
  F_Conductor(ct2, eta_2, eta_3, k, R23, phi23);
  
  // Phase shift
  float OPD = Dinc * ct2;
  vec2 phi2 = phi21 + phi23;

  // Compound terms
  vec3 I = vec3(0, 0, 0);
  vec2 R123 = R12 * R23;
  vec2 r123 = sqrt(R123);
  vec2 Rs   = sq(T121) * R23 / (vec2(1.0) - R123);

  // Reflectance term for m=0 (DC term amplitude)
  vec2 C0 = R12 + Rs;
  vec3 S0 = evaluateSensitivity(0.0, 0.0);
  I += (Depol(C0) * S0);

  // Reflectance term for m>0 (pairs of diracs)
  vec2 Cm = Rs - T121;
  for (int m = 1; m <= 3; ++m)
  {
    Cm *= r123;
    vec3 SmS = 2.0 * evaluateSensitivity(m * OPD, m * phi2.x);
    vec3 SmP = 2.0 * evaluateSensitivity(m * OPD, m * phi2.y);
    I += DepolColor(Cm.x * SmS, Cm.y * SmP);
  }

  // Convert back to RGB reflectance
  I = clamp(I * XYZ_TO_RGB, 0.0, 1.0);

  return I;
}

//--------------------------------------------------------------------------------
// Specular BRDF Dispatch
//--------------------------------------------------------------------------------

float distribution(float linearRoughness, float NoH)
{
#if   BRDF_SPECULAR_D == SPECULAR_D_GGX_UPYDK
  return D_GGX_UPYDK(linearRoughness, NoH);
#elif BRDF_SPECULAR_D == SPECULAR_D_GGX
  return D_GGX(linearRoughness, NoH);
#endif
}

float visibility(float linearRoughness, float NoV, float NoL)
{
#if   BRDF_SPECULAR_V == SPECULAR_V_SMITH_GGX_UPYDK
  return V_SmithGGX_UPYDK(linearRoughness, NoV, NoL);
#elif BRDF_SPECULAR_V == SPECULAR_V_SMITH_GGX
  return V_SmithGGXCorrelated(linearRoughness, NoV, NoL);
#elif BRDF_SPECULAR_V == SPECULAR_V_SMITH_GGX_FAST
  return V_SmithGGXCorrelated_Fast(linearRoughness, NoV, NoL);
#endif
}

vec3 fresnel(const vec3 f0, float LoH)
{
#if   BRDF_SPECULAR_F == SPECULAR_F_SCHLICK
  //float f90 = 1.0;
  float f90 = clamp(dot(f0, vec3(50.0 * 0.33)), 0.0, 1.0);
  return F_Schlick(f0, f90, LoH);
#endif
}

float distributionClearCoat(float linearRoughness, float NoH)
{
#if   BRDF_CLEAR_COAT_D == SPECULAR_D_GGX_UPYDK
  return D_GGX_UPYDK(linearRoughness, NoH);
#elif BRDF_CLEAR_COAT_D == SPECULAR_D_GGX
  return D_GGX(linearRoughness, NoH);
#endif
}

float visibilityClearCoat(float LoH)
{
#if   BRDF_CLEAR_COAT_V == SPECULAR_V_KELEMEN
    return V_Kelemen(LoH);
#endif
}

float fresnelClearCoat(float LoH)
{
#if   BRDF_CLEAR_COAT_F == SPECULAR_F_SCHLICK
  float f90 = 1.0;
  return F_Schlick(BRDF_CLEAR_COAT_F0, f90, LoH);
#endif
}

float distributionAnisotropic()
{
  return 0.0;
}

float visibilityAnisotropic()
{
  return 0.0;
}

vec3 fresnelIridescence(const PixelParameters pixel, float LoH)
{
  // iridescenceThickness unit is micrometer for this equation here. Mean 0.5 is 500nm.
  float Dinc = pixel.filmThickness * 3.0;
  float eta_0 = 1.0;
  float eta_1 = mix(1.0, BRDF_CLEAR_COAT_IOR, pixel.clearCoat);
  float eta_2 = mix(2.0, 1.0, pixel.filmThickness);
  float eta_3 = pixel.baseIor;

  float cosTheta1 = sqrt(1.0 + sq(eta_0 / eta_1) * (sq(LoH) - 1.0));
  float cosTheta2 = sqrt(1.0 - sq(eta_1 / eta_2) * (1.0 - sq(cosTheta1)));

  return F_Airy(cosTheta1, cosTheta2, Dinc, eta_1, eta_2, eta_3, pixel.kExtinction);
}

//--------------------------------------------------------------------------------
// Diffuse BRDF Implementation
//--------------------------------------------------------------------------------

float Fd_Lambert() 
{
  return 1.0 / PI;
}

float Fd_Burley(float linearRoughness, float NoV, float NoL, float LoH) 
{
    // Burley 2012, "Physically-Based Shading at Disney"
    float f90 = 0.5 + 2.0 * linearRoughness * LoH * LoH;
    float lightScatter = F_Schlick(1.0, f90, NoL);
    float viewScatter  = F_Schlick(1.0, f90, NoV);
    return lightScatter * viewScatter * (1.0 / PI);
}

//--------------------------------------------------------------------------------
// Diffuse BRDF Dispatch
//--------------------------------------------------------------------------------

float diffuse(float linearRoughness, float NoV, float NoL, float LoH)
{
#if   BRDF_DIFFUSE == DIFFUSE_LAMBERT
  return Fd_Lambert();
#elif BRDF_DIFFUSE == DIFFUSE_BURLEY
  return Fd_Burley(linearRoughness, NoV, NoL, LoH);
#endif
}

//--------------------------------------------------------------------------------
// BRDF Lobes
//--------------------------------------------------------------------------------

float clearCoatLobe(const PixelParameters pixel, const ShadingParameters shading, const vec3 H, float LoH, out float F)
{
  // We can use precalculated NoH if there's no N_CC map.
  float NoH = clamp(dot(shading.N_CC, H), 0.0, 1.0);

  float D = distributionClearCoat(pixel.clearCoatLinearRoughness, NoH);
  float V = visibilityClearCoat(LoH);
  F = fresnelClearCoat(LoH) * pixel.clearCoat;

  return D * V * F;
}

vec3 anisotropicLobe()
{
  return vec3(0.0);
}

vec3 isotropicLobe(const PixelParameters pixel, float NoV, float NoL, float NoH, float LoH, out vec3 out_F)
{
  float D = distribution(pixel.linearRoughness, NoH);
  float V = visibility(pixel.linearRoughness, NoV, NoL);

#if USE_IRIDESCENCE
  out_F = fresnelIridescence(pixel, LoH);
  vec3 F = mix(fresnel(pixel.f0, LoH), out_F, pixel.iridescenceMask);
#else
  out_F = vec3(1.0);
  vec3 F = fresnel(pixel.f0, LoH); /// clamp(dot(H, V), 0.0, 1.0)
#endif
  return D * V * F;
}

vec3 specularLobe(const PixelParameters pixel, float NoV, float NoL, float NoH, float LoH, out vec3 out_F)
{
  return isotropicLobe(pixel, NoV, NoL, NoH, LoH, out_F);
}

vec3 diffuseLobe(const PixelParameters pixel, float NoV, float NoL, float LoH)
{
  return pixel.diffuseColor * diffuse(pixel.linearRoughness, NoV, NoL, LoH);
}

//--------------------------------------------------------------------------------
// Shading
//--------------------------------------------------------------------------------

vec3 diffuseIrradiance(vec3 N)
{
  return sRGBtoLinear(texture(u_texCube0, N).rgb);
}

vec3 specularIrradiance(vec3 R, float roughness)
{
  const float MAX_REFLECTION_LOD = 4.0;

  float lod = MAX_REFLECTION_LOD * roughness;
  return sRGBtoLinear(textureLod(u_texCube1, R, lod).rgb); 
}

vec3 specularDFG(const PixelParameters pixel)
{
#if USE_MULTIPLE_SCATTERING
  // (1 - f0) * dfg.x + f0 * dfg.y
  return mix(pixel.dfg.xxx, pixel.dfg.yyy, pixel.f0);
#else
  return pixel.f0 * pixel.dfg.x + pixel.dfg.y;
#endif
}

vec2 prefilteredDFG(float roughness, float NoV)
{
  return texture(u_tex2d2, vec2(NoV, roughness)).rg;
}

float specularAO(float NoV, float ao, float roughness) 
{
  return clamp(pow(NoV + ao, exp2(-16.0 * roughness - 1.0)) - 1.0 + ao, 0.0, 1.0);
}

vec3 surfaceShading(const PixelParameters pixel, const ShadingParameters shading, 
                    const Light light, const float occlusion)
{
  vec3 H = normalize(shading.V + light.L);
  
  float NoV = shading.NoV;
  float NoL = max(dot(shading.N, light.L), 0.0);
  float NoH = max(dot(shading.N, H), 0.0);
  float LoH = max(dot(light.L, H), 0.0);

  vec3 F;
  vec3 Fr = specularLobe(pixel, NoV, NoL, NoH, LoH, F);
  vec3 Fd = diffuseLobe(pixel, NoV, NoL, LoH);

#if USE_IRIDESCENCE
  vec3 iridescenceAttenuation = vec3(1.0) - F * pixel.iridescenceMask;
  Fd *= iridescenceAttenuation;
  //Fr *= iridescenceAttenuation * iridescenceAttenuation;
#endif

#if USE_CLEAR_COAT
  float F_CC;
  float clearCoat = clearCoatLobe(pixel, shading, H, LoH, F_CC);
  // Energy compensation and absorption; the clear coat Fresnel term is
  // squared to take into account both entering through and exiting through
  // the clear coat layer
  float attenuation = 1.0 - F_CC;

  vec3 color = (Fd + Fr * pixel.energyCompensation * attenuation) * attenuation * NoL;

  // If the material has a normal map, we want to use the geometric normal
  // instead to avoid applying the normal map details to the clear coat layer
  float NoL_CC = clamp(dot(shading.N_CC, light.L), 0.0, 1.0);
  color += clearCoat * NoL_CC;

  // Early exit to avoid the extra multiplication by NoL
  return color * light.color * light.intensity * light.attenuation * occlusion;
#else
  vec3 color = Fd + Fr * pixel.energyCompensation;
  return color * light.color * light.intensity * light.attenuation * NoL * occlusion;
#endif
}

vec3 surfaceShadingIBL(const MaterialInput material, const PixelParameters pixel, const ShadingParameters shading)
{
  float diffuseAO = material.ambientOcclusion;
  float specularAO = specularAO(shading.NoV, diffuseAO, pixel.roughness);

#if USE_IRIDESCENCE
  vec3 F = mix(specularDFG(pixel), pixel.iridescenceFresnel, pixel.iridescenceMask);
#else
  vec3 F = specularDFG(pixel);
#endif

  vec3 Fd = pixel.diffuseColor * diffuseIrradiance(shading.N);
  Fd *= diffuseAO;

  vec3 Fr = specularIrradiance(shading.R, pixel.roughness) * F;   
  Fr *= specularAO * pixel.energyCompensation;

#if USE_IRIDESCENCE
  vec3 iridescenceAttenuation = vec3(1.0) - pixel.iridescenceFresnel * pixel.iridescenceMask;
  Fd *= iridescenceAttenuation;
  //Fr *= iridescenceAttenuation * iridescenceAttenuation;
#endif

#if USE_CLEAR_COAT
  float NoV_CC = abs(dot(shading.N_CC, shading.V)) + 1e-4;
  vec3 R_CC = reflect(-shading.V, shading.N_CC);
  
  float F_CC = fresnelClearCoat(NoV_CC) * pixel.clearCoat;
  float attenuation = 1.0 - F_CC;
  
  Fd *= attenuation;
  Fr *= attenuation * attenuation;
  Fr += specularIrradiance(R_CC, pixel.clearCoatRoughness) * specularAO * F_CC;
#endif

  return Fd + Fr;
}