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

#include "../../../include/core/gameobject.h"
#include "../../../include/components/mesh_filter.h"
#include "shape_settings.h"
#include "color_settings.h"

namespace vxr {

  class TerrainFace : public Object
  {
    VXR_OBJECT(TerrainFace, Object);
  public:
    void init(ref_ptr<ShapeGenerator> shapeGenerator, ref_ptr<Mesh> mesh, vec3 localUp);
    void constructMesh(uint32 resolution);
    void updateUVs(ref_ptr<ColorGenerator> color_generator);

    ref_ptr<ShapeGenerator> shapeGenerator;

    ref_ptr<Mesh> mesh;
    vec3 localUp;
    vec3 axisA;
    vec3 axisB;
    int resolution;
  };

} /* end of vxr namespace */