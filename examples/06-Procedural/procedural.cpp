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

#include "procedural.h"

// 0. Define the entry point.
VXR_DEFINE_APP_MAIN(vxr::Main)

namespace vxr
{

  Main::Main()
  {
    // 1. Initialize GPU and Window parameters.
    Params p;
    p.gpu = { 100, 100, 100, 100 };
    p.window = { { 1280, 720} };
    Engine::ref().set_preinit_params(p);
  }

  void Main::start()
  {
    // 2. Create a Scene with a skybox and a camera.
    ref_ptr<Scene> scene_;
    scene_.alloc()->set_name("Planet Editor Scene");
    Engine::ref().loadScene(scene_);

    ref_ptr<Skybox> skybox;
    skybox.alloc()->set_name("Material Editor Skybox");
    ref_ptr<Texture> environment = Engine::ref().assetManager()->loadTexture("../../assets/environments/milky_way.jpg", true);
    skybox->set_texture(environment);
    scene_->set_skybox(skybox);

    cam_.alloc()->set_name("Camera");
    cam_->addComponent<Camera>()->set_background_color(Color(0.03f, 0.03f, 0.05f, 1.0f));
    scene_->addObject(cam_);

    // 3. Create a Planet and add the custom material to the library.
    Engine::ref().assetManager()->addMaterial<mat::Planet>();
    planet_.alloc()->set_name("Planet");
    planet_->addComponent<Planet, Custom>();
    scene_->addObject(planet_);

    // 4. Create a directional light
    light_.alloc()->set_name("Sun");
    light_->addComponent<Light>()->set_type(Light::Type::Directional);
    light_->getComponent<Light>()->set_color(Color::CornSilk);
    light_->getComponent<Light>()->set_intensity(1.5f);
    light_->transform()->set_local_rotation(vec3(40.0f, 0.0f, 0.0f));
    scene_->addObject(light_);

    // 5. Set UI function
    Engine::ref().submitUIFunction([this]() { ui::Editor(); });

    Application::start();
  }

  void Main::update()
  {
    // 7. Interpolate camera position
    float speed = 5.0f;
    static bool first_zoom = true;
    if (planet_->getComponent<Planet>()->zoom)
    {
      first_zoom = true;
      cam_->transform()->set_local_position(Math::lerp(cam_->transform()->local_position(), zoom_on, speed * deltaTime()));
      cam_->transform()->set_local_rotation(Math::lerp(cam_->transform()->local_rotation_angles(), vec3(0.0f), speed * deltaTime()));
    }
    else
    {
      if (first_zoom)
      {
        cam_->transform()->set_local_position(Math::lerp(cam_->transform()->local_position(), zoom_off, speed * deltaTime()));
        if (glm::epsilonEqual(cam_->transform()->local_position(), zoom_off, 0.001f) == bvec3(true))
        {
          first_zoom = false;
        }
      }
      else
      {
        cam_->transform()->rotateAround(planet_, vec3(0.1f, 0.2f, -0.2f) * deltaTime());
      }    
    }
    Application::update();
  }

} /* end of vxr namespace */