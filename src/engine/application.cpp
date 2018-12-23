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
#include "../../include/engine/engine.h"
#include "../../include/graphics/window.h"

namespace vxr 
{

  Application::Application()
  {
    set_name("Application");
  }

  Application::~Application() 
  {
  }

  void Application::init() 
  {
    if (!Engine::ref().init())
    {
      exit_ = true;
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Initializing engine.\n");
      return;
    }
  }

  void Application::start() 
  {
    Engine::ref().start();
  }

  void Application::update() 
  {
    Engine::ref().update();
  }

  void Application::postUpdate()
  {
    Engine::ref().postUpdate();
  }

  void Application::renderUpdate()
  {
    Engine::ref().renderUpdate();
  }

  void Application::renderPostUpdate()
  {
    Engine::ref().renderPostUpdate();
  }

  void Application::stop() 
  {
    Engine::ref().stop();
  }

  bool Application::is_exiting()
  {
    return Engine::ref().is_exiting();
  }

  uint32 Application::run(int argc, char** argv) 
  {
#ifdef VXR_DEBUG_TRACING
    mtr_init("trace.json");
#endif
    VXR_TRACE_META_THREAD_NAME("Logic Thread");
    VXR_TRACE_SCOPE("VXR", __FUNCTION__);

    VXR_TRACE_BEGIN("VXR", "App Init");
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: Starting application.\n");
    init();

    if (exit_)
    {
      return 0;
    }
    VXR_TRACE_END("VXR", "App Init");
    VXR_TRACE_BEGIN("VXR", "App Start");
    start();
    VXR_TRACE_END("VXR", "App Start");
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: Reached first application loop.\n");

    double lastTime = Engine::ref().window()->uptime();
    double accumulator = 0.0;
    double uptime = 0.0;
    uint32 num_updates = 0;
    uint32 num_frames = 0;

    while (!is_exiting())
    {
      VXR_TRACE_BEGIN("VXR", "Frame");
      VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: New render frame (FPS: %d) (Logic updates: %d)\n", framerate_.fps, num_updates);
      double nowTime = Engine::ref().window()->uptime();
      double deltaTime = nowTime - lastTime;
      lastTime = nowTime;
      
      if ((Engine::ref().window()->uptime() - uptime) > 1.0)
      {
        ++uptime;
        framerate_.fps = num_frames;
        num_frames = 0;
      }

      num_updates = 0;
      accumulator += deltaTime;
      VXR_TRACE_BEGIN("VXR", "Loop Update");
      while (accumulator > framerate_.time_step)
      {
        if (num_updates >= framerate_.max_steps)
        {
          //VXR_LOG(VXR_DEBUG_LEVEL_WARNING, "[WARNING]: Exceeded max steps per frame (Consider lowering time_step).\n");
          break;
        }
        VXR_TRACE_BEGIN("VXR", "App Update");
        update();
        postUpdate();
        VXR_TRACE_END("VXR", "App Update");
        accumulator -= framerate_.time_step;
        num_updates++;
      }
      VXR_TRACE_END("VXR", "Loop Update");
      
      VXR_TRACE_BEGIN("VXR", "Render Update");
      renderUpdate();
      renderPostUpdate();
      VXR_TRACE_END("VXR", "Render Update");
      ++num_frames;
      VXR_TRACE_END("VXR", "Frame");
    }

    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: Stopping application.\n");
    stop();
#ifdef VXR_DEBUG_TRACING
    mtr_shutdown();
#endif
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: Stopped application with exit code 0.\n");
    return 0;
  }


  void Application::set_time_step(double time_step)
  {
    framerate_.time_step = time_step;
  }

  void Application::set_max_steps(uint32 steps)
  {
    framerate_.max_steps = steps;
  }

  float Application::deltaTime()
  {
    return (float)framerate_.time_step;
  }

  uint32 Application::fps()
  {
    return framerate_.fps;
  }

}