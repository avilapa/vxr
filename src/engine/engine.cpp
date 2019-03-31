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

#include "../../include/engine/engine.h"

#include "../../include/engine/GPU.h"
#include "../../include/core/scene.h"
#include "../../include/core/assets.h"

#include "../../include/components/ibl.h"
#include "../../include/components/light.h"
#include "../../include/components/renderer.h"
#include "../../include/components/transform.h"
#include "../../include/components/rigidbody.h"
#include "../../include/components/collider.h"
#include "../../include/components/mesh_filter.h"
#include "../../include/components/camera.h"
#include "../../include/components/custom.h"

#include <time.h>

#ifdef VXR_THREADING
#  define PX_SCHED_IMPLEMENTATION 1
#  include "../../deps/threading/px_sched.h"
#endif

namespace vxr 
{

  Engine& Engine::ref()
  {
    static Engine *inst = new Engine();
    return *inst;
  }

  Engine::Engine()
  {
    set_name("Engine");
    srand(time(NULL));

    gpu_.alloc();
    asset_manager_.alloc();
  }

  Engine::~Engine()
  {
  }

  void Engine::set_preinit_params(const Params& params)
  {
    preinit_params_ = params;
    gpu_->set_preinit_params(preinit_params_.gpu);
    gpu_->window_->set_preinit_params(preinit_params_.window);
  }

  bool Engine::init()
  {
    VXR_TRACE_BEGIN("VXR", "Engine Systems Init");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: [ENGINE] Initializing engine systems.\n");
#ifdef VXR_THREADING
    scheduler_.init();
#endif
    ibl_.alloc();
    light_.alloc();
    custom_.alloc();
    camera_.alloc();
    renderer_.alloc();
    collider_.alloc();
    transform_.alloc();
    rigidbody_.alloc();
    mesh_filter_.alloc();

    gpu_->init();
    asset_manager_->init();

    // Initialize systems
    light_->init();
    camera_->init();

    VXR_TRACE_END("VXR", "Engine Systems Init");
    return true;
  }

  void Engine::start()
  {
    if (!scene_)
    {
      return;
    }
    startSystems();
  }

  void Engine::preUpdate()
  {
    if (scene_.get())
    {
      VXR_TRACE_BEGIN("VXR", "Engine Systems Pre Update");
      custom_->preUpdate();
      rigidbody_->preUpdate();
      VXR_TRACE_END("VXR", "Engine Systems Pre Update");
    }
  }

  void Engine::update(float dt)
  {
    if (!scene_)
    {
      return;
    }
    VXR_TRACE_BEGIN("VXR", "Engine Systems Update");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: [ENGINE] Engine Update.\n");
    custom_->update(dt);
    rigidbody_->update(dt);
    VXR_TRACE_END("VXR", "Engine Systems Update");
  }

  void Engine::postUpdate()
  {
    if (!scene_)
    {
      return;
    }
    VXR_TRACE_BEGIN("VXR", "Engine Systems Post Update");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: [ENGINE] Engine Post Update.\n");
    custom_->postUpdate();
    VXR_TRACE_END("VXR", "Engine Systems Post Update");
  }

  void Engine::renderPreUpdate()
  {
#ifdef VXR_THREADING
    gpu_->prepareRender();
#endif
    if (!scene_)
    {
      return;
    }

    VXR_TRACE_BEGIN("VXR", "Engine Systems Render Pre Update");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: [ENGINE] Engine Render Pre Update.\n");
    custom_->renderPreUpdate();
    camera_->renderPreUpdate();
    light_->renderPreUpdate();
    VXR_TRACE_END("VXR", "Engine Systems Render Pre Update");

  }

  void Engine::renderUpdate()
  {
    if (!scene_)
    {
      return;
    }
    VXR_TRACE_BEGIN("VXR", "Systems Render Update");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: [ENGINE] Engine Render Update.\n");
    custom_->renderUpdate();
    transform_->renderUpdate();
    ibl_->renderUpdate();
    camera_->renderUpdate();
    renderer_->renderUpdate();
    VXR_TRACE_END("VXR", "Systems Render Update");
  }

