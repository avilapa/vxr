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

#include "../../include/components/camera.h"
#include "../../include/components/transform.h"
#include "../../include/components/light.h"

#include "../../include/engine/engine.h"
#include "../../include/engine/gpu.h"
#include "../../include/graphics/materials/material_instance.h"
#include "../../include/graphics/materials/material.h"
#include "../../include/graphics/composer.h"
#include "../../include/graphics/window.h"
#include "../../include/core/scene.h"
#include "../../include/core/assets.h"

namespace vxr 
{

  Camera::Camera()
  {
    set_name("Camera");
    composer_ = Engine::ref().assetManager()->default_camera_composer();
    ///TODO: If resolution was to be editable this would need updating.
    aspect_ = (float)Engine::ref().window()->params().size.x / (float)Engine::ref().window()->params().size.y;
  }

  Camera::~Camera()
  {
  }

  void Camera::onGUI()
  {
    ImGui::Spacing();
    ImGui::Combo(uiText("Clear Flags##ClearFlags").c_str(), (int*)&clear_flags_, "Solid Color\0Skybox\0\0");
    ImGui::Spacing();
    switch (clear_flags_)
    {
    case ClearFlags::SolidColor:
      ImGui::Text("Color      "); ImGui::SameLine();
      ImGui::ColorEdit3(uiText("##Color").c_str(), (float*)&background_color_);
      break;
    case ClearFlags::Skybox:
      if (!Engine::ref().scene()->skybox())
      {
        ImGui::PushStyleColor(0, ImVec4(0.8f, 0.3f, 0.12f, 1.0f));
        ImGui::Text("No skybox has been created.");
        ImGui::PopStyleColor();
      }
      else
      {
        Engine::ref().scene()->skybox()->material()->onGUI();
      }
      break;
    default:
      break;
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Clear Color"); ImGui::SameLine();
    ImGui::Checkbox(uiText("##ClearSettingsColor").c_str(), &clear_color_);
    ImGui::Text("Clear Depth"); ImGui::SameLine();
    ImGui::Checkbox(uiText("##ClearSettingsDepth").c_str(), &clear_depth_);
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("FOV        "); ImGui::SameLine();
    if (ImGui::DragFloat(uiText("##Fov").c_str(), &fov_, 0.1f, -FLT_MAX, FLT_MAX)) dirty_ = true;
    ImGui::Text("Near Plane "); ImGui::SameLine();
    if (ImGui::DragFloat(uiText("##NearPlane").c_str(), &near_plane_, 0.01f, -FLT_MAX, FLT_MAX)) dirty_ = true;
    ImGui::Text("Far Plane  "); ImGui::SameLine();
    if (ImGui::DragFloat(uiText("##FarPlane").c_str(), &far_plane_, 0.01f, -FLT_MAX, FLT_MAX)) dirty_ = true;

    ImGui::Spacing();
    ImGui::Text("Is main camera of current scene:"); ImGui::SameLine();
    bool is_main = (this == Engine::ref().camera()->main().get());
    if (is_main)
    {
      ImGui::PushStyleColor(0, ImVec4(0.12f, 0.9f, 0.3f, 1.0f));
    }
    else
    {
      ImGui::PushStyleColor(0, ImVec4(0.8f, 0.3f, 0.12f, 1.0f));
    }
    ImGui::Text(((is_main) ? "YES" : "NO"));
    ImGui::PopStyleColor();
    if (ImGui::Button(uiText("Make Main##MakeMain").c_str()))
    {
      Engine::ref().camera()->set_main(this);
    }
  }

  void Camera::computeTransformations()
  {
    VXR_TRACE_SCOPE("VXR", "Compute Transformations");
    projection_ = glm::perspective(glm::radians(fov_), aspect_, near_plane_, far_plane_);
    view_ = glm::lookAt(transform()->world_position(), transform()->world_position() + transform()->world_forward(), transform()->world_up());
    dirty_ = false;
  }

  void Camera::markForUpdate()
  {
    dirty_ = true;
  }

  bool Camera::hasChanged() const
  {
    return dirty_;
  }

  void Camera::set_clear_flags(ClearFlags::Enum flags)
  {
    clear_flags_ = flags;
  }

  Camera::ClearFlags::Enum Camera::clear_flags() const
  {
    return clear_flags_;
  }

  void Camera::set_composer(ref_ptr<Composer> composer)
  {
    composer_ = composer;
  }

  ref_ptr<Composer> Camera::composer() const
  {
    return composer_;
  }

  mat4 Camera::projection() const
  {
    return projection_;
  }

  mat4 Camera::view() const
  {
    return view_;
  }

  mat4 Camera::BuildProjection(float fovy_degrees, float aspect, float z_near, float z_far)
  {
    return glm::perspective(glm::radians(fovy_degrees), aspect, z_near, z_far);
  }

  mat4 Camera::BuildView(vec3 eye, vec3 center, vec3 up)
  {
    return glm::lookAt(eye, center, up);
  }

  System::Camera::Camera() :
    main_(nullptr),
    render_to_screen_(true)
  {
  }

  System::Camera::~Camera()
  {
  }

  void System::Camera::set_main(ref_ptr<vxr::Camera> camera)
  {
    if (!camera || !scene_)
    {
      main_ = nullptr;
      return;
    }

    if (main_ != camera)
    {
      if (camera->gameObject()->scene_id() != scene_->id())
      {
        return;
      }
      main_ = camera;
      main_->markForUpdate();
      main_->composer()->init();
    }
  }

  ref_ptr<Camera> System::Camera::main() const
  {
    return main_;
  }

  void System::Camera::init()
  {
    common_uniforms_.buffer = Engine::ref().gpu()->createBuffer({ BufferType::Uniform,  sizeof(common_uniforms_.data), Usage::Static, "Common" });
  }

  void System::Camera::onSceneChanged()
  {
    ComponentSystem::onSceneChanged();
    set_main(nullptr);
  }

  void System::Camera::start()
  {
    set_main(scene_->default_camera());
    if (!main_)
    {
      set_main(scene_->findCameraInScene());
    }
  }

  void System::Camera::renderPreUpdate()
  {
    if (!main_)
    {
      // Dummy commands to keep the engine running.
      DisplayList frame;
      frame.setupViewCommand();
      frame.clearCommand();
      Engine::ref().submitDisplayList(std::move(frame));
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [CAMERA] No camera instantiated in the scene.\n");
      return;
    }
  }

  void System::Camera::renderUpdate()
  {
    if (!main_)
    {
      return;
    }

    VXR_TRACE_SCOPE("VXR", "Camera Render Update");
    main_->composer()->setupFirstPass();

    DisplayList frame;
    if (main_->hasChanged() || main_->transform()->hasChanged())
    {
      main_->computeTransformations();
    }

    common_uniforms_.data.u_proj = main_->projection();
    common_uniforms_.data.u_view = main_->view();
    common_uniforms_.data.u_resolution = Engine::ref().window()->params().size;
    /// TODO: Fill xy.
    common_uniforms_.data.u_clear_color = main_->background_color().rgba();
    common_uniforms_.data.u_view_pos_num_lights = vec4(main_->transform()->world_position(), (float)Engine::ref().light()->num_lights());

    /// TODO: Move this into a different constant update buffer OR update this every frame as doing now.
    common_uniforms_.data.u_time = vec4(Engine::ref().window()->uptime(), 0, 0, 0);

    frame.fillBufferCommand()
      .set_buffer(common_uniforms_.buffer)
      .set_data(&common_uniforms_.data)
      .set_size(sizeof(common_uniforms_.data));

    frame.clearCommand()
      .set_color(main()->background_color().rgba())
      .set_clear_color(main()->clear_color())
      .set_clear_depth(main()->clear_depth())
      .set_clear_stencil(main()->clear_stencil());
    /// TODO: missing depth and stencil
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void System::Camera::renderPostUpdate()
  {
    if (!main_)
    {
      return;
    }

    VXR_TRACE_SCOPE("VXR", "Camera Render Post Update");

    main()->composer()->applyPostprocessing();
    main()->composer()->setupLastPass();

    if (render_to_screen_)
    {
      main()->composer()->renderToScreen();
    }
  }

  uint32 System::Camera::screen_texture_id()
  {
    return main()->composer()->final_texture_id();
  }

  void System::Camera::set_render_to_screen(bool enabled)
  {
    render_to_screen_ = enabled;
  }

  bool System::Camera::render_to_screen() const
  {
    return render_to_screen_;
  }

  gpu::Buffer System::Camera::common_uniforms_buffer() const
  {
    return common_uniforms_.buffer;
  }

  ref_ptr<System::Camera> System::Getter<Camera>::get()
  {
    return Engine::ref().camera();
  }
}