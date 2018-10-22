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

#include "mesh.h"

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

    cam_.alloc()->set_name("Camera");
    cam_->addComponent<Camera>()->set_background_color(Color(0.0f, 0.125f, 0.3f, 1.0f));
    cam_->transform()->set_local_position(vec3(0,0,4));
    scene_->addObject(cam_);

    mesh_ = Asset::loadModelOBJ("../../assets/meshes/obj/teapot_mesh.obj");
    mesh_->set_name("Teapot");
    mesh_->transform()->set_local_rotation(vec3(glm::radians(90.0f), 0, 0));
    scene_->addObject(mesh_);

    Application::start();
  }

  void Main::update()
  {
    mesh_->transform()->set_local_rotation(mesh_->transform()->local_rotation() + vec3(0.21, 0.12, 0.5) * deltaTime());

    Application::update();
  }

  void Main::renderUpdate()
  {
    updateWindowTitle();

    Engine::ref().submitUIFunction([this]() { ui::Editor(); });

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
      "VXR Mesh Example",
      fps());

    Engine::ref().window()->set_title(window_title_);
  }

} /* end of vxr namespace */