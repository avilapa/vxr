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

#include "../engine/GPU.h"
#include "../core/scene.h"
#include "../core/assets.h"
#include "../components/renderer.h"
#include "../components/transform.h"
#include "../components/mesh_filter.h"
#include "../components/camera.h"
#include "../components/custom.h"
#include "../components/light.h"

/**
* \file engine.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief VXR Engine singleton class. Contains all Systems as well as the GPU and is in charge of the logic thread updates.
*
*/
namespace vxr 
{

  class Engine : public Object 
  {

    VXR_OBJECT(Engine, Object);

    friend class Application;
  public:
    ~Engine();
    static Engine& ref();

    void set_preinit_params(const Params& params);

    bool is_exiting();

    void submitDisplayList(DisplayList &&dl);
    void submitUIFunction(std::function<void()> function);

    void loadScene(ref_ptr<Scene> scene);
    ref_ptr<Scene> scene();

    ref_ptr<GPU> gpu();
    ref_ptr<Window> window();
    ref_ptr<AssetManager> assetManager();

    ref_ptr<System::Light> light();
    ref_ptr<System::Custom> custom();
    ref_ptr<System::Camera> camera();
    ref_ptr<System::Renderer> renderer();
    ref_ptr<System::Transform> transform();
    ref_ptr<System::MeshFilter> meshFilter();

  private:
    Params preinit_params_;

    ref_ptr<GPU> gpu_;
    ref_ptr<Scene> scene_;
    ref_ptr<AssetManager> asset_manager_;

    ref_ptr<System::Light> light_;
    ref_ptr<System::Custom> custom_;
    ref_ptr<System::Camera> camera_;
    ref_ptr<System::Renderer> renderer_;
    ref_ptr<System::Transform> transform_;
    ref_ptr<System::MeshFilter> mesh_filter_;

  private:
    Engine();

    bool init();
    void start();
    void update();
    void postUpdate();
    void renderUpdate();
    void renderPostUpdate();
    void stop();
  };

} /* end of vxr namespace */