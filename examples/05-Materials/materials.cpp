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

#include "materials.h"

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
    // 2. Create two scenes and load the first one.
    scene_material_editor_.alloc()->set_name("Material Editor");
    createSceneMaterialEditor();

    scene_white_furnace_.alloc()->set_name("White Furnace Test");
    createSceneWhiteFurnaceTest();

    Engine::ref().loadScene(scene_material_editor_);
    
    // 3. Submit the custom UI function.
    Engine::ref().submitUIFunction([this]() { customUI(); });

    Application::start();
  }

  void Main::createSceneMaterialEditor()
  {
    // 2.1. Load a texture for the skybox of the first scene.
    ref_ptr<Skybox> skybox;
    skybox.alloc()->set_name("Material Editor Skybox");
    ref_ptr<Texture> environment = Engine::ref().assetManager()->loadTexture("../../assets/environments/barcelona_rooftop.hdr", true);
    skybox->set_texture(environment);
    scene_material_editor_->set_skybox(skybox);

    // 2.2. Create a camera and setup for skybox rendering.
    ref_ptr<GameObject> cam;
    cam.alloc()->set_name("Camera");
    cam->addComponent<Camera>()->transform()->set_local_position(vec3(0, 0, 1.0f));
    cam->getComponent<Camera>()->set_clear_flags(Camera::ClearFlags::Skybox);
    cam->getComponent<Camera>()->set_fov(70.0f);
    scene_material_editor_->addObject(cam);

    // 2.3. Setup initial material values for the models and assign the textures if any.
    ref_ptr<mat::Std::Instance> mat, mat_textured;
    mat.alloc();
    mat->set_metallic(1.0f);
    mat->set_roughness(0.4f);
    Color random = Color::Random(); random.a = 1.0;
    mat->set_albedo(random);

    mat_textured.alloc();
    mat_textured->set_metallic(1.0f);
    mat_textured->set_roughness(1.0f);
    mat_textured->set_albedo(Color::White);

    mat_textured->set_albedo(Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/albedo.png"));
    mat_textured->set_normal(Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/normal.png"));
    mat_textured->set_metallic(Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/metallic.png"));
    mat_textured->set_roughness(Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/roughness.png"));
    mat_textured->set_ambient_occlusion(Engine::ref().assetManager()->loadTexture("../../assets/models/substance_sphere/ao.png"));
    mat_textured->set_iridescence_thickness(Engine::ref().assetManager()->loadTexture("../../assets/textures/noise/noise_swirl.jpg"));

    // 2.4. Create the actual game objects and assign mesh and materials.
    preview_model_[0].alloc()->set_name("Preview Model 1");
    preview_model_[0]->addComponent<MeshFilter>()->mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/sphere/sphere.obj");
    preview_model_[0]->addComponent<Renderer>()->material = mat.get();

    preview_model_[0]->transform()->set_local_position(vec3(0.3f, 0, 0));
    preview_model_[0]->transform()->set_local_rotation(vec3(0, 30.f, 0));
    preview_model_[0]->transform()->set_local_scale(vec3(1.0));
    scene_material_editor_->addObject(preview_model_[0]);

    preview_model_[1].alloc()->set_name("Preview Model 2");
    preview_model_[1]->set_active(false);
    preview_model_[1]->addComponent<MeshFilter>()->mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/substance_sphere/substance_sphere.obj");
    preview_model_[1]->addComponent<Renderer>()->material = mat_textured.get();

    preview_model_[1]->transform()->set_local_position(vec3(0.3f, 0, 0));
    preview_model_[1]->transform()->set_local_rotation(vec3(0, 30.f, 0));
    preview_model_[1]->transform()->set_local_scale(vec3(0.5));
    scene_material_editor_->addObject(preview_model_[1]);

    // 2.5. Add a white directional light to the scene.
    ref_ptr<GameObject> dir_light;
    dir_light.alloc()->set_name("Directional Light");
    dir_light->addComponent<Light>()->set_type(Light::Type::Directional);
    dir_light->transform()->set_local_rotation(vec3(90.0f, 60.0f, 90.0f));
    scene_material_editor_->addObject(dir_light);
  }

  void Main::createSceneWhiteFurnaceTest()
  {
    static const unsigned char albedo_data[] =
    {
      255, 255, 255,   255, 255, 255,
      235, 151, 136,   235, 151, 136,
    };

    static const unsigned char furnace_data[] =
    {
      255, 255, 255,
    };

    // 2.1. Create a white texture for the furnace test skybox
    ref_ptr<Skybox> skybox;
    skybox.alloc()->set_name("White Furnace Test Skybox");
    ref_ptr<Texture> furnace;
    furnace.alloc()->set_size(1, 1);
    furnace->set_texels_format(TexelsFormat::RGB_U8);
    furnace->set_type(TextureType::T2D);
    furnace->set_data((void*)furnace_data);
    furnace->set_hdr(true);
    skybox->set_texture(furnace);
    scene_white_furnace_->set_skybox(skybox);

    // 2.2. Create a camera and setup for skybox rendering.
    ref_ptr<GameObject> cam;
    cam.alloc()->set_name("Camera");
    cam->addComponent<Camera>()->transform()->set_local_position(vec3(0, 0, 15));
    cam->getComponent<Camera>()->set_clear_flags(Camera::ClearFlags::Skybox);
    cam->getComponent<Camera>()->set_fov(25.0f);
    scene_white_furnace_->addObject(cam);
  
    // 2.3. Load a sphere mesh for the game objects.
    ref_ptr<Mesh> mesh = Engine::ref().assetManager()->loadMesh("../../assets/models/sphere/sphere.obj");
    mesh->set_name("Sphere");

    // 2.4. Create a two color texture for the spheres.
    ref_ptr<Texture> albedo;
    albedo.alloc()->set_size(2, 2);
    albedo->set_texels_format(TexelsFormat::RGB_U8);
    albedo->set_type(TextureType::T2D);
    albedo->set_filtering(SamplerFiltering::Nearest, SamplerFiltering::Nearest);
    albedo->set_data((void*)albedo_data);

    for (uint32 i = 0; i < 2; ++i)
    {
      for (uint32 j = 0; j < 10; ++j)
      {
        // 2.5. Prepare each sphere material instance.
        ref_ptr<mat::Std::Instance> mat;
        mat.alloc()->set_albedo(albedo);
        mat->set_metallic((float)i);
        mat->set_roughness((float)j / 9.0f);
        mat->set_clear_coat(0.0f);

        // 2.6. Create the actual game object for each sphere and assign mesh and material.
        ref_ptr<GameObject> obj;
        obj.alloc()->set_name("Sphere Object");
        obj->addComponent<MeshFilter>()->mesh = mesh.get();
        obj->addComponent<Renderer>()->material = mat.get();

        obj->transform()->set_local_position(vec3(((float)j - 4.5f), ((float)i - 0.5f), 0));
        obj->transform()->set_local_rotation(vec3(0, 0.5f, 0));
        obj->transform()->set_local_scale(vec3(0.9f));
        scene_white_furnace_->addObject(obj);
      }
    }
  }

  void Main::customUI()
  {
    static bool showing_scene_1 = true;
    ImGui::Begin("Scene Selector");
    ImGui::Text("Example 05 aims to showcase the material model of the engine.");
    if (ImGui::Button((showing_scene_1) ? "Change to White Furnace Test scene" : "Change to Material Editor Scene"))
    {
      showing_scene_1 = !showing_scene_1;
      Engine::ref().loadScene((showing_scene_1) ? scene_material_editor_ : scene_white_furnace_);
    }

    ImGui::End();

    if (showing_scene_1)
    {
      materialEditorUI();
    }
  }

  void Main::materialEditorUI()
  {
    ImGui::Begin("Material Editor");

    static uint32 preview = 0;

    if (ImGui::Button("Next Preview Object"))
    {
      preview_model_[preview]->set_active(false);

      if (preview < NUM_PREVIEWS - 1)
      {
        preview++;
      }
      else
      {
        preview = 0;
      }

      preview_model_[preview]->set_active(true);
    }

    ImGui::Spacing();

    preview_model_[preview]->getComponent<Renderer>()->material->onGUI();

    ImGui::End();
  }

} /* end of vxr namespace */