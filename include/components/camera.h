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
#include "../graphics/material.h"
#include "../graphics/mesh.h"

/**
* \file camera.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Camera Component and Camera System classes. The Camera System performs internal render-to-texture procedures and controls Viewport size.
*
*/
namespace vxr 
{

  namespace System { class Camera; }

	class Camera : public Component
  {

    VXR_OBJECT(Camera, Component);

    friend class System::Camera;
	public:
    Camera();
		virtual ~Camera();

    virtual void onGUI() override;

#define PROPERTY(type, name, fname, ...) \
     private:\
      type name = __VA_ARGS__;\
     public:\
      void set_##fname(const type &c) { name = c; dirty_ = true; }\
      type fname() const { return name; }

    PROPERTY(float, fov_, fov, 70.0f);
    PROPERTY(float, aspect_, aspect, 1280.0f / 720.0f);
    PROPERTY(float, near_plane_, near_plane, 0.1f);
    PROPERTY(float, far_plane_, far_plane, 900.0f);

    PROPERTY(Color, background_color_, background_color, Color::PhyreBlue);

    mat4 projection();
    mat4 view();

    bool clear_color;
    bool clear_depth;
    bool clear_stencil;

#undef PROPERTY

  public:
    void computeTransformations();
    bool hasChanged();

  private:
    bool dirty_ = true;

    mat4 projection_;
    mat4 view_;
	};

  class Scene;

  namespace System 
  {
    class Camera : public ComponentSystem
    {

      VXR_OBJECT(System::Camera, ComponentSystem);

      friend class Renderer;
    public:
      Camera();
      virtual ~Camera();

      void set_main(ref_ptr<vxr::Camera> camera);
      ref_ptr<vxr::Camera> main();

      virtual void init() override;
      virtual void update() override;
      virtual void renderUpdate() override;
      virtual void renderPostUpdate() override;

      void set_render_to_screen(bool enabled);
      void set_render_size(uvec2 size);

      bool render_to_screen();
      uint32 screen_id();

    private:
      std::vector<ref_ptr<vxr::Camera>> components_;
      ref_ptr<Scene> scene_;
      ref_ptr<vxr::Camera> main_ = nullptr;
      bool render_to_screen_ = true;
      uvec2 render_size_{ 0, 0 };

      gpu::Framebuffer screen_;
      ref_ptr<Texture> screen_texture_;

      ref_ptr<Material> screen_material_;
      ref_ptr<EngineMesh::Quad> screen_quad_;

      struct CommonUniforms
      {
        gpu::Buffer buffer;
        Shader::CommonData data;
      } common_uniforms_;

    public:
      template<typename T> ref_ptr<T> createInstance()
      {
        ref_ptr<T> c;
        c.alloc();
        components_.push_back(c.get());
        return c.get();
      }
    };

    template<> class Getter<vxr::Camera>
    {
    public:
      static ref_ptr<System::Camera> get();
    };

  }

} /* end of vxr namespace */