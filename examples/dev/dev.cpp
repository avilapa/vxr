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
    cam_->addComponent<Camera>()->set_background_color(Color(0.06f, 0.06f, 0.06f, 1.0f));
    cam_->transform()->set_local_position(vec3(0.0f, 0.8f, 3.0f));
    cam_->transform()->set_local_rotation(vec3(-0.28f, 0, 0));
    scene_->addObject(cam_);
    Engine::ref().camera()->set_main(cam_->getComponent<Camera>());

    /*ref_ptr<Texture> pa;
    pa.alloc()->load("../../assets/textures/marble/albedo.tga");
    ref_ptr<Texture> pr;
    pr.alloc()->load("../../assets/textures/marble/roughness.tga");
    ref_ptr<Texture> pn;
    pn.alloc()->load("../../assets/textures/marble/normal.tga");

    ref_ptr<mesh::Quad> plane;
    plane.alloc();
    ref_ptr<mat::Std::Instance> plane_mat;
    plane_mat.alloc();
    plane_mat->set_metallic(0.0f);
    plane_mat->set_albedo(pa);
    plane_mat->set_roughness(pr);
    plane_mat->set_normal(pn);

    objects_floor_.alloc()->set_name("Objects Floor");
    objects_floor_->addComponent<MeshFilter>()->mesh = plane.get();
    objects_floor_->addComponent<Renderer>()->material = plane_mat.get();

    objects_floor_->transform()->set_local_rotation(vec3(-1.57f, 0, 0));
    objects_floor_->transform()->set_local_position(vec3(0, -0.57f, 0));
    objects_floor_->transform()->set_local_scale(vec3(1.5f));

    scene_->addObject(objects_floor_);*/

    ref_ptr<Texture> ma = Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/albedo.png");
    ref_ptr<Texture> mm = Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/metallic.png");
    ref_ptr<Texture> mr = Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/roughness.png");
    ref_ptr<Texture> mo = Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/ao.png");
    //ref_ptr<Texture> mn = Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/normal_2k.png");

    //ref_ptr<Texture> test = Engine::ref().assetManager()->loadTexture("../../assets/textures/noise/noise_swirl.jpg");
    //ref_ptr<Texture> testn = Engine::ref().assetManager()->loadTexture("../../assets/textures/normal.jpg");

    ref_ptr<Mesh> mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/substance_sphere/substance_sphere.obj");
    mesh->set_name("Substance Sphere");
    for (uint32 i = 0; i < 1; ++i)
    {
      for (uint32 j = 0; j < 1; ++j)
      {
        ref_ptr<mat::Std::Instance> mat;
        mat.alloc();
        mat->set_metallic((float)j / 4.0f);
        mat->set_roughness((float)i / 9.0f);
        mat->set_albedo(Color::White);

        mat->set_albedo(ma);
        mat->set_metallic(mm);
        mat->set_roughness(mr);
        mat->set_ambient_occlusion(mo);
        //mat->set_normal(mn);
        //mat->set_clear_coat_normal(testn);
        //mat->set_iridescence_thickness(test);

        ref_ptr<GameObject> obj;
        obj.alloc();
        obj->addComponent<MeshFilter>()->mesh = mesh.get();
        obj->addComponent<Renderer>()->material = mat.get();

        //obj->transform()->set_local_position(vec3(((float)i - 4.5f), ((float)j - 2.0f), 0));
        obj->transform()->set_local_rotation(vec3(0, 0.5f, 0));
        obj->transform()->set_local_scale(vec3(1.0));
        scene_->addObject(obj);

      }
    }

    ref_ptr<GameObject> dir_light;
    dir_light.alloc()->set_name("Directional Light");
    dir_light->addComponent<Light>()->set_type(Light::Type::Directional);
    dir_light->transform()->set_local_rotation(vec3(0.4, -0.5, -0.8));
    scene_->addObject(dir_light);

    light_node_.alloc()->set_name("Light Node");
    scene_->addObject(light_node_);

    ref_ptr<Mesh> light_mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/sphere/sphere.obj");
    light_mesh->set_name("Light Mesh");

    for (uint32 i = 0; i < NUM_LIGHTS; ++i)
    {
      Color light_color = Color::Random().desaturate(-0.5);
      ref_ptr<mat::Unlit::Instance> light_mat;
      light_mat.alloc()->set_color(light_color);
      light_[i].alloc()->set_name("Light");
      light_[i]->addComponent<Light>()->set_color(light_color);
      light_[i]->getComponent<Light>()->set_intensity(1.0f);
      light_[i]->getComponent<Light>()->set_falloff(0.0001f);
      light_[i]->transform()->set_parent(light_node_->transform());
      light_[i]->addComponent<Renderer>()->material = light_mat.get();
      light_[i]->addComponent<MeshFilter>()->mesh = light_mesh;
      light_[i]->transform()->set_local_scale(vec3(0.12f));
      vec3 rand_pos = vec3(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f) * 2.0f - 1.0f;
      rand_pos = glm::normalize(rand_pos - light_node_->transform()->local_position()) + 0.5f;
      light_[i]->transform()->set_local_position(rand_pos);
    }

    ref_ptr<mat::Negative::Instance> m1;
    m1.alloc();
    //Engine::ref().camera()->main()->composer()->addRenderPass(m1.get());

    ref_ptr<mat::Grayscale::Instance> m2;
    m2.alloc();
    //Engine::ref().camera()->main()->composer()->addRenderPass(m2.get());

    Engine::ref().submitUIFunction([]() { ui::Editor(); });
  }

  void Main::update()
  {
    Application::update();
    //light_node_->transform()->set_local_rotation(light_node_->transform()->local_rotation() + vec3(0.31, 1.06, 0.3) * deltaTime());
    //light_node_->transform()->set_local_scale(vec3(1 + sin(Engine::ref().window()->uptime() * 0.1f)));
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