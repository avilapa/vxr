[![vxr engine logo](/assets/textures/readme/logo.png)](https://avilapa.github.io)

_version 0.4.1_

# What is [vxr](https://github.com/avilapa/vxr)?

__vxr__ is a project that emerged from the desire to build a __general purpose rendering tool__ that I could use to implement anything I was curious about or wanted to learn about.

As it stands now, it is a __multithreaded 3D game development engine written in C++__ and oriented to support different rendering backends. The engine has two different APIs for development:

  - A __lower level API__ based on render display list commands that can be used to build __small but efficient__ applications.
  - A __higher level API__ built on top of the lower level one, which abstracts it to provide a __rendering agnostic and component oriented__ API.
  
Project webpage: https://avilapa.github.io/vxr-engine/

![Planets](/assets/textures/readme/planets.png)

# Building

_[A vs2017 + opengl Solution is provided by default]_

This project uses [GENie.lua](https://github.com/bkaradzic/GENie) to create the project solution. Open a CMD in the root directory and execute the following commands to re-create a solution:

	cd project
	..\tools\genie [parameters] <vs2017|vs2015>
	
The solution will be located in _project/vs/VXR-Engine.sln_.
  
### Parameters

 - __--gl__: Build with OpenGL backend.
 - __--debug_tracing__: Build with minitrace and enable _trace.json_ file creation.
 - __--no-console__: Do not launch a console with the graphic window.
 - __--no-threading__: Launch rendering on the same thread as the logic.
 - __--no-ui__: Disable all the UI (will still compile ImGui though).

# Examples

A list of examples are provided within the solution to showcase the engine's capabilities. To create a new example, add a call to the function '__makeProject("yourExampleName");__' at the end of the file _project/genie.lua_ and build.

#### 01-HelloWorld
![01-HelloWorld](/assets/textures/readme/01-HelloWorld.png)
#### 02-Instancing
![02-Instancing](/assets/textures/readme/02-Instancing.png)
#### 03-Framebuffers
![03-Framebuffers](/assets/textures/readme/03-Framebuffers.png)
#### 04-Mesh
![04-Mesh](/assets/textures/readme/04-Mesh.png)
#### 06-PlanetEditor
![06-PlanetEditor](/assets/textures/readme/06-PlanetEditor.png)
# Features

- Multithreaded agnostical graphics API
- Command Based Rendering, allowing to build the render commands in advance to be run in a separate thread
- _GameObjects_ with the following components currently implemented (each component is controlled and updated by a specific system):
  - Transform: Containing the object's transformation and position in the scene's hierarchy.
  - Mesh Filter: Reference to the mesh the object will be rendered with.
  - Renderer: Reference to the Material Instance the object will be rendered with.
    - Current Materials:
      - Standard (Lit)
      - Unlit
      - Wireframe
      - Skybox
      - Screen
      - Planet (_only in the PlanetEditor example!_)
      - ___Add your own!___
  - Light: Turns the object into a source of light.
  - Image Based Light.
  - Camera: Adds to an object all the functionality needed to render from its point of view with its unique parameters.
    - Camera Composer: Organizes all post-processing needed for any specific camera, and handles updates automatically.
      - Postprocessing Materials:
        - One-Pass-Filters: Grayscale, Negative.
  - Custom Component: Similar to _Unity's MonoBehaviour_, allows the user to create custom reusable components with specific behaviours.
- Model loading (_.obj_)
- UI Editor

# API Reference


### Higher Level API

```c++
// 0. Define the entry point.
VXR_DEFINE_APP_MAIN(vxr::Main)

namespace vxr
{

  Main::Main()
  {
    // 1. Initialize GPU and Window parameters.
    Params p;
    p.gpu = { 100, 100, 100, 100 };
    p.window = { { 1920, 1080} };
    Engine::ref().set_preinit_params(p);
  }

  void Main::start()
  {
    // 2. Create a camera.
    cam_.alloc()->set_name("Camera");
    cam_->addComponent<Camera>()->transform()->set_local_position(vec3(0,0,4));

    // 3. Load the Teapot mesh and set an initial rotation.
    mesh_ = Asset::loadModelOBJ("../../assets/meshes/obj/teapot_mesh.obj");
    mesh_->set_name("Teapot");
    mesh_->transform()->set_local_rotation(vec3(glm::radians(90.0f), 0.0f, 0.0f));
    
    // 4. Create a light
    light_.alloc()->set_name("Sun");
    light_->addComponent<Light>()->set_color(Color::Random());
    light_->transform()->set_local_position(vec3(0.8, 0.3, 2.0));

    // 5. Create a Scene, parent the objects and load.
    ref_ptr<Scene> scene_;
    scene_.alloc();

    scene_->addObject(cam_);
    scene_->addObject(mesh_);
    scene_->addObject(light_);

    Engine::ref().loadScene(scene_);

    // 6. Submit the UI function.
    Engine::ref().submitUIFunction([this]() { ui::Editor(); });

    Application::start();
  }

  void Main::update()
  {
    // 7. Rotate the mesh in update() instead of renderUpdate() to make the rotation framerate independent 
    // by multiplying it by deltaTime(). The update() method may be executed several times in a frame to 
    // catch up with the render thread.
    mesh_->transform()->set_local_rotation(mesh_->transform()->local_rotation() 
    				           + vec3(0.21, 0.12, 0.5) * deltaTime());
    Application::update();
  }

} /* end of vxr namespace */
```

### Lower Level API

```c++
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
    vertex_buffer_ = Engine::ref().gpu()->createBuffer({ BufferType::Vertex, 
							 sizeof(vertex_data), 
							 Usage::Static });
    index_buffer_  = Engine::ref().gpu()->createBuffer({ BufferType::Index,  
    							 sizeof(index_data),  
							 Usage::Static });

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

    // 3. Create a render command display list to fill the index and vertex buffers with the 
    // appropriate data once.
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
```

# Supported Platforms

- Windows

# Supported Backends

- OpenGL
- _WIP: DirectX3D 11_

# Dependencies

The project includes some 3rd party dependencies, but not all of them are necessary (e.g. you won't need GLEW if you generate the project to use DirectX3D 11, you won't need minitrace if you don't want trace debugging, etc).

- [glm](https://github.com/g-truc/glm)
- [imgui](https://github.com/ocornut/imgui)
- [tinyobjloader](https://github.com/syoyo/tinyobjloader)
- [voxelizer](https://github.com/karimnaaji/voxelizer)
- [stb](https://github.com/nothings/stb)

### Optional Dependencies

- [glew](http://glew.sourceforge.net/) - _If you are building with OpenGL (--gl)_
- [glfw](https://github.com/glfw/glfw) - _If you are building with OpenGL (--gl)_
- [minitrace](https://github.com/hrydgard/minitrace) - _If you are building with debug tracing (--debug-tracing)_

# Contributing

This is the first project I have made open source, therefore I am wide open to feedback from the community and contributions :^)

# Thanks

Special thanks to my previous tutor [PpluX](https://github.com/pplux), top coder who lately wrote [px_render.h](https://github.com/pplux/px/blob/master/px_render.h), from which I learned a lot and incorporated part of it into my OpenGL backend.

# [MIT License](/LICENSE)

# Contact

Mail: victorap97@gmail.com

Twitter: https://twitter.com/97torvic

Web/Blog/Portfolio: https://avilapa.github.io/
