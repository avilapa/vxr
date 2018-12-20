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

#include "planet.h"

namespace vxr
{

  void Planet::start()
  {
    set_name("Planet");
    vec3 directions[] = { vec3(0,1,0), vec3(0,-1,0), vec3(-1,0,0), vec3(1,0,0), vec3(0,0,-1), vec3(0,0,1) };

    shapeSettings.alloc();
    shapeGenerator.alloc()->init(shapeSettings);

    colorSettings.alloc();
    colorGenerator.alloc()->init(colorSettings);

    for (uint32 i = 0; i < 6; i++)
    {
      ref_ptr<GameObject> meshObj;
      meshObj.alloc()->set_name("Terrain Face");
      meshObj->transform()->set_parent(transform());

      ref_ptr<Mesh> mesh;
      mesh.alloc();

      meshObj->addComponent<Renderer>()->material = colorGenerator->mat.get();
      ref_ptr<MeshFilter> mf = meshObj->addComponent<MeshFilter>();
      meshFilters.push_back(mf);
      mf->mesh = mesh;

      ref_ptr<TerrainFace> tf;
      tf.alloc()->init(shapeGenerator, meshFilters[i]->mesh, directions[i]);
      terrainFaces.push_back(tf);
    }
    generate();
  }

  void Planet::renderUpdate()
  {
    if (shapeGenerator->hasChanged() || uiSettings.resolution != resolution || uiSettings.faceRenderMask != faceRenderMask)
    {
      resolution = uiSettings.resolution;
      faceRenderMask = uiSettings.faceRenderMask;
      generateMesh();
    }

    if (colorGenerator->hasChanged() || uiSettings.wireframe != wireframe)
    {
      wireframe = uiSettings.wireframe;
      for (uint32 i = 0; i < terrainFaces.size(); ++i)
      {
        if (uiSettings.wireframe)
        {
          transform()->child(i)->getComponent<Renderer>()->material = colorGenerator->wireframe_mat.get();
        }
        else
        {
          transform()->child(i)->getComponent<Renderer>()->material = colorGenerator->mat.get();
        }
      }
      generateColors();
    }
  }

  void Planet::generate()
  {
    generateMesh();
  }

  void Planet::generateMesh()
  {
    shapeGenerator->elevationMinMax.reset();
    for (uint32 i = 0; i < terrainFaces.size(); ++i)
    {
      bool renderFace = faceRenderMask == FaceRenderMask::All || (int)faceRenderMask - 1 == i;
      transform()->child(i)->gameObject()->set_active(renderFace);
      if (renderFace)
      {
        terrainFaces[i]->constructMesh(resolution);
      }
    }

    colorGenerator->updateElevation(vec2(shapeGenerator->elevationMinMax.min, shapeGenerator->elevationMinMax.max));
    generateColors();
  }

  void Planet::generateColors()
  {
    colorGenerator->updateColors();
    for (uint32 i = 0; i < terrainFaces.size(); ++i)
    {
      if (transform()->child(i)->gameObject()->active())
      {
        terrainFaces[i]->updateUVs(colorGenerator);
      }
    }
  }

  void Planet::onGUI()
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
      | ImGuiTreeNodeFlags_Selected
      | ImGuiTreeNodeFlags_OpenOnDoubleClick
      | ImGuiTreeNodeFlags_DefaultOpen;

    ImGui::Spacing();
    if (ImGui::TreeNodeEx(uiText("General Settings").c_str(), node_flags))
    {
      ImGui::Spacing();
      ImGui::Checkbox("Enabled", &uiSettings.wireframe);
      ImGui::Spacing();
      ImGui::ColorEdit3("Color", (float*)&colorGenerator->uiSettings->wireframe_color);
      ImGui::Spacing();
      ImGui::SliderInt("Resolution", &uiSettings.resolution, 2, 200);
      ImGui::Spacing();
      ImGui::Combo("Render Face Mask", (int*)&uiSettings.faceRenderMask, "All\0Top\0Bottom\0Left\0Right\0Front\0Back");
      ImGui::Spacing();
      if (ImGui::Button("Zoom to Top layer"))
      {
        zoom = !zoom;
      }
      ImGui::TreePop();
    }
    colorGenerator->onGUI();
    shapeGenerator->onGUI();
  }

} /* end of vxr namespace */