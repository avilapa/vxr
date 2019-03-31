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

#include "../core/object.h"
#include "../graphics/display_list.h"

#include "../graphics/ui/log.h"
#include "../graphics/ui/editor.h"


#include <functional>

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

  class GPU;
  class Window;
  class Scene;
  class AssetManager;

  namespace System { class IBL; }
  namespace System { class Light; }
  namespace System { class Custom; }
  namespace System { class Camera; }
  namespace System { class Renderer; }
  namespace System { class Collider; }
  namespace System { class Transform; }
  namespace System { class Rigidbody; }
  namespace System { class MeshFilter; }

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
#ifdef VXR_THREADING
    void submitAsyncTask(threading::Task& task, threading::Sync* sync);
#endif 

    void loadScene(ref_ptr<Scene> scene);
    ref_ptr<Scene> scene();

    ref_ptr<GPU> gpu();
    ref_ptr<Window> window();
    ref_ptr<AssetManager> assetManager();

    ref_ptr<System::IBL> ibl();
    ref_ptr<System::Light> light();
    ref_ptr<System::Custom> custom();
    ref_ptr<System::Camera> camera();
    ref_ptr<System::Renderer> renderer();
    ref_ptr<System::Collider> collider();
    ref_ptr<System::Transform> transform();
    ref_ptr<System::Rigidbody> rigidbody();
    ref_ptr<System::MeshFilter> meshFilter();

    vxr::ui::EditorLog Log;

  private:
    Params preinit_params_;

#ifdef VXR_THREADING
    threading::Scheduler scheduler_;
#endif

    ref_ptr<GPU> gpu_;
    ref_ptr<Scene> scene_;
    ref_ptr<AssetManager> asset_manager_;

    ref_ptr<System::IBL> ibl_;
    ref_ptr<System::Light> light_;
    ref_ptr<System::Custom> custom_;
    ref_ptr<System::Camera> camera_;
    ref_ptr<System::Renderer> renderer_;
    ref_ptr<System::Collider> collider_;
    ref_ptr<System::Transform> transform_;
    ref_ptr<System::Rigidbody> rigidbody_;
    ref_ptr<System::MeshFilter> mesh_filter_;

  private:
    Engine();

    bool init();
    void start();
    void preUpdate();
    void update(float dt);
    void postUpdate();
    void renderPreUpdate();
    void renderUpdate();
    void renderPostUpdate();
    void stop();

    void startSystems();
    void stopSystems();
  };

  #define VXR_LOG(LEVEL, ...) \
    VXR_DEBUG_FUNC(LEVEL, __VA_ARGS__);\
    if(LEVEL<=VXR_DEBUG_LEVEL_INFO) Engine::ref().Log.AddLog(__VA_ARGS__);

} /* end of vxr namespace */