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

// 0. Define the entry point.
VXR_DEFINE_APP_MAIN(vxr::Main)

namespace vxr
{

  Main::Main()
  {
    // 1. Initialize GPU and Window parameters.
    Params p;
    p.gpu = { 100, 100, 100, 100 };
    p.window = { { 1280, 720 } };
    Engine::ref().set_preinit_params(p);
  }

  void Main::start()
  {
    // 2. Create a Scene, parent the objects and load.
    ref_ptr<Scene> scene_;
    scene_.alloc();
    Engine::ref().loadScene(scene_);

    ref_ptr<Skybox> skybox;
    skybox.alloc()->set_name("Default Skybox");
    ref_ptr<Texture> input;
    input = Engine::ref().assetManager()->loadTexture("../../assets/environments/goldenhour", "tga");
    skybox->set_texture(input);
    scene_->set_skybox(skybox);

    // 3. Create a camera.
    cam_.alloc()->set_name("Camera");
    cam_->addComponent<Camera>()->transform()->set_local_position(vec3(0.0f, 0.0f, 4.0f));
    cam_->getComponent<Camera>()->set_clear_flags(Camera::ClearFlags::Skybox);
    scene_->addObject(cam_);

    // 4. Load the Teapot mesh and setup a rough metallic material.
    teapot_.alloc()->set_name("Teapot");
    teapot_->transform()->set_local_position(vec3(-2.0f, 0.0f, 0.0f));
    teapot_->transform()->set_local_rotation(vec3(90.0f, 0.0f, 0.0f));
    teapot_->transform()->set_local_scale(vec3(0.5f));

    ref_ptr<mat::Std::Instance> teapot_mat;
    teapot_mat.alloc();
    teapot_mat->set_albedo(Color::ClearWater);
    teapot_mat->set_metallic(1.0f);
    teapot_mat->set_roughness(0.5f);
    teapot_mat->set_clear_coat(1.0f);
    teapot_mat->set_clear_coat_roughness(0.5f);
    teapot_mat->set_iridescence(0.5f);

    teapot_->addComponent<Renderer>()->material = teapot_mat.get();
    teapot_->addComponent<MeshFilter>()->mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/teapot/teapot.obj");

    scene_->addObject(teapot_);

    // 5. Load the Sphere mesh and setup a reflective red plastic material.
    sphere_.alloc()->set_name("Sphere");
    sphere_->transform()->set_local_position(vec3(0.0f, 0.0f, 0.0f));
    sphere_->transform()->set_local_scale(vec3(1.4f));

    ref_ptr<mat::Std::Instance> sphere_mat;
    sphere_mat.alloc();
    sphere_mat->set_albedo(Color::Red);
    sphere_mat->set_metallic(0.0f);
    sphere_mat->set_roughness(0.0f);
    sphere_mat->set_reflectance(0.8f);

    sphere_->addComponent<Renderer>()->material = sphere_mat.get();
    sphere_->addComponent<MeshFilter>()->mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/sphere/sphere.obj");

    scene_->addObject(sphere_);

    // 6. Load the Suzanne mesh and setup the corresponding textures for the material.
    suzanne_.alloc()->set_name("Suzanne");
    suzanne_->transform()->set_local_position(vec3(2.0f, 0.0f, 0.0f));
    suzanne_->transform()->set_local_scale(vec3(0.7f));

    ref_ptr<mat::Std::Instance> suzanne_mat;
    suzanne_mat.alloc();
    suzanne_mat->set_albedo(Engine::ref().assetManager()->loadTexture("../../assets/models/suzanne/albedo.png"));
    suzanne_mat->set_metallic(Engine::ref().assetManager()->loadTexture("../../assets/models/suzanne/metallic.png"));
    suzanne_mat->set_roughness(Engine::ref().assetManager()->loadTexture("../../assets/models/suzanne/roughness.png"));
    suzanne_mat->set_normal(Engine::ref().assetManager()->loadTexture("../../assets/models/suzanne/normal.png"));
    suzanne_mat->set_ambient_occlusion(Engine::ref().assetManager()->loadTexture("../../assets/models/suzanne/ao.png"));
    suzanne_mat->set_clear_coat(0.0f);

    suzanne_->addComponent<Renderer>()->material = suzanne_mat.get();
    suzanne_->addComponent<MeshFilter>()->mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/suzanne/suzanne.obj");

    scene_->addObject(suzanne_);

    // 7. Create a directional light
    light_.alloc()->set_name("Sun");
    light_->addComponent<Light>()->set_type(Light::Type::Directional);
    light_->getComponent<Light>()->set_color(Color::White);
    light_->transform()->set_local_rotation(vec3(2.34f, -0.2f, -1.0f));
    scene_->addObject(light_);

    // 8. Submit the UI function.
    Engine::ref().submitUIFunction([this]() { ui::Editor(); });

    Application::start();
  }

  void Main::update()
  {
    // 9. Rotate the meshes in update() instead of renderUpdate() to make the transformations framerate
    // independent by multiplying it by deltaTime().The update() method may be executed several times in
    // a frame to catch up with the render thread.
    float t = Engine::ref().window()->uptime();
    
    teapot_->transform()->rotateX(30.0f * deltaTime());
    teapot_->transform()->rotateZ(25.0f * deltaTime());
    teapot_->transform()->set_world_position(vec3(-2, cos(t) * 100 * deltaTime(), 0));

    suzanne_->transform()->rotate(vec3(-15.0f, 10.0f, -5.0f) * deltaTime());
    suzanne_->transform()->set_world_position(vec3(2, sin(t) * 100 * deltaTime(), 0));
    
    sphere_->transform()->set_local_scale(vec3(sin(t) * 100 * deltaTime() * 0.5f + 1.0f));

    cam_->transform()->rotateAround(sphere_, vec3(0,10,0) * deltaTime());

    Application::update();
  }

} /* end of vxr namespace */