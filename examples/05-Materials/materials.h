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

#include "../../include/engine/application.h"
#include "../../include/engine/core_minimal.h"

/**
* \file mesh.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief This example showcases the PBR material model of the engine using the high level API.
*
* This example takes on creating multiple scenes and changing between them. The first scene shows
* the PBR material model of the engine and adds some custom UI functions to it. The second scene
* performs a 'white furnace test' of the material model to showcase the current energy conservation
* features of the model.
*
*/
namespace vxr 
{

  class Main : public Application
  {

    VXR_OBJECT(Main, Application);

  public:
    Main();

    void start() override;
    void update(float dt) override;

  private:
    void createSceneMaterialEditor();
    void createSceneWhiteFurnaceTest();

    void customUI();
    void materialEditorUI();

  private:
    ref_ptr<Scene> scene_material_editor_;
    ref_ptr<Scene> scene_white_furnace_;

    const static uint32 NUM_PREVIEWS = 3;
    ref_ptr<GameObject> preview_model_[NUM_PREVIEWS];
  };

} /* end of vxr namespace */