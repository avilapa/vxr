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
#include "../../include/components/mesh_filter.h"
#include "../../include/components/camera.h"
#include "../../include/components/custom.h"

#include <time.h>

namespace vxr 
{

  Engine& Engine::ref()
  {
    static Engine *inst = new Engine();
    return *inst;
  }

  Engine::Engine() /// variables
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
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: Initializing engine systems.\n");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Engine Init.\n");
    // Initialize systems
    gpu_->init();
    asset_manager_->init();

    ibl_.alloc();
    light_.alloc();
    custom_.alloc();
    camera_.alloc();
    renderer_.alloc();
    transform_.alloc();
    mesh_filter_.alloc();

    light_->init();
    camera_->init();

    VXR_TRACE_END("VXR", "Engine Systems Init");
    return true;
  }

  void Engine::start()
  {
    VXR_TRACE_BEGIN("VXR", "Engine Systems Start");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Engine Start.\n");
    custom_->start();
    VXR_TRACE_END("VXR", "Engine Systems Start");
  }

  void Engine::update()
  {
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Engine Update.\n");
    custom_->update();
    transform_->update();
    ibl_->update();
    light_->update();
    camera_->update();
    renderer_->update();
  }

  void Engine::postUpdate()
  {
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Engine Post Update.\n");
    custom_->postUpdate();
  }

  void Engine::renderUpdate()
  {
#ifdef VXR_THREADING
    gpu_->prepareRender();
#endif
    VXR_TRACE_BEGIN("VXR", "Systems Render Update");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Engine Render Update.\n");
    custom_->renderUpdate();
    transform_->renderUpdate();
    ibl_->renderUpdate();
    light_->renderUpdate();
    camera_->renderUpdate();
    renderer_->renderUpdate();
    VXR_TRACE_END("VXR", "Systems Render Update");
  }

  void Engine::renderPostUpdate()
  {
    VXR_TRACE_BEGIN("VXR", "Systems Render Post Update");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Engine Post Render Update.\n");
    custom_->renderPostUpdate();
    transform_->renderPostUpdate();
    renderer_->renderPostUpdate();
    camera_->renderPostUpdate();
    VXR_TRACE_END("VXR", "Systems Render Post Update");
    gpu_->execute();
  }

  void Engine::stop()
  {
    gpu_->stop();

    custom_->stop();
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
    gpu_->ui_ = function;
#endif
  }

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
    scene_ = scene;
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

  ref_ptr<System::Transform> Engine::transform()
  {
    return transform_;
  }

  ref_ptr<System::MeshFilter> Engine::meshFilter()
  {
    return mesh_filter_;
  }

} /* end of vxr namespace */