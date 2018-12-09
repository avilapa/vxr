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

// 0. Define the entry point.
VXR_DEFINE_APP_MAIN(vxr::Main)

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
    // 1. Create vertex and index buffer for the triangle.
    vertex_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Vertex, sizeof(vertex_data), Usage::Static });
    index_buffer_  = Engine::ref().gpu()->createBuffer({ BufferType::Index,  sizeof(index_data),  Usage::Static });

    // 2. Initialize shader data and vertex attributes data and create material for the triangle.
    // Shader version 330 and a set of helper functions are provided by default. You can also use
    // the function 'Shader::Load(std::string file)' to load the shader from a file.
    gpu::Material::Info mat_info;
    mat_info.shader.vert =
      "in vec3 a_position;                        \n"
      "in vec3 a_color;                           \n"
      "out vec3 color;                            \n"
      "void main()                                \n"
      "{                                          \n"
      "  gl_Position = vec4(a_position, 1.0);     \n"
      "  color = a_color;                         \n"
      "}                                          \n";
    mat_info.shader.frag =
      "in vec3 color;                             \n"
      "void main()                                \n" 
      "{                                          \n"
      "  setFragmentColor(color);                 \n"
      "}                                          \n";

    mat_info.attribs[0] = { "a_position", VertexFormat::Float3 };
    mat_info.attribs[1] = { "a_color",    VertexFormat::Float3 };

    material_ = Engine::ref().gpu()->createMaterial(mat_info);

    // 3. Create a render command display list to fill the index and vertex buffers with the appropriate data once.
    DisplayList frame;
    frame.fillBufferCommand()
      .set_buffer(vertex_buffer_)
      .set_data(vertex_data)
      .set_size(sizeof(vertex_data));
    frame.fillBufferCommand()
      .set_buffer(index_buffer_)
      .set_data(index_data)
      .set_size(sizeof(index_data));

    // 4. Submit display list to be rendered on the GPU.
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void Main::renderUpdate()
  {
    DisplayList frame;
    // 5. Clear the screen.
    frame.clearCommand()
      .set_color(Color::Black);
    // 6. Setup the material for rendering with the vertex buffer attached.
    frame.setupMaterialCommand()
      .set_material(material_)
      .set_buffer(0, vertex_buffer_);
    // 7. Create render command and submit display list to accummulate with previous commands.
    frame.renderCommand()
      .set_index_buffer(index_buffer_)
      .set_count(sizeof(index_data) / sizeof(uint16))
      .set_type(IndexFormat::UInt16);
    Engine::ref().submitDisplayList(std::move(frame));

    Application::renderUpdate();
  }

} /* end of vxr namespace */