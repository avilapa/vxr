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

#include "framebuffers.h"
#include "../../include/graphics/display_list.h"

#include "../../include/graphics/ui.h"

VXR_DEFINE_APP_MAIN(vxr::Main)

#define GLSL(...) "#version 330\n" #__VA_ARGS__

namespace vxr 
{
  static float vertex_data[] = {
    -0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,		// 0
    -0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,		// 1
     0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,		// 2
     0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,		// 3

    -0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 0.0f,		// 4
    -0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,		// 5
     0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,		// 6
     0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	 1.0f, 1.0f,		// 7

    -0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 0.0f,		// 8
    -0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 0.0f, 1.0f, 	  // 9
     0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,		// 10
     0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	 1.0f, 1.0f,		// 11

     0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,		// 12
     0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,		// 13
     0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,		// 14
     0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,		// 15

    -0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,		// 16
    -0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,		// 17
    -0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,		// 18
    -0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,		// 19

    -0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 0.0f, 0.0f,		// 20
    -0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 0.0f, 1.0f,		// 21
     0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 1.0f, 0.0f,		// 22
     0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	 1.0f, 1.0f		  // 23
  };

  static uint16 index_data[] = {
    1, 2, 0,
    1, 3, 2,
    5, 6, 4,
    5, 7, 6,
    9, 10, 8,
    9, 11, 10,
    13, 14, 12,
    13, 15, 14,
    17, 18, 16,
    17, 19, 18,
    21, 22, 20,
    21, 23, 22
  };

  float vertex_data_fb[] = {
    -1.0, -1.0, .0,     0.0, 0.0,
     1.0, -1.0, .0,     1.0, 0.0,
     1.0,  1.0, .0,     1.0, 1.0,
    -1.0,  1.0, .0,     0.0, 1.0
  };

  uint16 index_data_fb[] = {
    0, 2, 1,
    0, 3, 2
  };

