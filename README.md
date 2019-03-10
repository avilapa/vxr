
[![vxr engine logo](https://github.com/avilapa/vxr/blob/media/logo_background.png)](https://avilapa.github.io)
_version 0.5.3_

# What is [vxr](https://github.com/avilapa/vxr)?

__vxr__ is a project that emerged from the desire to build a __general purpose rendering tool__ that I could use to implement anything I was curious about or wanted to learn about.

As it stands now, it is a __multithreaded 3D game development engine written in C++__ and oriented to support different rendering backends. It has its own __PBR material model__ based on the standard microfacet BRDF, but extended to support other visual properties of materials such as __iridescence__ or __clear coat__. The engine has two different APIs for development:

  - A [__lower level API__](https://github.com/avilapa/vxr/blob/master/examples/01-HelloWorld/hello_world.cpp) based on render display list commands that can be used to build __small but efficient__ applications.
  - A [__higher level API__](https://github.com/avilapa/vxr/blob/master/examples/04-Mesh/mesh.cpp) built on top of the lower level one, which abstracts it to provide a __rendering agnostic and component oriented__ API.
  
Project webpage: https://avilapa.github.io/vxr-engine/

![material model](https://github.com/avilapa/vxr/blob/media/material_model.png)
_Material Model (from left to right): std, std + clear coat, std + iridescence, std + iridescence + clear coat._

# Building

_[A vs2017 + opengl Solution is provided by default]_

This project uses [GENie.lua](https://github.com/bkaradzic/GENie) to create the project solution. Open a CMD in the root directory and execute the following commands to re-create a solution:

	cd project
	..\tools\genie [parameters] <vs2017|vs2015>
	
The solution will be located in _project/vs/VXR-Engine.sln_.

A list of example projects are provided within the solution to showcase the engine's capabilities. To __create a new project__, add a call to the function '__makeProject("yourProjectName");__' at the end of the file _project/genie.lua_ and build.
  
### Parameters

 - __--gl__: Build with OpenGL backend.
 - __--debug_tracing__: Build with minitrace and enable _trace.json_ file creation.
 - __--no-console__: Do not launch a console with the graphic window.
 - __--no-threading__: Launch rendering on the same thread as the logic + no async loading tasks.
 - __--no-ui__: Disable all the UI (will still compile ImGui though).

# Examples

#### 01-HelloWorld
![01-HelloWorld](https://github.com/avilapa/vxr/blob/media/01-HelloWorld.png)
#### 02-Instancing
![02-Instancing](https://github.com/avilapa/vxr/blob/media/02-Instancing.png)
#### 03-Framebuffers
![03-Framebuffers](https://github.com/avilapa/vxr/blob/media/03-Framebuffers.png)
#### 04-Mesh
![04-Mesh](https://github.com/avilapa/vxr/blob/media/04-Mesh.png)
#### 05-Materials
![05-Material](https://github.com/avilapa/vxr/blob/media/05-Material.png)
#### 06-Procedural
![06-Procedural](https://github.com/avilapa/vxr/blob/media/06-Procedural.png)

# Features

- Multithreaded agnostical graphics API.
- Command Based Rendering, allowing to build the render commands in advance to be run in a separate thread.
- _GameObjects_ with the following components currently implemented (each component is controlled and updated by a specific system):
  - Transform: Containing the object's transformation and position in the scene's hierarchy.
    - _Quaternions!_ 
  - Mesh Filter: Reference to the mesh the object will be rendered with.
  - Renderer: Reference to the Material Instance the object will be rendered with.
    - Current Materials:
      - Standard Lit (PBR)
        - Microfacet BRDF + Thin Film Iridescence + Clear Coat
      - Unlit
      - Wireframe
      - Skybox
      - Screen
      - Planet (_only in the PlanetEditor example!_)
      - ___Add your own!___
  - Light: Turns the object into a source of light.
    - Directional and Punctual lights supported.
  - Image Based Light: Calculates irradiance of the scene to be used by all PBR materials in the engine.
    - Takes either a cubemap texture or a 2D equirectangular texture.
  - Camera: Adds to an object all the functionality needed to render from its point of view with its unique parameters.
    - Camera Composer: Organizes all post-processing needed for any specific camera, and handles updates automatically.
      - Postprocessing Materials:
        - One-Pass-Filters: Grayscale, Negative.
  - Custom Component: Similar to _Unity's MonoBehaviour_, allows the user to create custom reusable components with specific behaviours.
- Model loading (_.obj_).
- Lazy Loading: All resources are loaded in different threads to prevent the engine from freezing.
- UI Editor.

# Support

### Platforms

- Windows

### Backends

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
- [px_sched.h](https://github.com/pplux/px/blob/master/px_sched.h) - _If you are not building with single threading (--no-threading)_

# Contributing

This is the first project I have made open source, therefore I am wide open to feedback from the community and contributions :^)

# Acknowledgements

- The OpenGL render backend is greatly inspired by [PpluX](https://github.com/pplux)'s [px_render.h](https://github.com/pplux/px/blob/master/px_render.h).
- The material model is greatly inspired by [Google's Filament](https://github.com/google/filament).

# [MIT License](/LICENSE)

# Contact

Mail: victorap97@gmail.com

Twitter: https://twitter.com/97torvic

Portfolio: https://avilapa.github.io/portfolio
