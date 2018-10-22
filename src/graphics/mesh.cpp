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

#include "../../include/graphics/mesh.h"

#include "../../include/engine/engine.h"

#define VOXELIZER_IMPLEMENTATION
#include "../../deps/mesh/voxelizer/voxelizer.h"

namespace vxr
{

  Mesh::Mesh()
  {
    set_name("Mesh");
  }

  Mesh::~Mesh()
  {

  }

  void Mesh::onGUI()
  {

  }

  bool Mesh::setup()
  {
    if (!hasChanged())
    {
      return true;
    }
    VXR_TRACE_SCOPE("VXR", "Mesh Setup");

    if (vertices_.size() == 0)
    {
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Missing vertices of mesh object with name %s", name().c_str());
      return false;
    }

    if (indices_.size() == 0)
    {
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Missing indices of mesh object with name %s", name().c_str());
      return false;
    }

    if (normals_.size() < vertices_.size())
    {
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Automatically recomputed normals of mesh object with name %s", name().c_str());
      recomputeNormals();
    }

    if (uv_.size() < vertices_.size())
    {
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Automatically recomputed texture coordinates of mesh object with name %s", name().c_str());
      uv_.clear();
      for (uint32 i = 0; i < vertices_.size(); ++i)
      {
        uv_.push_back(vec2());
      }
    }

	  gpu_.vertex.data.clear();
    gpu_.index.data.clear();
    for (uint32 i = 0; i < vertices_.size(); ++i)
    {
      gpu_.vertex.data.push_back(vertices_[i].x);
      gpu_.vertex.data.push_back(vertices_[i].y);
      gpu_.vertex.data.push_back(vertices_[i].z);
      gpu_.vertex.data.push_back(normals_[i].x);
      gpu_.vertex.data.push_back(normals_[i].y);
      gpu_.vertex.data.push_back(normals_[i].z);
      gpu_.vertex.data.push_back(uv_[i].x);
      gpu_.vertex.data.push_back(uv_[i].y);
    }
    gpu_.index.data = indices_;

    DisplayList add_to_frame;
		size_t v_size = gpu_.vertex.data.size() * sizeof(float);
		size_t i_size = gpu_.index.data.size() * sizeof(uint32);
    if (!gpu_.vertex.buffer.id)
    { 
      /// This size is rendered useless after the first mesh rebuild.
      gpu_.vertex.buffer = Engine::ref().gpu()->createBuffer({ BufferType::Vertex, v_size, usage_ });
    }
    if (!gpu_.index.buffer.id)
    {
      /// This size is rendered useless after the first mesh rebuild.
      gpu_.index.buffer = Engine::ref().gpu()->createBuffer({ BufferType::Index, i_size, usage_ });
    }
    add_to_frame.fillBufferCommand()
      .set_buffer(gpu_.vertex.buffer)
      .set_data(&gpu_.vertex.data[0])
      .set_size(v_size);
    add_to_frame.fillBufferCommand()
      .set_buffer(gpu_.index.buffer)
      .set_data(&gpu_.index.data[0])
      .set_size(i_size);
    Engine::ref().submitDisplayList(std::move(add_to_frame));

    dirty_ = false;
    return true;
  }

  void Mesh::set_usage(Usage::Enum usage)
  {
    usage_ = usage;
  }

  bool Mesh::hasChanged()
  {
    return dirty_;
  }

  uint32 Mesh::indexCount()
  {
    return indices_.size();
  }

  IndexFormat::Enum Mesh::indexFormat()
  {
    return IndexFormat::UInt32;
  }

