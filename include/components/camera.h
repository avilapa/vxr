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

#include "../core/component.h"
#include "../graphics/materials/shader.h"
#include "../graphics/gpu_resources.h"
#include "../graphics/composer.h"

/**
* \file camera.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Camera Component and Camera System classes. 
* 
* The Camera System performs internal render-to-texture procedures and controls Viewport size.
*
*/
namespace vxr 
{

  namespace System { class Camera; }

	class Camera : public Component
  {
    VXR_OBJECT(Camera, Component);
	public:
    Camera();
		~Camera();

    struct ClearFlags
    {
      enum Enum
      {
        SolidColor,
        Skybox,
      };
    };

    virtual void onGUI() override;

    void set_composer(ref_ptr<Composer> composer);
    ref_ptr<Composer> composer() const;

#define PROPERTY(type, name, fname, ...)                            \
     private:                                                       \
      type name = __VA_ARGS__;                                      \
     public:                                                        \
      void set_##fname(const type &c) { name = c; markForUpdate(); }\
      type fname() const { return name; }

    PROPERTY(float, fov_, fov, 50.0f);
    PROPERTY(float, aspect_, aspect, 1280.0f / 720.0f);
    PROPERTY(float, near_plane_, near_plane, 0.1f);
    PROPERTY(float, far_plane_, far_plane, 900.0f);

    PROPERTY(Color, background_color_, background_color, Color::PhyreBlue);

    /// TODO: Remove dirty update.
    PROPERTY(bool, clear_color_, clear_color, true);
    PROPERTY(bool, clear_depth_, clear_depth, true);
    PROPERTY(bool, clear_stencil_, clear_stencil, true);
#undef PROPERTY

    void set_clear_flags(ClearFlags::Enum flags);
    ClearFlags::Enum clear_flags() const;

    mat4 projection() const;
    mat4 view() const;

    static mat4 BuildProjection(float fovy_degrees, float aspect, float z_near, float z_far);
    static mat4 BuildView(vec3 eye, vec3 center, vec3 up);

  public:
    void computeTransformations();
    void markForUpdate();
    bool hasChanged() const;

  private:
    bool dirty_ = true;     
    ClearFlags::Enum clear_flags_ = ClearFlags::SolidColor;

    ref_ptr<Composer> composer_;

    mat4 projection_;
    mat4 view_;
	};

  namespace mat { class MaterialInstance; }

  namespace System 
  {
    class Camera : public ComponentSystem
    {
      VXR_COMPONENT_SYSTEM(Camera, ComponentSystem);
    public:
      Camera();
      ~Camera();

      void set_main(ref_ptr<vxr::Camera> camera);
      ref_ptr<vxr::Camera> main() const;

      void init() override;
      void start() override;
      void renderPreUpdate() override;
      void renderUpdate() override;
      void renderPostUpdate() override;

      void onSceneChanged() override;

      void set_render_to_screen(bool enabled);
      bool render_to_screen() const;
      uint32 screen_texture_id();

      gpu::Buffer common_uniforms_buffer() const;

    private:
      ref_ptr<vxr::Camera> main_;
      
      bool render_to_screen_;

      struct CommonUniforms
      {
        gpu::Buffer buffer;
        Shader::CommonData data;
      } common_uniforms_;
    };

    template<> class Getter<vxr::Camera>
    {
    public:
      static ref_ptr<System::Camera> get();
    };

  }

} /* end of vxr namespace */