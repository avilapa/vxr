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

#include "../../../include/graphics/materials/standard.h"
#include "../../../include/core/assets.h"
#include "../../../include/engine/engine.h"
#include "../../../include/components/ibl.h"

namespace vxr
{

  namespace mat
  {

    ScreenMaterial::ScreenMaterial()
    {
      set_name("Screen Material");
      set_shaders("screen_standard.vert", "screen_standard.frag");

      set_num_textures(1);
      set_uniforms_enabled(false);
    }

    Std::Std()
    {
      set_name("Std");
      set_shaders("standard.vert", "standard.frag");

      //set_blend_params(true, vec4(0), BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha, BlendOp::Add, BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha, BlendOp::Add);
      
      set_num_textures(11);
      set_uniforms_enabled(true);
      set_uniforms_name("Standard");

      set_common_texture(0, Engine::ref().assetManager()->default_cubemap()); // Irradiance
      set_common_texture(1, Engine::ref().assetManager()->default_cubemap()); // Prefiltered
      set_common_texture(2, Engine::ref().assetManager()->default_texture_white()); // BRDF LUT

      Engine::ref().ibl()->subscribeMaterialForIBLTextures(name());
    }

    Std::Instance::Instance()
    {
      init("Std");

      set_texture(3, Engine::ref().assetManager()->default_texture_white()); // Albedo
      set_texture(4, Engine::ref().assetManager()->default_texture_white()); // Metallic
      set_texture(5, Engine::ref().assetManager()->default_texture_white()); // Roughmess
      set_texture(6, Engine::ref().assetManager()->default_texture_white()); // Ambient Occlusion
      set_texture(7, Engine::ref().assetManager()->default_texture_normal()); // Normal
      set_texture(8, Engine::ref().assetManager()->default_texture_normal()); // Clear Coat Normal
      set_texture(9, Engine::ref().assetManager()->default_texture_white()); // Emissive
      set_texture(10, Engine::ref().assetManager()->default_texture_white()); // Iridiscence Thickness

      uniforms_.standard.albedo = vec4(1);
      uniforms_.standard.emissive = vec4(0);
      uniforms_.standard.metallic_roughness_reflectance_ao = vec4(1.0f, 1.0f, 0.5f, 1.0f);
      uniforms_.standard.clear_coat_clear_coat_roughness = vec4(1.0f, 0.0f, 0, 0);
      uniforms_.standard.iridescence_mask_thickness_ior_k = vec4(0.0f, 0.5f, 1.33f, 0.33f);
    }

    void Std::Instance::onGUI()
    {
      MaterialInstance::onGUI();
      switch (active_material())
      {
      case 0:
        ImGui::Spacing();
        ImGui::Text("Base Layer:");
        ImGui::ColorEdit4(uiText("Albedo##Base").c_str(), (float*)&uniforms_.standard.albedo);
        ImGui::ColorEdit4(uiText("Emissive##Base").c_str(), (float*)&uniforms_.standard.emissive);
        ImGui::SliderFloat(uiText("Metallic##Base").c_str(), &uniforms_.standard.metallic_roughness_reflectance_ao.x, 0.0f, 1.0f);
        ImGui::SliderFloat(uiText("Roughness##Base").c_str(), &uniforms_.standard.metallic_roughness_reflectance_ao.y, 0.0f, 1.0f);
        ImGui::SliderFloat(uiText("Reflectance##Base").c_str(), &uniforms_.standard.metallic_roughness_reflectance_ao.z, 0.0f, 1.0f);
        ImGui::SliderFloat(uiText("Ambient Occlusion##Base").c_str(), &uniforms_.standard.metallic_roughness_reflectance_ao.w, 0.0f, 1.0f);
        ImGui::Spacing();
        ImGui::Text("Thin Film Layer:");
        ImGui::SliderFloat(uiText("Iridescence##ThinFilm").c_str(), &uniforms_.standard.iridescence_mask_thickness_ior_k.x, 0.0f, 1.0f);
        ImGui::SliderFloat(uiText("Thickness##ThinFilm").c_str(), &uniforms_.standard.iridescence_mask_thickness_ior_k.y, 0.0f, 1.0f);
        ImGui::SliderFloat(uiText("Base IOR##ThinFilm").c_str(), &uniforms_.standard.iridescence_mask_thickness_ior_k.z, 1.0f, 5.0f);
        ImGui::SliderFloat(uiText("Extinction k##ThinFilm").c_str(), &uniforms_.standard.iridescence_mask_thickness_ior_k.w, 0.0, 5.0f);
        ImGui::Spacing();
        ImGui::Text("External Layer (Coating):");
        ImGui::SliderFloat(uiText("Clear Coat##Coating").c_str(), &uniforms_.standard.clear_coat_clear_coat_roughness.x, 0.0f, 1.0f);
        ImGui::SliderFloat(uiText("Roughness##Coating").c_str(), &uniforms_.standard.clear_coat_clear_coat_roughness.y, 0.0f, 1.0f);
        ImGui::Spacing();
        break;
      case 1:
        break;
      case 2:
        break;
      }
    }

    void Std::Instance::set_albedo(Color color)
    {
      uniforms_.standard.albedo = color.rgba();
    }

    void Std::Instance::set_albedo(ref_ptr<Texture> texture)
    {
      set_texture(3, texture);
    }

    void Std::Instance::set_metallic(float value)
    {
      uniforms_.standard.metallic_roughness_reflectance_ao.x = value;
    }

    void Std::Instance::set_metallic(ref_ptr<Texture> texture)
    {
      set_texture(4, texture);
    }

    void Std::Instance::set_roughness(float value)
    {
      uniforms_.standard.metallic_roughness_reflectance_ao.y = value;
    }

    void Std::Instance::set_roughness(ref_ptr<Texture> texture)
    {
      set_texture(5, texture);
    }

    void Std::Instance::set_reflectance(float value)
    {
      uniforms_.standard.metallic_roughness_reflectance_ao.z = value;
    }

    void Std::Instance::set_ambient_occlusion(float value)
    {
      uniforms_.standard.metallic_roughness_reflectance_ao.w = value;
    }

    void Std::Instance::set_ambient_occlusion(ref_ptr<Texture> texture)
    {
      set_texture(6, texture);
    }

    void Std::Instance::set_normal(ref_ptr<Texture> texture)
    {
      set_texture(7, texture);
    }

    void Std::Instance::set_iridescence(float value)
    {
      uniforms_.standard.iridescence_mask_thickness_ior_k.x = value;
    }

    void Std::Instance::set_iridescence_thickness(float value)
    {
      uniforms_.standard.iridescence_mask_thickness_ior_k.y = value;
    }

    void Std::Instance::set_iridescence_thickness(ref_ptr<Texture> texture)
    {
      set_texture(10, texture);
    }

    void Std::Instance::set_iridescence_base_ior(float value)
    {
      uniforms_.standard.iridescence_mask_thickness_ior_k.z = value;
    }

    void Std::Instance::set_iridescence_extinction_k(float value)
    {
      uniforms_.standard.iridescence_mask_thickness_ior_k.w = value;
    }

    void Std::Instance::set_clear_coat(float value)
    {
      uniforms_.standard.clear_coat_clear_coat_roughness.x = value;
    }

    void Std::Instance::set_clear_coat_roughness(float value)
    {
      uniforms_.standard.clear_coat_clear_coat_roughness.y = value;
    }

    void Std::Instance::set_clear_coat_normal(ref_ptr<Texture> texture)
    {
      set_texture(8, texture);
    }

  }

}