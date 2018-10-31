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

// 0. Define the entry point.
VXR_DEFINE_APP_MAIN(vxr::Main)

namespace vxr
{

  Main::Main()
  {
    // 1. Initialize GPU and Window parameters.
    Params p;
    p.gpu = { 100, 100, 100, 100 };
    p.window = { { 1920, 1080} };
    Engine::ref().set_preinit_params(p);
  }

  void Main::start()
  {
    // 2. Create a camera.
    cam_.alloc()->set_name("Camera");
    cam_->addComponent<Camera>()->set_background_color(Color(0.03f, 0.03f, 0.05f, 1.0f));
    
    // 3. Create a Planet
    planet_.alloc()->set_name("Planet");
    planet_->addComponent<Planet, Custom>();

    // 4. Create a Scene, parent the objects and load.
    ref_ptr<Scene> scene_;
    scene_.alloc()->set_name("Planet Editor Scene");

    scene_->addObject(cam_);
    scene_->addObject(planet_);

    Engine::ref().loadScene(scene_);

    // 5. Set UI function
    Engine::ref().submitUIFunction([this]() { ui::Editor(); });

    Application::start();
  }

  void Main::update()
  {
    // 6. Interpolate camera position
    float speed = 5.0f;
    if (cam_->transform()->local_position() != (planet_->getComponent<Planet>()->zoom ? zoom_on : zoom_off))
    {
      cam_->transform()->set_local_position(Math::lerp(cam_->transform()->local_position(), (planet_->getComponent<Planet>()->zoom ? zoom_on : zoom_off), speed * deltaTime()));
    }

    Application::update();
  }

} /* end of vxr namespace */