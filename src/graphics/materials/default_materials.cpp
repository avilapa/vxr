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

#include "../../../include/graphics/materials/default_materials.h"

#include "../../../include/engine/engine.h"
#include "../../../include/graphics/ui.h"

namespace vxr
{

  Screen::Screen()
  {
    set_name("Screen");
    set_shaders("screen_standard.vert", "screen_standard.frag");

    set_num_textures(1);
    set_uniforms_enabled(false);
  }

  Unlit::Unlit()
  {
    set_name("Unlit");
    set_shaders("unlit.vert", "unlit.frag");

    set_num_textures(0);
    set_uniforms_enabled(true);
    set_uniforms_name("Unlit");
  }

  Unlit::Instance::Instance()
  {
    init("Unlit");
  }

  void Unlit::Instance::set_color(Color color)
  {
    uniforms_.unlit.color = color.rgba();
  }

  Color Unlit::Instance::color() const
  {
    return Color(uniforms_.unlit.color);
  }

  void Unlit::Instance::onGUI()
  {
    MaterialInstance::onGUI();
    ImGui::ColorEdit4("Color", (float*)& uniforms_.unlit.color);
  }

  Wireframe::Wireframe()
  {
    set_name("Wireframe");
    set_shaders("unlit.vert", "unlit.frag");

    set_num_textures(0);
    set_uniforms_enabled(true);
    set_uniforms_name("Unlit");
    set_render_mode(RenderMode::Wireframe);
  }

  Wireframe::Instance::Instance()
  {
    init("Wireframe");
  }

  void Wireframe::Instance::set_color(Color color)
  {
    uniforms_.unlit.color = color.rgba();
  }

  Color Wireframe::Instance::color() const
  {
    return Color(uniforms_.unlit.color);
  }

  void Wireframe::Instance::onGUI()
  {
    MaterialInstance::onGUI();
    ImGui::ColorEdit4("Color", (float*)& uniforms_.unlit.color);
  }

  Standard::Standard()
  {
    set_name("Standard");
    set_shaders("standard.vert", "standard.frag");

    set_num_textures(0);
    set_uniforms_enabled(true);
    set_uniforms_name("Standard");
  }

  Standard::Textured::Textured()
  {
    set_name("Standard Textured");
    set_shaders("standard.vert", "standard_tex.frag");

    set_num_textures(1);
    set_uniforms_enabled(false);
  }

  Standard::TexturedCubemap::TexturedCubemap()
  {
    set_name("Standard Textured Cubemap");
    set_shaders("standard.vert", "standard_texc.frag");

    set_num_textures(1);
    set_uniforms_enabled(false);
  }

  Standard::Instance::Instance()
  {
    init({ "Standard", "Standard Textured", "Standard Textured Cubemap" });
    color_texture_.alloc()->set_name("Color Texture");

    uniforms_.standard.color = vec4(1);
  }

  void Standard::Instance::onGUI()
  {
    MaterialInstance::onGUI();
    switch (active_material())
    {
    case 0:
      ImGui::ColorEdit4("Color", (float*)&uniforms_.standard.color);
      break;
    case 1:
      break;
    case 2:
      break;
    }
  }

  void Standard::Instance::set_color(Color color)
  {
    uniforms_.standard.color = color.rgba();
  }

  void Standard::Instance::set_color_texture(const char* file)
  {
    color_texture_->load(file);
    color_texture_->set_type(TextureType::T2D);
    set_active_material(1);
    set_texture(0, color_texture_);
  }

  void Standard::Instance::set_color_texture(const char* folder, const char* extension)
  {
    color_texture_->load(folder, extension);
    color_texture_->set_type(TextureType::CubeMap);
    set_active_material(2);
    set_texture(0, color_texture_);
  }

  void Standard::Instance::set_color_texture(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft)
  {
    color_texture_->load(rt, lf, up, dn, bk, ft);
    color_texture_->set_type(TextureType::CubeMap);
    set_active_material(2);
    set_texture(0, color_texture_);
  }

  Color Standard::Instance::color() const
  {
    return Color(uniforms_.standard.color);
  }

  ref_ptr<Texture> Standard::Instance::color_texture() const
  {
    return color_texture_;
  }

  Skybox::Skybox()
  {
    set_name("Skybox");
    set_shaders("skybox.vert", "skybox.frag");

    set_num_textures(1);
    set_uniforms_enabled(false);
    set_uniforms_name("Skybox");

    set_cull(Cull::Front);
  }

  Skybox::Instance::Instance()
  {
    init("Skybox");
    skybox_texture_.alloc()->set_name("Color Texture");
    skybox_texture_->set_type(TextureType::CubeMap);
  }

  void Skybox::Instance::onGUI()
  {
    MaterialInstance::onGUI();
  }

  void Skybox::Instance::set_color_texture(const char* folder, const char* extension)
  {
    skybox_texture_->load(folder, extension);
    set_texture(0, skybox_texture_);
  }

  void Skybox::Instance::set_color_texture(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft)
  {
    skybox_texture_->load(rt, lf, up, dn, bk, ft);
    set_texture(0, skybox_texture_);
  }

  ref_ptr<Texture> Skybox::Instance::color_texture() const
  {
    return skybox_texture_;
  }
}