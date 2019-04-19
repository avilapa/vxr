#pragma once

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
#include "../../include/engine/core_minimal.h"
#include "../../include/graphics/height_map.h"
#include "../../include/physics/collider_height_map.h"

/**
* \file physics.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief This example showcases the use of rigidbody physics and collision detection in the engine.
*        NOTE: This example should be compiled in a single thread. (--no-threading)
*/
namespace vxr 
{

  class Main : public Application
  {
    VXR_OBJECT(Main, Application);
  public:
    Main();

    virtual void start() override;
    virtual void update(float dt) override;

  private:
    void SpawnNewSphere(const vec3& position, const float& scale, const Color& color);
    void RespawnObject(ref_ptr<GameObject> obj, const vec3& position);

    void Input();

  private:
    static const uint32 NUM_HEIGHT_MAPS = 5;
    static const uint32 NUM_CAMERAS = 2;
    static const uint32 NUM_COLLIDER_MATERIALS = 2;
    uint32 current_height_map_ = 0;
    uint32 current_camera_ = 0;
    uint32 current_collider_material_ = 0;

    uint32 current_first_sphere_ = 0;

    std::vector<ref_ptr<GameObject>> spheres_;
    ref_ptr<GameObject> light_;
    ref_ptr<Scene> scene_;

    ref_ptr<GameObject> camera_[NUM_CAMERAS];

    ref_ptr<mesh::HeightMap> height_map_[NUM_HEIGHT_MAPS];
    ref_ptr<ColliderHeightMap> collider_hm_;
    ref_ptr<GameObject> collider_;
    ref_ptr<mat::MaterialInstance> collider_materials_[NUM_COLLIDER_MATERIALS];

    // Camera settings
    float cam_incl_ = -35.0f;
    float cam_height_ = 40.0f;
    float rotation_speed_ = 0.5f;

    uint32 triangle_drop_index_ = 0;
    uint32 vertex_drop_index_ = 0;
  };

} /* end of vxr namespace */