  void Mesh::voxelize(vec3 voxel_size, double precision)
  {
    vx_mesh_t* mesh;
    vx_mesh_t* result;

    mesh = vx_mesh_alloc(vertices_.size() * 3, indices_.size());

    for (uint32 f = 0; f < indices_.size(); f++) 
    {
      mesh->indices[f] = indices_[f];
    }

    for (uint32 v = 0; v < vertices_.size(); v++) 
    {
      mesh->vertices[v].x = vertices_[v].x;
      mesh->vertices[v].y = vertices_[v].y;
      mesh->vertices[v].z = vertices_[v].z;
    }

    result = vx_voxelize(mesh, voxel_size.x, voxel_size.y, voxel_size.z, precision);

    vertices_.clear();
    normals_.clear();
    indices_.clear();
    uv_.clear();

    for (uint32 v = 0; v < result->nindices; v += 3) 
    { /// Voxel normals are incorrect
      normals_.push_back(vec3(result->normals[result->normalindices[v]].x, result->normals[result->normalindices[v+1]].y, result->normals[result->normalindices[v+2]].z));
      indices_.push_back(result->indices[v]);
      indices_.push_back(result->indices[v+1]);
      indices_.push_back(result->indices[v+2]);
    }

    for (uint32 v = 0; v < result->nvertices; v++) 
    {
      vertices_.push_back(vec3(result->vertices[v].x, result->vertices[v].y, result->vertices[v].z));
      uv_.push_back(vec2()); /// UV for voxels are...? voxel size / actual size?
    }

    vx_mesh_free(mesh);
    vx_mesh_free(result);
    dirty_ = true;
  }

  void Mesh::recomputeNormals()
  {
    normals_.clear();

    for (uint32 i = 0; i < vertices_.size(); i++)
    {
      normals_.push_back(vec3());
    }

    for (uint32 i = 0; i < indices_.size(); i += 3)
    {
      vec3 a = vertices_[indices_[i + 0]];
      vec3 b = vertices_[indices_[i + 1]];
      vec3 c = vertices_[indices_[i + 2]];
      vec3 n = glm::normalize(glm::cross(a - b, b - c));

      normals_[indices_[i + 0]] = n;
      normals_[indices_[i + 1]] = n;
      normals_[indices_[i + 2]] = n;
    }
  }

  // -------------------------------------------------------------------------------------------------------

  static std::vector<vec3> cube_pos = {
    {-0.5f, -0.5f, -0.5f },
    {-0.5f,  0.5f, -0.5f },
    { 0.5f, -0.5f, -0.5f },
    { 0.5f,  0.5f, -0.5f },

    {-0.5f, -0.5f,  0.5f },
    {-0.5f, -0.5f, -0.5f },
    { 0.5f, -0.5f,  0.5f },
    { 0.5f, -0.5f, -0.5f },

    {-0.5f,  0.5f, -0.5f },
    {-0.5f,  0.5f,  0.5f },
    { 0.5f,  0.5f, -0.5f },
    { 0.5f,  0.5f,  0.5f },

    { 0.5f, -0.5f, -0.5f },
    { 0.5f,  0.5f, -0.5f },
    { 0.5f, -0.5f,  0.5f },
    { 0.5f,  0.5f,  0.5f },

    {-0.5f, -0.5f,  0.5f },
    {-0.5f,  0.5f,  0.5f },
    {-0.5f, -0.5f, -0.5f },
    {-0.5f,  0.5f, -0.5f },

    {-0.5f,  0.5f,  0.5f },
    {-0.5f, -0.5f,  0.5f },
    { 0.5f,  0.5f,  0.5f },
    { 0.5f, -0.5f,  0.5f },
  };

  static std::vector<vec2> cube_uv = {
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },

    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },

    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },

    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },

    {-0.0f, 0.0f },
    {-0.0f, 1.0f },
    {-1.0f, 0.0f },
    {-1.0f, 1.0f },

    { 0.0f, 0.0f },
    { 0.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
  };

  static std::vector<uint32> cube_index = {
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

  EngineMesh::Cube::Cube()
  {
    set_name("Cube Mesh");
    set_indices(cube_index);
    set_vertices(cube_pos);
    recomputeNormals();
    set_uv(cube_uv);
  }

  EngineMesh::Cube::~Cube()
  {

  }

  std::vector<vec3> quad_pos = {
    {-1.0f, -1.0f,  0.0f },
    { 1.0f, -1.0f,  0.0f },
    { 1.0f,  1.0f,  0.0f },
    {-1.0f,  1.0f,  0.0f },
  };

  std::vector<vec2> quad_uv = {
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
  };

  std::vector<uint32> quad_index = {
    0, 1, 2,
    0, 2, 3
  };

  EngineMesh::Quad::Quad()
  {
    set_name("Quad Mesh");
    set_indices(quad_index);
    set_vertices(quad_pos);
    recomputeNormals();
    set_uv(quad_uv);
  }

  EngineMesh::Quad::~Quad()
  {

  }
}