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

#include "physics.h"

#include "../../include/physics/collider_sphere.h"
#include "../../include/components/rigidbody.h"
#include "../../include/components/collider.h"

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
    scene_.alloc();
    Engine::ref().loadScene(scene_);

    // 2.1. Create a skybox.
    ref_ptr<Skybox> skybox;
    skybox.alloc();
    ref_ptr<Texture> background;
    background.alloc()->set_size(1, 1);
    background->set_texels_format(TexelsFormat::RGB_U8);
    background->set_type(TextureType::T2D);
    background->set_data(Color::PhyreBlue);
    background->set_hdr(true);
    skybox->set_texture(background);
    scene_->set_skybox(skybox);

    // 2.2. Add Cameras.
    camera_[0].alloc()->set_name("Camera");
    camera_[0]->addComponent<Camera>()->transform()->set_local_position(vec3(0.0f, cam_height_, 35.0f));
    camera_[0]->getComponent<Camera>()->transform()->set_local_rotation(vec3(cam_incl_, 0.0f, 0.0f));
    camera_[0]->getComponent<Camera>()->set_background_color(Color::PhyreBlue);
    camera_[0]->getComponent<Camera>()->set_clear_flags(Camera::ClearFlags::Skybox);
    scene_->addObject(camera_[0]);

    camera_[1].alloc()->set_name("Camera Top");
    camera_[1]->addComponent<Camera>()->transform()->set_local_position(vec3(0.0f, 85.0f, 0.0f));
    camera_[1]->getComponent<Camera>()->transform()->set_local_rotation(vec3(-90.0f, 0.0f, 0.0f));
    camera_[1]->getComponent<Camera>()->set_fov(25.0f);
    camera_[1]->getComponent<Camera>()->set_background_color(Color::PhyreBlue);
    camera_[1]->getComponent<Camera>()->set_clear_flags(Camera::ClearFlags::Skybox);
    scene_->addObject(camera_[1]);

    // 2.3. Load all height map textures into meshes.
    height_map_[0].alloc()->loadHeightMap("assets/heightmap.bmp", 2.0f, 0.75f, 2.0f);
    height_map_[1].alloc()->loadHeightMap("assets/heightmap.bmp", 2.0f, 0.75f);
    height_map_[2].alloc()->loadHeightMap("assets/circle.bmp", 2.0f, 0.75f);
    height_map_[3].alloc()->loadHeightMap("assets/fall.bmp", 2.0f, 0.75f);
    height_map_[4].alloc()->loadHeightMap("assets/diagonal.bmp", 2.0f, 0.75f);

    // 2.4. Create the different materials for the height maps.
    collider_materials_[0].allocT<mat::Wireframe::Instance>()->set_color(Color::White);
    collider_materials_[1].allocT<mat::Std::Instance>()->set_albedo(Color::Gold);

    // 2.5. Create the height map object, with a Rigidbody and add its physical properties.
    collider_.alloc()->set_name("Collider");
    collider_->addComponent<Renderer>()->material = collider_materials_[current_collider_material_];
    collider_->addComponent<MeshFilter>()->mesh = height_map_[current_height_map_].get();
    collider_->addComponent<Rigidbody>()->set_use_gravity(false);
    collider_->getComponent<Rigidbody>()->set_mass(0.0f);
    collider_->getComponent<Rigidbody>()->set_restitution(1.0f);

    // 2.6. Add a Collider component and set the shape to a height map collider with the proper mesh assigned.
    collider_hm_.alloc()->set_height_map(height_map_[current_height_map_]);
    collider_hm_->setAabbExtents(vec3(-30.0f), vec3(30.0f));
    collider_->addComponent<Collider>()->set_shape(collider_hm_.get());

    scene_->addObject(collider_);

    // 2.7. Add a directional light to the scene.
    ref_ptr<GameObject> dir_light;
    dir_light.alloc()->set_name("Directional Light");
    dir_light->addComponent<Light>()->set_type(Light::Type::Directional);
    dir_light->transform()->set_local_rotation(vec3(90.0f, 60.0f, 90.0f));
    scene_->addObject(dir_light);

    // 3. Submit the UI function.
    /// TODO: Heavy custom UI work can cause a crash due to UI not being executed thread safely at the moment.
    Engine::ref().submitUIFunction([this]() { Input(); ui::Editor(); });

    Application::start();
  }

  void Main::update(float dt)
  {
    // 4. Bring back up spheres that fall below a certain height.
    for (auto& sphere : spheres_)
    {
      if (sphere->transform()->local_position().y < -15.0f)
      {
        vec3 random_pos = vec3(rand() % 28 - 14, 40.0f, rand() % 28 - 14);
        RespawnObject(sphere, random_pos);
      }
    }

    Application::update(dt);
  }

  void Main::SpawnNewSphere(const vec3& position, const float& scale, const Color& color)
  {
    ref_ptr<GameObject> sphere;
    sphere.alloc()->set_name("Sphere");
    sphere->transform()->set_local_position(position);
    sphere->transform()->set_local_scale(vec3(scale * 0.05f));

    ref_ptr<mat::Std::Instance> std_sphere_mat;
    std_sphere_mat.alloc()->set_albedo(color);
    sphere->addComponent<Renderer>()->material = std_sphere_mat.get();
    sphere->addComponent<MeshFilter>()->mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/sphere/sphere.obj");
    sphere->addComponent<Rigidbody>()->set_mass(scale * 0.5f);
    sphere->getComponent<Rigidbody>()->set_restitution(0.4f);
    ref_ptr<ColliderSphere> coll;
    coll.alloc()->set_radius(scale * 0.5f);
    sphere->addComponent<Collider>()->set_shape(coll.get());

    scene_->addObject(sphere);
    spheres_.push_back(sphere);
  }

  void Main::RespawnObject(ref_ptr<GameObject> obj, const vec3& position)
  {
    obj->transform()->set_local_position(position);
    
    ref_ptr<Rigidbody> rg = obj->getComponent<Rigidbody>().get();
    if (rg != nullptr)
    {
      rg->set_velocity(vec3(0.0f));
    }
  }

  void Main::Input()
  {
    // Change collider material.
    if (ImGui::IsKeyPressed('W'))
    {
      if (++current_collider_material_ >= NUM_COLLIDER_MATERIALS)
      {
        current_collider_material_ = 0;
      }

      collider_->getComponent<Renderer>()->material = collider_materials_[current_collider_material_];
    }

    // Change active camera.
    if (ImGui::IsKeyPressed('C'))
    {
      if (++current_camera_ >= NUM_CAMERAS)
      {
        current_camera_ = 0;
      }

      Engine::ref().camera()->set_main(camera_[current_camera_]->getComponent<Camera>());
    }

    // Handle camera rotation
    if (ImGui::IsKeyDown('Q'))
    {
      cam_height_ += rotation_speed_;
    }
    if (ImGui::IsKeyDown('A'))
    {
      cam_height_ -= rotation_speed_;
    }

    vec3 collider_pos = collider_->transform()->local_position();
    collider_pos.y = cam_height_;
    quat cam_incl = camera_[0]->transform()->local_rotation();
    cam_incl = quat(cam_incl.w, -cam_incl.x, 0.0f, 0.0f);
    camera_[0]->transform()->rotate(cam_incl);

    if (ImGui::IsKeyDown('O'))
    {
      camera_[0]->transform()->rotateAround(collider_pos, vec3(0.0f, rotation_speed_, 0.0f));
    }
    if (ImGui::IsKeyDown('P'))
    {
      camera_[0]->transform()->rotateAround(collider_pos, vec3(0.0f,-rotation_speed_, 0.0f));
    }

    cam_incl = quat(cam_incl.w, -cam_incl.x, 0.0f, 0.0f);
    camera_[0]->transform()->rotate(cam_incl);
    camera_[0]->transform()->set_local_position_y(cam_height_);

    // Slow down physics to 1fps
    if (ImGui::IsKeyDown(' '))
    {
      Engine::ref().rigidbody()->set_debug_update(true, 1.0, deltaTime());
    }
    else
    {
      Engine::ref().rigidbody()->set_debug_update(false, 1.0, deltaTime());
    }
    
    // Spawn opposable spheres
    if (ImGui::IsKeyPressed('F'))
    {
      SpawnNewSphere(vec3(+14.0f, 30.0f, 0.0f), 2.0f, Color::Red);
      SpawnNewSphere(vec3(-14.0f, 30.0f, 0.0f), 2.0f, Color::Red);
    }

    // (UP) Spawn random sphere
    if (ImGui::IsKeyPressed(265) || ImGui::IsKeyPressed('R'))
    {
      float random_scale = 1.0f + (rand() % 30 * 0.1f);
      vec3 random_pos = vec3(rand() % 28 - 14, 30.0f, rand() % 28 - 14);
      if (current_first_sphere_ > 0)
      {
        current_first_sphere_--;
        spheres_[current_first_sphere_]->set_active(true);
        RespawnObject(spheres_[current_first_sphere_], random_pos);
      }
      else
      {
        SpawnNewSphere(random_pos, random_scale, Color::Random());
      }
    }

    // (DOWN) Hide first sphere
    if (ImGui::IsKeyPressed(264))
    {
      if (current_first_sphere_ <= spheres_.size() - 1)
      {
        spheres_[current_first_sphere_]->set_active(false);
        current_first_sphere_++;
      }
    }

    // Change active height map
    if (ImGui::IsKeyPressed('H'))
    {
      if (++current_height_map_ >= NUM_HEIGHT_MAPS)
      {
        current_height_map_ = 0;
      }

      collider_->addComponent<MeshFilter>()->mesh = height_map_[current_height_map_].get();
      collider_hm_->set_height_map(height_map_[current_height_map_]);

      if (triangle_drop_index_ > height_map_[current_height_map_]->face_count())
      {
        triangle_drop_index_ = 0;
      }
    }

    if (ImGui::IsKeyPressed('U'))
    {
      if (++triangle_drop_index_ > height_map_[current_height_map_]->face_count())
      {
        triangle_drop_index_ = 0;
      }
      vertex_drop_index_ = 3;
    }

    if (ImGui::IsKeyPressed('I'))
    {
      if (--triangle_drop_index_ < 0)
      {
        triangle_drop_index_ = height_map_[current_height_map_]->face_count() - 1;
      }
      vertex_drop_index_ = 3;
    }

    if (ImGui::IsKeyPressed('D') || ImGui::IsKeyPressed('U') || ImGui::IsKeyPressed('I'))
    {
      vec3 pos;
      if (++vertex_drop_index_ > 3)
      {
        vertex_drop_index_ = 0;
      }
      if (vertex_drop_index_ == 3)
      {
        pos = (height_map_[current_height_map_]->vertices()[triangle_drop_index_ * 3 + 0] + height_map_[current_height_map_]->vertices()[triangle_drop_index_ * 3 + 1] + height_map_[current_height_map_]->vertices()[triangle_drop_index_ * 3 + 2]) / 3.0f;
      }
      else
      {
        pos = height_map_[current_height_map_]->vertices()[triangle_drop_index_ * 3 + vertex_drop_index_];
      }

      pos.y = 15.0f;
      if (spheres_.size() > 0)
      {
        RespawnObject(spheres_[spheres_.size() - 1], pos);
      }
      else
      {
        SpawnNewSphere(pos, 1.0f, Color::Random());
      }
    }
  }


} /* end of vxr namespace */