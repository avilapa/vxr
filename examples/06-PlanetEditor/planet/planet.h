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

#include "../../../include/engine/core_minimal.h"
#include "terrain_face.h"
#include "shape_settings.h"
#include "color_settings.h"

namespace vxr {

  class Planet : public Custom
  {
    VXR_OBJECT(Planet, Custom);
  public:
    virtual void start() override;
    virtual void renderUpdate() override;
    void generate();

    void generateMesh();
    void generateColors();

    virtual void onGUI() override;

    bool zoom = false;

    struct FaceRenderMask
    {
      enum Enum
      {
        All, Top, Bottom, Left, Right, Front, Back,
      };
    };
    FaceRenderMask::Enum faceRenderMask;

    int resolution = 30;
    bool wireframe = false;

    std::vector<ref_ptr<MeshFilter>> meshFilters;
    std::vector<ref_ptr<TerrainFace>> terrainFaces;

    ref_ptr<ShapeGenerator> shapeGenerator;
    ref_ptr<ColorGenerator> colorGenerator;

    ref_ptr<ColorSettings> colorSettings;
    ref_ptr<ShapeSettings> shapeSettings;

    struct UISettings 
    {
      FaceRenderMask::Enum faceRenderMask;
      bool wireframe = false;
      int resolution = 30;
    } uiSettings;
  };

} /* end of vxr namespace */