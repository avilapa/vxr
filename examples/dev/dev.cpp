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

#include "dev.h"

VXR_DEFINE_APP_MAIN(vxr::Main)

namespace vxr
{

  Main::Main()
  {
    Params p;
    p.gpu = { 100, 100, 100, 100 };
    p.window = { { 1280, 720} };
    Engine::ref().set_preinit_params(p);
  }

  void Main::init()
  {
    Application::init();
  }

  void Main::start()
  {
    ref_ptr<Scene> scene_;
    scene_.alloc()->set_name("Test Scene");
    Engine::ref().loadScene(scene_);

    cam_.alloc()->set_name("Camera");
    cam_->addComponent<Camera>()->set_background_color(Color::Black);
    cam_->transform()->set_local_position(vec3(0.0f, 0.0f, 1.8f));
    scene_->addObject(cam_);
    Engine::ref().camera()->set_main(cam_->getComponent<Camera>());

    ref_ptr<EngineMesh::Cube> cube;
    ref_ptr<mat::Standard::Instance> mat;
    ref_ptr<mat::Standard::Instance> matc;
    obj_.alloc();
    cube.alloc();
    mat.alloc()->set_color_texture("../../assets/textures/image.tga");
    //matc.alloc()->set_color_texture("../../assets/textures/skybox/sunset", "png");

    obj_->addComponent<Renderer>()->material = mat.get();
    obj_->addComponent<MeshFilter>()->mesh = cube.get();

    scene_->addObject(obj_);

    light_node_.alloc()->set_name("Light Node");
    scene_->addObject(light_node_);

    for (uint32 i = 0; i < NUM_LIGHTS; ++i)
    {
      Color light_color = Color::Random().desaturate(-0.5);
      ref_ptr<mat::Unlit::Instance> light_mat;
      light_mat.alloc()->set_color(light_color);
      light_[i].alloc()->set_name("Light");
      light_[i]->addComponent<Light>()->set_color(light_color);
      light_[i]->getComponent<Light>()->set_intensity(1.0f);
      light_[i]->transform()->set_parent(light_node_->transform());
      light_[i]->addComponent<Renderer>()->material = light_mat.get();
      light_[i]->addComponent<MeshFilter>()->mesh = cube.get();
      light_[i]->transform()->set_local_scale(vec3(0.12f));
      vec3 rand_pos = vec3(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f) * 2.0f - 1.0f;
      rand_pos = glm::normalize(rand_pos - light_node_->transform()->local_position());
      light_[i]->transform()->set_local_position(rand_pos);
    }

    ref_ptr<mat::Negative::Instance> m1;
    m1.alloc();
    Engine::ref().camera()->main()->composer()->addRenderPass(m1.get());

    ref_ptr<mat::Grayscale::Instance> m2;
    m2.alloc();
    Engine::ref().camera()->main()->composer()->addRenderPass(m2.get());

    Engine::ref().submitUIFunction([]() { ui::Editor(); });
  }

  void Main::update()
  {
    Application::update();
    obj_->transform()->set_local_rotation(obj_->transform()->local_rotation() + vec3(0.21, 0.12, 0.5) * deltaTime());
    light_node_->transform()->set_local_rotation(light_node_->transform()->local_rotation() + vec3(0.31, 1.06, 0.3) * deltaTime());
    light_node_->transform()->set_local_scale(vec3(1 + sin(Engine::ref().window()->uptime() * 0.1f)));
  }

  void Main::renderUpdate()
  {
    Application::renderUpdate();
  }

  void Main::stop()
  {
    Application::stop();
  }

} /* end of vxr namespace */