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

#include "../graphics/render_context.h"

/**
* \file mesh.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Mesh class, containing the vertex, index, uv and normal buffers. Also some prepared Engine Meshes are defined here.
*
*/
namespace vxr
{

  namespace System { class Renderer; }
  class GameObject;
  class Composer;

  class Mesh : public Object
  {
    VXR_OBJECT(Mesh, Object);
    friend class System::Renderer;
    friend class Composer;
  public:
    Mesh();
    virtual ~Mesh();

    virtual void onGUI() override;

#define PROPERTY(type, name, fname, ...) \
     private:\
      type name = __VA_ARGS__;\
     public:\
      void set_##fname(const type &data) { name = data; dirty_ = true; }\
      type fname() const { return name; }

    PROPERTY(std::vector<vec3>,   vertices_, vertices, {});
    PROPERTY(std::vector<vec3>,   normals_,  normals,  {});
    PROPERTY(std::vector<vec2>,   uv_,       uv,       {});
    PROPERTY(std::vector<uint32>, indices_,  indices,  {});

#undef PROPERTY

    void voxelize(vec3 voxel_size, double precision);
    void recomputeNormals();

    void set_usage(Usage::Enum usage);

    bool hasChanged();
    uint32 indexCount();
    IndexFormat::Enum indexFormat();

  private:
    bool dirty_ = true;

    Usage::Enum usage_ = Usage::Static;

    struct GPU
    {
      struct Vertex
      {
        gpu::Buffer buffer;
        std::vector<float> data;
      } vertex;

      struct Index
      {
        gpu::Buffer buffer;
        std::vector<uint32> data;
      } index;
    } gpu_;

    bool setup();
  };

  namespace EngineMesh
  {
    class Cube : public Mesh
    {

      VXR_OBJECT(Cube, Mesh);

    public:
      Cube();
      virtual ~Cube();
    };

    class Quad : public Mesh
    {

      VXR_OBJECT(Quad, Mesh);

    public:
      Quad();
      virtual ~Quad();
    };
  }

} /* end of vxr namespace */