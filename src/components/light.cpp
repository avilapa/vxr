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

#include "../../include/components/light.h"

#include "../../include/engine/engine.h"
#include "../../include/engine/gpu.h"
#include "../../include/core/gameobject.h"
#include "../../include/core/scene.h"

namespace vxr 
{

  Light::Light()
  {
    set_name("Light");
    contributes_ = false;

    type_ = Type::Punctual;
    color_ = Color::White;
    intensity_ = 1.0f;
    ambient_ = 0.15f;
    falloff_ = 1.0f;
    cone_angle_ = 180.0f;
  }

  Light::~Light()
  {
  }

  void Light::onGUI()
  {
    ImGui::Spacing();
    ImGui::Text("Is contributing to current scene:"); ImGui::SameLine();
    if (contributes_)
    {
      ImGui::PushStyleColor(0, ImVec4(0.12f, 0.9f, 0.3f, 1.0f));
    }
    else
    {
      ImGui::PushStyleColor(0, ImVec4(0.8f, 0.3f, 0.12f, 1.0f));
    }
    ImGui::Text(((contributes_) ? "YES" : "NO"));
    ImGui::PopStyleColor();
    ImGui::Spacing();
    ImGui::Text("Type         "); ImGui::SameLine();
    ImGui::Combo(uiText("##Type").c_str(), (int*)&type_, "Punctual\0Directional\0\0");
    ImGui::Spacing();
    ImGui::Text("Ambient      "); ImGui::SameLine();
    ImGui::DragFloat(uiText("##Ambient").c_str(), &ambient_, 0.01f, -FLT_MAX, FLT_MAX);
    ImGui::Text("Intensity    "); ImGui::SameLine();
    ImGui::DragFloat(uiText("##Intensity").c_str(), &intensity_, 0.01f, -FLT_MAX, FLT_MAX);
    ImGui::Spacing();
    switch (type_)
    {
    case Type::Directional:
      break;
    case Type::Punctual:
      ImGui::Text("Falloff      "); ImGui::SameLine();
      ImGui::DragFloat(uiText("##Falloff").c_str(), &falloff_, 0.01f, -FLT_MAX, FLT_MAX);
      ImGui::Text("Cone Angle   "); ImGui::SameLine();
      ImGui::DragFloat(uiText("##Cone Angle").c_str(), &cone_angle_, 1.0f, -FLT_MAX, FLT_MAX);
      break;
    }
    ImGui::Spacing();
    ImGui::Text("Light Color  "); ImGui::SameLine();
    ImGui::ColorEdit3(uiText("##Color").c_str(), (float*)&color_);
  }

  void Light::set_type(Type::Enum type)
  {
    type_ = type;
  }

  void Light::set_color(const Color& color)
  {
    color_ = color;
  }

  void Light::set_intensity(const float& intensity)
  {
    intensity_ = intensity;
  }

  void Light::set_ambient(const float& ambient)
  {
    ambient_ = ambient;
  }

  void Light::set_falloff(const float& falloff)
  {
    falloff_ = falloff;
  }

  System::Light::Light()
  {
    num_lights_ = 0;
  }

  System::Light::~Light()
  {

  }

  void System::Light::init()
  {
    light_uniforms_.buffer = Engine::ref().gpu()->createBuffer({ BufferType::Uniform,  
      sizeof(light_uniforms_.data), 
      Usage::Static, 
      "Lights" });
  }

  void System::Light::update()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
      // Scene changed
    }
  }

  void System::Light::renderUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Light Render Update");
    if (!scene_)
    {
      return;
    }

    num_lights_ = 0;
    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id() || !c->gameObject()->active())
      {
        c->contributes_ = false;
        continue;
      }

      if (num_lights_ < kMaxLightSources)
      {
        c->contributes_ = true;
        /*if (c->hasChanged())
        {
          c->computeTransformations();
        }*/

        light_uniforms_.data.position_falloff[num_lights_] = vec4(c->transform()->world_position(), (c->type_ == vxr::Light::Type::Directional) ? 0.0f : c->falloff_);
        light_uniforms_.data.color_intensity[num_lights_] = vec4(c->color_.rgb(), c->intensity_);
        light_uniforms_.data.direction_ambient[num_lights_] = vec4(c->transform()->world_rotation_angles(), c->ambient_);

        num_lights_++;
      }

    }

    DisplayList frame;
    frame.fillBufferCommand()
      .set_buffer(light_uniforms_.buffer)
      .set_data(&light_uniforms_.data)
      .set_size(sizeof(light_uniforms_.data));
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void System::Light::renderPostUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Light Render Post Update");
    if (!scene_)
    {
      return;
    }

    /*for (auto &c : components_)
    {
      //c->dirty_ = false;
    }*/
  }

  uint32 System::Light::num_lights() const
  {
    return num_lights_;
  }

  gpu::Buffer System::Light::light_uniforms_buffer() const
  {
    return light_uniforms_.buffer;
  }

  ref_ptr<System::Light> System::Getter<Light>::get()
  {
    return Engine::ref().light();
  }
}