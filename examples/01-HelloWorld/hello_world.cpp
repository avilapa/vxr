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

#include "hello_world.h"
#include "../../include/graphics/display_list.h"

VXR_DEFINE_APP_MAIN(vxr::Main)

#define GLSL(...) "#version 330\n" #__VA_ARGS__

namespace vxr 
{
  static float vertex_data[] = {
   -0.5, -0.5, 0.0,  1.0,  0.0,  0.0,
    0.5, -0.5, 0.0,  0.0,  1.0,  0.0,
    0.0,  0.5, 0.0,  0.0,  0.0,  1.0,
  };

  static vxr::uint16 index_data[] = {
    0, 1, 2,
  };

  void Main::start()
  {
    window_title_ = (char*)malloc(512 + strlen("VXR Hello World Test"));

    vertex_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Vertex, sizeof(vertex_data), Usage::Static });
    index_buffer_  = Engine::ref().gpu()->createBuffer({ BufferType::Index,  sizeof(index_data),  Usage::Static });

    gpu::Material::Info mat_info;
    mat_info.shader.vert = GLSL(
      in vec3 a_position;
      in vec3 a_color;
      out vec3 color;
      void main() { gl_Position = vec4(a_position, 1.0); color = a_color; }
    );
    mat_info.shader.frag = GLSL(
      in vec3 color;
      out vec4 fragColor;
      void main() { fragColor = vec4(color, 1.0); }
    );

    mat_info.attribs[0] = { "a_position", VertexFormat::Float3 };
    mat_info.attribs[1] = { "a_color",    VertexFormat::Float3 };

    material_ = Engine::ref().gpu()->createMaterial(mat_info);

    DisplayList frame;
    frame.fillBufferCommand()
      .set_buffer(vertex_buffer_)
      .set_data(vertex_data)
      .set_size(sizeof(vertex_data));
    frame.fillBufferCommand()
      .set_buffer(index_buffer_)
      .set_data(index_data)
      .set_size(sizeof(index_data));
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void Main::renderUpdate()
  {
    sprintf(window_title_,
      "%s: %d FPS @ 1280 x 720, Rendering time: %f ms",
      "VXR Hello World Test",
      fps(), 1.0 / (double)fps());

    Engine::ref().window()->set_title(window_title_);
    
    DisplayList frame;
    frame.clearCommand()
      .set_color(Color::Black);
    frame.setupMaterialCommand()
      .set_material(material_)
      .set_buffer(0, vertex_buffer_);
    frame.renderCommand()
      .set_index_buffer(index_buffer_)
      .set_count(sizeof(index_data) / sizeof(uint16))
      .set_type(IndexFormat::UInt16);
    Engine::ref().submitDisplayList(std::move(frame));

    Application::renderUpdate();
  }

} /* end of vxr namespace */