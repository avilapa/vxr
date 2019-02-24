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

#include "../gameobjects/skybox.h"
#include "../components/camera.h"
#include "../graphics/materials/skybox.h"

/**
* \file scene.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Scene descriptor class.
*
*/
namespace vxr 
{

	class Scene : public Object 
  {
    VXR_OBJECT(Scene, Object);
	public:
		Scene();
		~Scene();

    void onGUI() override;
    ref_ptr<GameObject> onGUI_SelectedObject() const;

    void addObject(ref_ptr<GameObject> obj);
    ref_ptr<GameObject> root();

    void set_default_camera(ref_ptr<Camera> cam);
    ref_ptr<Camera> default_camera();
    ref_ptr<Camera> findCameraInScene();

    struct RenderSettings
    {
      ref_ptr<mat::Skybox::Instance> skybox;
    };

    void set_skybox(ref_ptr<Skybox> skybox);
    ref_ptr<Skybox> skybox() const;

  private:
    void recursiveHierarchyTree(ref_ptr<GameObject> child, int* clicked_node);

  private:
    ref_ptr<GameObject> root_;
    ref_ptr<Skybox> skybox_;
    ref_ptr<Camera> default_camera_ = nullptr;

    // UI
    uint32 hierarchy_selected_node_ = 0;
    ref_ptr<GameObject> hierarchy_selected_object_ = nullptr;
	};

} /* end of vxr namespace */