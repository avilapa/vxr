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

#include "../../include/engine/engine.h"
#include "../../include/graphics/ui.h"

namespace vxr 
{

  Camera::Camera()
  {
    set_name("Camera");
    aspect_ = (float)Engine::ref().window()->params().size.x / (float)Engine::ref().window()->params().size.y; ///TODO: If resolution were to be editable this would need updating.
  }

  Camera::~Camera()
  {

  }

  void Camera::onGUI()
  {
    ImGui::Spacing();
    ImGui::Text("Field of View"); ImGui::SameLine();
    if (ImGui::DragFloat(uiText("##Fov").c_str(), &fov_, 0.01f, -FLT_MAX, FLT_MAX)) dirty_ = true;
    ImGui::Text("Aspect Ratio "); ImGui::SameLine();
    if (ImGui::DragFloat(uiText("##Aspect").c_str(), &aspect_, 0.01f, -FLT_MAX, FLT_MAX)) dirty_ = true;
    ImGui::Text("Near Plane   "); ImGui::SameLine();
    if (ImGui::DragFloat(uiText("##NearPlane").c_str(), &near_plane_, 0.01f, -FLT_MAX, FLT_MAX)) dirty_ = true;
    ImGui::Text("Far Plane    "); ImGui::SameLine();
    if (ImGui::DragFloat(uiText("##FarPlane").c_str(), &far_plane_, 0.01f, -FLT_MAX, FLT_MAX)) dirty_ = true;
    ImGui::Text("Clear Color  "); ImGui::SameLine();
    ImGui::ColorEdit3(uiText("##Color").c_str(), (float*)&background_color_);
  }

  void Camera::computeTransformations()
  {
    VXR_TRACE_SCOPE("VXR", "Compute Transformations");
    projection_ = glm::perspective(glm::radians(fov_), aspect_, near_plane_, far_plane_);
    view_ = glm::lookAt(transform()->local_position(), transform()->local_position() + transform()->forward(), transform()->up()); /// Will I need world position?
    dirty_ = false;
  }

  bool Camera::hasChanged()
  {
    return dirty_;
  }

  mat4 Camera::projection()
  {
    return projection_;
  }

  mat4 Camera::view()
  {
    return view_;
  }

  System::Camera::Camera()
  {
    screen_texture_.alloc();
    screen_material_.alloc();
    screen_quad_.alloc();
  }

  System::Camera::~Camera()
  {

  }

  void System::Camera::set_main(ref_ptr<vxr::Camera> camera)
  {
    if (main_ != camera)
    {
      main_ = camera;
      main_->dirty_ = true;
    }
  }

  ref_ptr<Camera> System::Camera::main()
  {
    if (!main_)
    {
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not find any camera. Returning NULL.\n");
      return nullptr;
    }
    return main_;
  }

  void System::Camera::init()
  {
    common_uniforms_.buffer = Engine::ref().gpu()->createBuffer({ BufferType::Uniform,  sizeof(common_uniforms_.data), Usage::Static, "Common" });
    
    gpu::Texture::Info fb_tex_color; fb_tex_color.width = Engine::ref().window()->params().size.x, fb_tex_color.height = Engine::ref().window()->params().size.y;
    gpu::Texture::Info fb_tex_depth; fb_tex_depth.width = Engine::ref().window()->params().size.x, fb_tex_depth.height = Engine::ref().window()->params().size.y;
    fb_tex_color.format = TexelsFormat::RGBA_U8;
    fb_tex_depth.format = TexelsFormat::Depth_U16;

    render_size_ = { Engine::ref().window()->params().size.x, fb_tex_color.height = Engine::ref().window()->params().size.y };

    screen_ = Engine::ref().gpu()->createFramebuffer({ fb_tex_color, fb_tex_depth, 1 });
    screen_texture_->init(screen_.color_texture());

    screen_material_->addTexture(screen_texture_);
    screen_material_->set_shaders("../../assets/shaders/glsl/screen_standard.vert", "../../assets/shaders/glsl/screen_standard.frag");

    screen_material_->setupTextures();
    screen_material_->setup();

    screen_quad_->setup();
  }

  void System::Camera::update()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
      set_main(Engine::ref().scene()->default_camera());
      // Scene changed
    }
  }

  void System::Camera::renderUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Camera Render Update");
    if (!main_ || !scene_)
    {
      ///VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[WARNING]: Could not find any camera.\n"); /// This causes spam when using lower level API.
      return;
    }
    DisplayList frame;
    if (main()->hasChanged() || main()->transform()->hasChanged())
    {
      main()->computeTransformations();

      common_uniforms_.data.resolution = Engine::ref().window()->params().size;
      common_uniforms_.data.u_clear_color = main()->background_color().rgba();
      common_uniforms_.data.u_proj = main()->projection();
      common_uniforms_.data.u_view = main()->view();

      frame.fillBufferCommand()
        .set_buffer(common_uniforms_.buffer)
        .set_data(&common_uniforms_.data)
        .set_size(sizeof(common_uniforms_.data));
    }

    frame.setupViewCommand() /// Should this go in Composer?
      .set_viewport({ 0,0, (uint16)(render_size_.x), (uint16)render_size_.y })
      .set_framebuffer(screen_)
      .set_resolution(render_size_);
    frame.clearCommand()
      .set_color(main()->background_color().rgba())
      .set_clear_color(main()->clear_color)
      .set_clear_depth(main()->clear_depth)
      .set_clear_stencil(main()->clear_stencil);
    /// TODO: missing depth and stencil
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void System::Camera::renderPostUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "Camera Render Post Update");
    if (!main_ || !scene_)
    {
      ///VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[WARNING]: Could not find any camera.\n"); /// This causes spam when using lower level API.
      return;
    }

    for (auto &c : components_)
    {
      c->dirty_ = false;
    }
    DisplayList frame;
    frame.setupViewCommand()
      .set_viewport({ 0,0, (uint16)common_uniforms_.data.resolution.x, (uint16)common_uniforms_.data.resolution.y });
    
    if (render_to_screen_)
    {
      frame.clearCommand()
        .set_color(Color::Black)
        .set_clear_color(true)
        .set_clear_depth(true);
      frame.setupMaterialCommand()
        .set_material(screen_material_->gpu_.mat)
        .set_buffer(0, screen_quad_->gpu_.vertex.buffer)
        .set_v_texture(screen_material_->gpu_.tex);
      frame.renderCommand()
        .set_index_buffer(screen_quad_->gpu_.index.buffer)
        .set_count(screen_quad_->indexCount())
        .set_type(screen_quad_->indexFormat());
    }

    Engine::ref().submitDisplayList(std::move(frame));
  }

  uint32 System::Camera::screen_id()
  {
    return screen_texture_->id();
  }

  void System::Camera::set_render_to_screen(bool enabled)
  {
    render_to_screen_ = enabled;
  }

  void System::Camera::set_render_size(uvec2 render_size)
  {
    render_size_ = render_size;
  }

  bool System::Camera::render_to_screen()
  {
    return render_to_screen_;
  }

  ref_ptr<System::Camera> System::Getter<Camera>::get()
  {
    return Engine::ref().camera();
  }
}