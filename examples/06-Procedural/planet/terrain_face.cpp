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

#include "terrain_face.h"

namespace vxr
{

  void TerrainFace::init(ref_ptr<ShapeGenerator> shapeGenerator, ref_ptr<Mesh> mesh, vec3 localUp)
  {
    this->shapeGenerator = shapeGenerator;
    this->mesh = mesh;
    this->localUp = localUp;

    axisA = vec3(localUp.y, localUp.z, localUp.x);
    axisB = glm::cross(localUp, axisA);
  }

  void TerrainFace::constructMesh(uint32 resolution)
  {
    this->resolution = resolution;
    std::vector<vec3> vertices;
    std::vector<uint32> triangles;
    std::vector<vec2> uv;
    uint32 triIndex = 0;

    for (uint32 i = 0; i < resolution * resolution; ++i) 
    {
      vertices.push_back(vec3());
    }

    if (mesh->uv().size() != resolution * resolution)
    {
      for (uint32 i = 0; i < resolution * resolution; ++i)
      {
        uv.push_back(vec2());
      }
    }
    else
    {
      uv = mesh->uv();
    }

    for (uint32 i = 0; i < (resolution - 1) * (resolution - 1) * 6; ++i)
    {
      triangles.push_back(0);
    }
    
    for (uint32 y = 0; y < resolution; y++)
    {
      for (uint32 x = 0; x < resolution; x++)
      {
        int i = x + y * resolution;
        vec2 percent = vec2(x / (float)(resolution - 1), y / (float)(resolution - 1));
        vec3 pointOnUnitCube = localUp + (percent.x - .5f) * 2 * axisA + (percent.y - .5f) * 2 * axisB;
        vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);
        vertices[i] = shapeGenerator->calculatePointOnPlanet(pointOnUnitSphere);

        if (x != resolution - 1 && y != resolution - 1)
        {
          triangles[triIndex] = i;
          triangles[triIndex + 1] = i + resolution + 1;
          triangles[triIndex + 2] = i + resolution;

          triangles[triIndex + 3] = i;
          triangles[triIndex + 4] = i + 1;
          triangles[triIndex + 5] = i + resolution + 1;
          triIndex += 6;
        }
      }
    }
    mesh->set_vertices(vertices);
    mesh->set_indices(triangles);
    mesh->recomputeNormals();
    mesh->set_uv(uv);
    mesh->set_usage(Usage::Dynamic);
  }

  void TerrainFace::updateUVs(ref_ptr<ColorGenerator> color_generator)
  {
    std::vector<vec2> uv;

    for (int y = 0; y < resolution; y++)
    {
      for (int x = 0; x < resolution; x++)
      {
        vec2 percent = vec2(x / (float)(resolution - 1), y / (float)(resolution - 1));
        vec3 pointOnUnitCube = localUp + (percent.x - .5f) * 2 * axisA + (percent.y - .5f) * 2 * axisB;
        vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

        float biome_uv = color_generator->percentFromPoint(pointOnUnitSphere);
        uv.push_back({ biome_uv, 0.0f });
      }
    }

    mesh->set_uv(uv);
  }

} /* end of vxr namespace */