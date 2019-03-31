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

#include "../../include/graphics/height_map.h"

#include "../../deps/stb/stb_image.h"

namespace vxr
{

  mesh::HeightMap::HeightMap()
  {

  }

  mesh::HeightMap::~HeightMap()
  {

  }

  void mesh::HeightMap::loadHeightMap(const char* file, float grid_size, float height_range, float height_clamp)
  {
    // Load texture.
    int w, h, comp, bytes_per_pixel;
    unsigned char* data;

    data = stbi_load(file, &w, &h, &comp, 0);
    bytes_per_pixel = comp * sizeof(stbi_uc);

    if (!data)
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [HEIGHTMAP] Load failed. Unknown texture format.\n");
      return;
    }

    if (w < 1 || h < 1)
    {
      free(data);
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [HEIGHTMAP] Load failed. Invalid texture data.\n");
      return;
    }

    height_map_.resize(w * h);

    uint32 k = 0;
    for (int j = 0; j < h; j++)
    {
      for (int i = 0; i < w; i++)
      {
        unsigned char height = data[k];

        uint32 index = w * j + i;

        vec4 pixel;
        pixel.x = (i - (((float)w - 1) / 2)) * grid_size;
        pixel.y = (float)height / 6 * height_range;
        pixel.z = (j - (((float)h - 1) / 2)) * grid_size;
        pixel.w = 0;

        height_map_[index] = pixel;

        k += 3;
      }
    }

    free(data);
    data = nullptr;

    // Rebuild vertex data.

    face_count_ = (h - 1)*(w - 1) * 2;

    vec3 v0, v1, v2, v3;
    int i0, i1, i2, i3;

    std::vector<vec3> v;
    std::vector<vec3> n;
    std::vector<vec2> t;
    std::vector<uint32> indices;

    uint32 map_index = 0;
    k = 0;

    for (int j = 0; j < h; ++j)
    {
      for (int i = 0; i < w; ++i)
      {
        if (j < w - 1 && i < h - 1)
        {
          i0 = map_index;
          i1 = map_index + w;
          i2 = map_index + 1;
          i3 = map_index + w + 1;

          v0 = height_map_[i0];
          v1 = height_map_[i1];
          v2 = height_map_[i2];
          v3 = height_map_[i3];

          vec3 vA = v0 - v1;
          vec3 vB = v1 - v2;
          vec3 vC = v3 - v1;

          if (v0.y >= height_clamp && v1.y >= height_clamp && v2.y >= height_clamp && v3.y >= height_clamp || height_clamp == 0.0f)
          {
            vec3 vN1;
            vN1 = glm::cross(vA, vB);
            vN1 = glm::normalize(vN1);
            v.push_back(v0);
            v.push_back(v1);
            v.push_back(v2);
            n.push_back(vN1);
            n.push_back(vN1);
            n.push_back(vN1);
            t.push_back(vec2(0.0f, 0.0f));
            t.push_back(vec2(0.0f, 1.0f));
            t.push_back(vec2(1.0f, 0.0f));
            indices.push_back(k + 0);
            indices.push_back(k + 1);
            indices.push_back(k + 2);
            k += 3;

            vec3 vN2;
            vN2 = glm::cross(vB, vC);
            vN2 = glm::normalize(vN2);
            v.push_back(v2);
            v.push_back(v1);
            v.push_back(v3);
            n.push_back(vN2);
            n.push_back(vN2);
            n.push_back(vN2);
            t.push_back(vec2(1.0f, 0.0f));
            t.push_back(vec2(0.0f, 1.0f));
            t.push_back(vec2(1.0f, 1.0f));
            indices.push_back(k + 0);
            indices.push_back(k + 1);
            indices.push_back(k + 2);
            k += 3;
          }
          else
          {
            face_count_ -= 6;
          }
        }
        map_index++;

      }
    }
    
    set_indices(indices);
    set_vertices(v);
    set_normals(n);
    set_uv(t);
  }

  uint32 mesh::HeightMap::face_count() const
  {
    return face_count_;
  }
}