  void Main::start()
  {
    window_title_ = (char*)malloc(512 + strlen("VXR Instancing Test"));

    DisplayList frame;
    {
      vec3 position = vec3(0.0f, 0.0f, -8.0f), forward = vec3(0.0f, 0.0f, 1.0f), up = vec3(0.0f, 1.0f, 0.0f);
      u_state_static_.view = glm::lookAt(position, position + forward, up);
      u_state_static_.proj = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.5f, 900.0f);

      gpu::Material::Info mat_info;
      mat_info.shader.vert = GLSL(
        in vec3 a_position;
      in vec3 a_normal;
      in vec2 a_uv;
      layout(std140) uniform UniformState_Static
      {
        mat4 view;
        mat4 proj;
      };
      layout(std140) uniform UniformState_Stream
      {
        mat4 model;
      };
      out vec3 c;
      out vec2 uv;
      void main()
      {
        gl_Position = proj * view * model * vec4(a_position, 1.0);
        c = (a_position + 1) * 0.5;
        uv = a_uv;
      }
      );
      mat_info.shader.frag = GLSL(
        in vec3 c;
      in vec2 uv;
      uniform sampler2D u_tex0;
      out vec4 fragColor;
      void main()
      {
        fragColor = vec4(c * texture(u_tex0, uv).rgb, 1.0);
      }
      );

      mat_info.attribs[0] = { "a_position", VertexFormat::Float3 };
      mat_info.attribs[1] = { "a_normal",   VertexFormat::Float3 };
      mat_info.attribs[2] = { "a_uv",       VertexFormat::Float2 };

      mat_info.textures[0] = TextureType::T2D;

      gpu::Texture::Info tex_info;
      tex_data_ = gpu::Texture::loadFromFile("../../assets/textures/image.tga", tex_info);
      tex_info.magnification_filter = SamplerFiltering::Nearest;
      tex_info.minification_filter = SamplerFiltering::Nearest;

      vertex_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Vertex,  sizeof(vertex_data), Usage::Static });
      index_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Index,   sizeof(index_data),  Usage::Static });

      static_uniform_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Uniform, sizeof(UniformState_Static), Usage::Static, "UniformState_Static" });
      stream_uniform_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Uniform, sizeof(UniformState_Stream), Usage::Stream, "UniformState_Stream" });

      material_ = Engine::ref().gpu()->createMaterial(mat_info);
      texture_ = Engine::ref().gpu()->createTexture(tex_info);

      frame.fillBufferCommand()
        .set_buffer(vertex_buffer_)
        .set_data(vertex_data)
        .set_size(sizeof(vertex_data));
      frame.fillBufferCommand()
        .set_buffer(index_buffer_)
        .set_data(index_data)
        .set_size(sizeof(index_data));
      frame.fillBufferCommand()
        .set_buffer(static_uniform_buffer_)
        .set_data(&u_state_static_)
        .set_size(sizeof(UniformState_Static));
      frame.fillTextureCommand()
        .set_texture(texture_)
        .set_data(tex_data_);
    }
    { // Quad Material & Object setup 
      vertex_buffer_fb_ = Engine::ref().gpu()->createBuffer({ BufferType::Vertex, sizeof(vertex_data_fb), Usage::Static });
      index_buffer_fb_ =  Engine::ref().gpu()->createBuffer({ BufferType::Index, sizeof(index_data_fb),   Usage::Static });

      contrast_uniform_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Uniform, sizeof(UniformState_Contrast), Usage::Static, "UniformState_Contrast" });

      gpu::Material::Info mat_info;
      mat_info.shader.vert = GLSL(
        in vec3 a_position;
        in vec2 a_uv;
        out vec2 uv;
        void main() 
        {
          gl_Position = vec4(a_position, 1.0);
          uv = a_uv;
        }
      );
      mat_info.shader.frag = GLSL(
        in vec2 uv;
        uniform sampler2D u_tex0;
        layout(std140) uniform UniformState_Contrast
        {
          float contrast;
          float brightness;
        };
        out vec4 fragColor;
        void main() 
        {
          vec4 color = texture(u_tex0, uv);
          color.xyz /= color.w;
          color.xyz = (color.xyz - 0.5f) * contrast + 0.5f;
          color.xyz += brightness;
          color.xyz *= color.w;
          fragColor = vec4(color.xyz, 1.0);
        }
      );
      mat_info.attribs[0] = { "a_position", VertexFormat::Float3 };
      mat_info.attribs[1] = { "a_uv", VertexFormat::Float2 };
      mat_info.textures[0] = TextureType::T2D;
      mat_info.cull = Cull::Disabled;
      material_fb_ = Engine::ref().gpu()->createMaterial(mat_info);

      frame.fillBufferCommand()
        .set_buffer(vertex_buffer_fb_)
        .set_data(vertex_data_fb)
        .set_size(sizeof(vertex_data_fb));
      frame.fillBufferCommand()
        .set_buffer(index_buffer_fb_)
        .set_data(index_data_fb)
        .set_size(sizeof(index_data_fb));
    }
    Engine::ref().submitDisplayList(std::move(frame));

    // Create Framebuffer (offscreen rendering)
    gpu::Texture::Info fb_tex_color; fb_tex_color.width = 1280, fb_tex_color.height = 720;
    gpu::Texture::Info fb_tex_depth; fb_tex_depth.width = 1280, fb_tex_depth.height = 720;
    fb_tex_color.format = TexelsFormat::RGBA_U8;
    fb_tex_depth.format = TexelsFormat::Depth_U16;

    framebuffer_ = Engine::ref().gpu()->createFramebuffer({ fb_tex_color, fb_tex_depth, 1 });

    for (uint32 x = 0; x < kNUM_CUBES_ROW; ++x)
    {
      for (uint32 y = 0; y < kNUM_CUBES_ROW; ++y)
      {
        uint32 i = x * kNUM_CUBES_ROW + y;
        mat4 model = glm::translate(vec3((x - (float)(kNUM_CUBES_ROW - 1) * 0.5f) * 1.5f, (y - (float)(kNUM_CUBES_ROW - 1) * 0.5f) * 1.5f, 0));
        u_state_stream_[i].model = model;
      }
    }
  }

  void Main::update()
  {
	  static double v = 0.0;
	  v += 0.003;
    u_state_contrast_.brightness = sin(u_state_contrast_.brightness * 50.0f * deltaTime() + v) + 2.0f * 0.2f;
    u_state_contrast_.contrast = sin(u_state_contrast_.contrast * 21.0f * deltaTime() + v) + 1.5f;
	
    for (uint32 x = 0; x < kNUM_CUBES_ROW; ++x)
    {
      for (uint32 y = 0; y < kNUM_CUBES_ROW; ++y)
      {
        uint32 i = x * kNUM_CUBES_ROW + y;

        mat4 model = u_state_stream_[i].model;
        mat4 rot_x = glm::rotate((x + 1) * 0.21f * deltaTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        mat4 rot_y = glm::rotate((y + 1) * 0.37f * deltaTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        u_state_stream_[i].model = model * rot_x * rot_y;
      }
    }

    Application::update();
  }

  void Main::renderUpdate()
  {
    sprintf(window_title_,
      "%s: %d FPS @ 1280 x 720",
      "VXR Instancing Test",
      fps());

    Engine::ref().window()->set_title(window_title_);

    DisplayList frame;
    frame.setupViewCommand()
      .set_viewport({ 0,0,1280,720 })
      .set_framebuffer(framebuffer_);
    frame.clearCommand()
      .set_color({ 0.1f, 0.1f, 0.1f, 1.0f });

    for (uint32 x = 0; x < kNUM_CUBES_ROW; ++x)
    {
      for (uint32 y = 0; y < kNUM_CUBES_ROW; ++y)
      {
        uint32 i = x * kNUM_CUBES_ROW + y;

		frame.fillBufferCommand()
			.set_buffer(stream_uniform_buffer_)
			.set_data(&u_state_stream_[i])
			.set_size(sizeof(UniformState_Stream));
        frame.setupMaterialCommand()
          .set_material(material_)
          .set_buffer(0, vertex_buffer_)
          .set_uniform_buffer(0, static_uniform_buffer_)
          .set_uniform_buffer(1, stream_uniform_buffer_)
          .set_texture(0, texture_);
        frame.renderCommand()
          .set_index_buffer(index_buffer_)
          .set_count(sizeof(index_data) / sizeof(uint16))
          .set_type(IndexFormat::UInt16);
      }
    }

    frame.setupViewCommand()
      .set_viewport({ 0,0,1280,720 });
    frame.clearCommand()
      .set_color({ 0.5f, 0.7f, 0.8f, 1.0f })
      .set_clear_color(true)
      .set_clear_depth(true);
    frame.fillBufferCommand()
      .set_buffer(contrast_uniform_buffer_)
      .set_data(&u_state_contrast_)
      .set_size(sizeof(UniformState_Contrast));
    frame.setupMaterialCommand()
      .set_material(material_fb_)
      .set_buffer(0, vertex_buffer_fb_)
      .set_texture(0, framebuffer_.color_texture())
      .set_uniform_buffer(0, contrast_uniform_buffer_);
    frame.renderCommand()
      .set_index_buffer(index_buffer_fb_)
      .set_count(sizeof(index_data_fb) / sizeof(uint16))
      .set_type(IndexFormat::UInt16);

    Engine::ref().submitDisplayList(std::move(frame));
    Engine::ref().submitUIFunction([this]() { ui::Test(); });
    Application::renderUpdate();
  }

  void Main::stop() 
  {
    if (tex_data_)
    {
      free(tex_data_);
      tex_data_ = nullptr;
    }
    Application::stop();
  }

} /* end of vxr namespace */