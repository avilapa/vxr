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

layout(std140) uniform Standard
{
  	vec4 albedo;
    vec4 emissive;
    vec4 metallic_roughness_reflectance_ao;
    vec4 clear_coat_clear_coat_roughness;
    vec4 iridescence_mask_thickness_ior_k;
};

void main()
{
    MaterialInput inputs = initMaterial();

    inputs.baseColor.xyz = sRGBtoLinear(texture(u_tex2d3, getUV()).xyz);
    inputs.baseColor.xyz *= albedo.xyz;
    inputs.baseColor.w = albedo.w;
#if MAT_HAS_EMISSIVE
    inputs.emissive.xyz = sRGBtoLinear(texture(u_tex2d9, getUV()).xyz);
    inputs.emissive.xyz *= emissive.xyz;
#endif
    inputs.metallic = texture(u_tex2d4, getUV()).x;
    inputs.metallic *= metallic_roughness_reflectance_ao.x;

    inputs.roughness = texture(u_tex2d5, getUV()).x;
    inputs.roughness *= metallic_roughness_reflectance_ao.y;

    inputs.reflectance = metallic_roughness_reflectance_ao.z;

    inputs.ambientOcclusion = texture(u_tex2d6, getUV()).x;
    inputs.ambientOcclusion *= metallic_roughness_reflectance_ao.w;

#if MAT_HAS_NORMAL_MAP
    inputs.normal = texture(u_tex2d7, getUV()).xyz * 2.0 - 1.0;
#endif

#if MAT_HAS_CLEAR_COAT
    inputs.clearCoat = clear_coat_clear_coat_roughness.x;
    inputs.clearCoatRoughness = clear_coat_clear_coat_roughness.y;
#if MAT_HAS_CLEAR_COAT_NORMAL_MAP
    inputs.clearCoatNormal = texture(u_tex2d8, getUV()).xyz * 2.0 - 1.0;
#endif
#endif
#if MAT_HAS_IRIDESCENCE
    inputs.iridescenceMask = iridescence_mask_thickness_ior_k.x;
    inputs.filmThickness = iridescence_mask_thickness_ior_k.y * texture(u_tex2d10, getUV()).r;
    inputs.baseIor = iridescence_mask_thickness_ior_k.z;
    inputs.kExtinction = iridescence_mask_thickness_ior_k.w;
#endif
    setFragmentColor(evaluateMaterial(inputs));
}