  void Engine::renderPostUpdate()
  {
    if (scene_.get())
    {
      VXR_TRACE_BEGIN("VXR", "Systems Render Post Update");
      VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: [ENGINE] Engine Post Render Update.\n");
      custom_->renderPostUpdate();
      renderer_->renderPostUpdate();
      camera_->renderPostUpdate();
      VXR_TRACE_END("VXR", "Systems Render Post Update");
    }
    gpu_->execute();
  }

  void Engine::stop()
  {
    gpu_->stop();
    stopSystems();
  }

  void Engine::startSystems()
  {
    VXR_TRACE_BEGIN("VXR", "Engine Systems Start");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: [ENGINE] Engine Start.\n");
    custom_->start();
    camera_->start();
    ibl_->start();
    VXR_TRACE_END("VXR", "Engine Systems Start");
  }

  void Engine::stopSystems()
  {
    VXR_TRACE_BEGIN("VXR", "Systems Stop");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: [ENGINE] Engine Stop.\n");
    custom_->stop();
    VXR_TRACE_END("VXR", "Systems Stop");
  }

  bool Engine::is_exiting()
  {
#ifdef VXR_THREADING
    return gpu()->is_exiting();
#else
    return window()->is_exiting();
#endif
  }

  void Engine::submitDisplayList(DisplayList &&dl)
  {
    gpu_->moveOrAppendCommands(std::move(dl));
  }

  void Engine::submitUIFunction(std::function<void()> function)
  {
#ifdef VXR_UI
    gpu_->window_->ui_ = function;
#endif
  }

#ifdef VXR_THREADING
  void Engine::submitAsyncTask(threading::Task& task, threading::Sync* sync)
  {
    scheduler_.run(task, sync);
  }
#endif

  ref_ptr<GPU> Engine::gpu() 
  {
    return gpu_;
  }

  ref_ptr<Window> Engine::window()
  {
    return gpu_->window_;
  }

  void Engine::loadScene(ref_ptr<Scene> scene)
  {
    VXR_TRACE_SCOPE("VXR", "Load Scene");
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [ENGINE] Loading scene...\n");
    if (!scene)
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [ENGINE] Could not load scene.\n");
      return;
    }

    stopSystems();

    scene_ = scene;

    // Update components
    ibl_->onSceneChanged();
    light_->onSceneChanged();
    custom_->onSceneChanged();
    camera_->onSceneChanged();
    renderer_->onSceneChanged();
    collider_->onSceneChanged();
    transform_->onSceneChanged();
    rigidbody_->onSceneChanged();
    mesh_filter_->onSceneChanged();

    startSystems();
  }

  ref_ptr<Scene> Engine::scene()
  {
    return scene_;
  }

  ref_ptr<AssetManager> Engine::assetManager()
  {
    return asset_manager_;
  }

  ref_ptr<System::IBL> Engine::ibl()
  {
    return ibl_;
  }

  ref_ptr<System::Light> Engine::light()
  {
    return light_;
  }

  ref_ptr<System::Custom> Engine::custom()
  {
    return custom_;
  }

  ref_ptr<System::Camera> Engine::camera()
  {
    return camera_;
  }

  ref_ptr<System::Renderer> Engine::renderer()
  {
    return renderer_;
  }

  ref_ptr<System::Collider> Engine::collider()
  {
    return collider_;
  }

  ref_ptr<System::Transform> Engine::transform()
  {
    return transform_;
  }

  ref_ptr<System::Rigidbody> Engine::rigidbody()
  {
    return rigidbody_;
  }

  ref_ptr<System::MeshFilter> Engine::meshFilter()
  {
    return mesh_filter_;
  }

} /* end of vxr namespace */