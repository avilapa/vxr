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

#include "instancing.h"
#include "../../include/graphics/display_list.h"

// 0. Define the entry point.
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

  void Main::start()
  {
    // 1. Create vertex and index buffer to render a cube.
    vertex_buffer_             = Engine::ref().gpu()->createBuffer({ BufferType::Vertex,  sizeof(vertex_data), Usage::Static });
    index_buffer_              = Engine::ref().gpu()->createBuffer({ BufferType::Index,   sizeof(index_data),  Usage::Static });
    // 2. Create instance data for positions and colors of each cube.
    instance_positions_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Vertex,  sizeof(instance_positions_), Usage::Dynamic });
    instance_colors_buffer_    = Engine::ref().gpu()->createBuffer({ BufferType::Vertex,  sizeof(instance_colors_),    Usage::Static });
    // 3. Create uniform buffer to store the static data of a cube.
    uniform_buffer_            = Engine::ref().gpu()->createBuffer({ BufferType::Uniform, sizeof(UniformState_Static), Usage::Static, "UniformState_Static" });

    // 4. Initialize shader data. The engine uses UBO for the objects, so the correct syntax to use custom uniforms can be found below. 
    gpu::Material::Info mat_info;
    mat_info.shader.vert = GLSL(
      in vec3 a_position;
      in vec3 a_normal;
      in vec2 a_uv;
      in vec3 a_instance_position;
      in vec3 a_instance_color;
      layout(std140) uniform UniformState_Static
      {
        mat4 model;
        mat4 view;
        mat4 proj;
      };
      out vec3 c;
      out vec2 uv;
      void main()
      {
        gl_Position = proj * view * model * vec4(a_position + a_instance_position, 1.0);
        c = a_instance_color;
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

    // 5. Initialize vertex attributes. This time, we'll pass three different buffers. One with positions normals and uvs data, another with the instance positions and a last one with instance colors.
    mat_info.attribs[0] = { "a_position",          VertexFormat::Float3 };
    mat_info.attribs[1] = { "a_normal",            VertexFormat::Float3 };
    mat_info.attribs[2] = { "a_uv",                VertexFormat::Float2 };
    mat_info.attribs[3] = { "a_instance_position", VertexFormat::Float3, 1, VertexStep::PerInstance };
    mat_info.attribs[4] = { "a_instance_color",    VertexFormat::Float3, 2, VertexStep::PerInstance };

    // 6. Add a texture to the material.
    mat_info.textures[0] = TextureType::T2D;

    // 7. Load the texture and create the asset.
    gpu::Texture::Info tex_info;
    tex_data_ = gpu::Texture::loadFromFile("../../assets/textures/image.tga", tex_info);
    tex_info.magnification_filter = SamplerFiltering::Nearest;
    tex_info.minification_filter = SamplerFiltering::Nearest;
    texture_ = Engine::ref().gpu()->createTexture(tex_info);

    // 8. Initialize instance data.
    for (int i = 0; i < kNUM_INSTANCES; ++i)
    {
      instance_colors_[i] = vec3(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f);
    }

    // 9. And finally create the material for rendering the cubes with the information.
    material_ = Engine::ref().gpu()->createMaterial(mat_info);

    // 10. Set camera view and perspective matrices of the uniform buffer.
    vec3 position = vec3(57.5f, 15.0f, -5.0f), forward = vec3(0.0f, -0.5f, 1.0f), up = vec3(0.0f, 1.0f, 1.0f);
    float fov = 70.0f;

    u_state_static_.model = mat4(1);
    u_state_static_.view = glm::lookAt(position, position + forward, up);
    u_state_static_.proj = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, 0.5f, 900.0f);

    // 11. Create a render command display list to fill the index and vertex buffers with the appropriate data once.
    DisplayList frame;
    frame.fillBufferCommand()
      .set_buffer(vertex_buffer_)
      .set_data(vertex_data)
      .set_size(sizeof(vertex_data));
    frame.fillBufferCommand()
      .set_buffer(index_buffer_)
      .set_data(index_data)
      .set_size(sizeof(index_data));
    // 12. Fill the instance colors buffer (we will be filling the positions in the update, as the buffer requires a dynamic update).
    frame.fillBufferCommand()
      .set_buffer(instance_colors_buffer_)
      .set_data(instance_colors_)
      .set_size(sizeof(instance_colors_));
    // 13. Fill the uniform data once.
    frame.fillBufferCommand()
      .set_buffer(uniform_buffer_)
      .set_data(&u_state_static_)
      .set_size(sizeof(UniformState_Static));
    // 14. Upload texture data to the GPU.
    frame.fillTextureCommand()
      .set_texture(texture_)
      .set_data(tex_data_);
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void Main::update()
  {
    // 15. Update instances position. This operations are performed in the update() instead of the renderUpdate() for them to take into account deltaTime() and be framerate independent.
    static float v = 0;
    for (int i = 0; i < kNUM_INSTANCES; ++i)
    {
      instance_positions_[i] =
      {
        (i / 500)*1.5f,
        750.0f*(float)(sin(i*3.1415 / 10 + v) * deltaTime()),
        (float)(i % 500)*1.5f
      };
    }
    v += 0.01f;

    Application::update();
  }

  void Main::renderUpdate()
  {
    DisplayList frame;
    frame.clearCommand()
      .set_color({ 0.1f, 0.1f, 0.1f, 1.0f });
    // 16. Re-upload instance position data to the GPU once each frame.
    frame.fillBufferCommand()
      .set_buffer(instance_positions_buffer_)
      .set_data(instance_positions_)
      .set_size(sizeof(instance_positions_));
    // 17. Setup material for rendering. This time, all the buffers and textures need to be set as parameters.
    frame.setupMaterialCommand()
      .set_material(material_)
      .set_buffer(0, vertex_buffer_)
      .set_buffer(1, instance_positions_buffer_)
      .set_buffer(2, instance_colors_buffer_)
      .set_uniform_buffer(0, uniform_buffer_)
      .set_texture(0, texture_);
    // 18. Add render command with the number of instances to be rendered.
    frame.renderCommand()
      .set_index_buffer(index_buffer_)
      .set_count(sizeof(index_data) / sizeof(uint16))
      .set_type(IndexFormat::UInt16)
      .set_instances(kNUM_INSTANCES);
    Engine::ref().submitDisplayList(std::move(frame));

    Application::renderUpdate();
  }

  void Main::stop() 
  {
    // 19. Free the texture data at the end of the application.
    if (tex_data_)
    {
      free(tex_data_);
      tex_data_ = nullptr;
    }
    Application::stop();
  }

} /* end of vxr namespace */