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

#include "planet_editor.h"

#include "../../include/graphics/ui.h"
#include "../../include/core/assets.h"

VXR_DEFINE_APP_MAIN(vxr::Main)

namespace vxr
{

  Main::Main()
  {
    window_title_ = (char*)malloc(512 + strlen("VXR Instancing Test"));
    Params p;
    p.gpu = { 100, 100, 100, 100 };
    p.window = { { 1920, 1080} };
    Engine::ref().set_preinit_params(p);
  }

  void Main::init()
  {
    Application::init();
  }

  void Main::start()
  {
    ref_ptr<Scene> scene_;
    scene_.alloc();
    Engine::ref().loadScene(scene_);
    cam_.alloc();
    cam_->addComponent<Camera>();
    cam_->transform()->set_local_position(zoom_off);
    scene_->addObject(cam_);
    planet_.alloc()->addComponent<Planet, Custom>();
    scene_->addObject(planet_);

    Application::start();
  }

  void Main::update()
  {
    Application::update();
  }


  static vec3 vec3_lerp(vec3 a, vec3 b, float t) {
    float ax = a[0],
      ay = a[1],
      az = a[2];
    vec4 out;
    out[0] = ax + t * (b[0] - ax);
    out[1] = ay + t * (b[1] - ay);
    out[2] = az + t * (b[2] - az);
    return out;
  }

  void Main::renderUpdate()
  {
    updateWindowTitle();

    float speed = 0.1f;
    if (cam_->transform()->local_position() != (planet_->getComponent<Planet>()->zoom ? zoom_on : zoom_off))
    {
      cam_->transform()->set_local_position(vec3_lerp(cam_->transform()->local_position(), (planet_->getComponent<Planet>()->zoom ? zoom_on : zoom_off), speed));
    }

    Engine::ref().submitUIFunction([this]() { ui::Editor(); });
    //Engine::ref().submitUIFunction([this]() { planet_->onGUI(); });

    Application::renderUpdate();
  }

  void Main::stop()
  {
    Application::stop();
  }

  void Main::updateWindowTitle()
  {
    sprintf(window_title_,
      "%s: %d FPS @ 1920 x 1080",
      "VXR dev",
      fps());

    Engine::ref().window()->set_title(window_title_);
  }

} /* end of vxr namespace */