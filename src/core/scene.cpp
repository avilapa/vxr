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

#include "../../include/core/scene.h"
#include "../../include/components/renderer.h"
#include "../../include/components/mesh_filter.h"
#include "../../include/components/ibl.h"

namespace vxr
{

  Scene::Scene()
  {
    set_name("Scene");
    root_.alloc()->set_name("Scene Root");
    root_->scene_id_ = id();
  }

  Scene::~Scene()
  {

  }

  void Scene::onGUI()
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_CollapsingHeader
      | ImGuiTreeNodeFlags_OpenOnArrow
      | ImGuiTreeNodeFlags_OpenOnDoubleClick
      | ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::CollapsingHeader(uiText("Hierarchy").c_str(), node_flags))
    {
      ImGui::Spacing();
      int32 clicked_node = -1;
      recursiveHierarchyTree(root_, &clicked_node);
      if (clicked_node != -1)
      {
        hierarchy_selected_node_ = clicked_node;
      }
    }
    ImGui::Spacing();

    if (ImGui::CollapsingHeader(uiText("Lighting").c_str(), node_flags))
    {
      ImGui::Spacing();
      ImGui::Text("Environment");
      ImGui::Spacing();
      ImGui::Text("Skybox Material:"); ImGui::SameLine();
      if (!skybox_)
      {
        ImGui::PushStyleColor(0, ImVec4(0.8f, 0.3f, 0.12f, 1.0f)); 
        ImGui::Text("None");
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered())
        {
          ImGui::BeginTooltip();
          ImGui::Text("No skybox has been created.\nRight click to create default.");
          if (ImGui::GetIO().MouseClicked[1])
          {
            ref_ptr<Skybox> skybox;
            skybox.alloc()->set_name("Default Skybox");
            set_skybox(skybox);
            Engine::ref().camera()->main()->set_clear_flags(Camera::ClearFlags::Skybox);
          }
          ImGui::EndTooltip();
        }
      }
      else
      {
        ImGui::Text("SBox");
        /// Cubemap texture list
        static int test = 0;
        ImGui::Combo(uiText("Map##CubemapTexture").c_str(), (int*)&test, "White Boy\0Sunset\0\0");
      }
    }
  }

  void Scene::addObject(ref_ptr<GameObject> obj)
  {
    obj->transform()->set_parent(root_->transform());
    if (!default_camera_)
    {
      default_camera_ = obj->getComponent<Camera>();
    }
  }

  void Scene::set_default_camera(ref_ptr<Camera> cam)
  {
    default_camera_ = cam;
  }

  ref_ptr<Camera> Scene::default_camera()
  {
    return default_camera_;
  }

  ref_ptr<GameObject> Scene::root()
  {
    return root_;
  }

  void Scene::set_skybox(ref_ptr<Skybox> skybox)
  {
    if (this == Engine::ref().scene().get())
    {
      Engine::ref().ibl()->set_main(skybox->getComponent<IBL>());
    }
    skybox_ = skybox;
  }

  ref_ptr<Skybox> Scene::skybox() const
  {
    return skybox_;
  }

  void Scene::recursiveHierarchyTree(ref_ptr<GameObject> child, int *clicked_node)
  {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow
      | ImGuiTreeNodeFlags_OpenOnDoubleClick
      | ((hierarchy_selected_node_ == child->id()) ? ImGuiTreeNodeFlags_Selected : 0)
      | ((child->transform()->num_children() == 0) ? ImGuiTreeNodeFlags_Leaf : 0)
      | ImGuiTreeNodeFlags_DefaultOpen;

    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)child->id(), node_flags, "%s", child->name().c_str());

    if (ImGui::IsItemClicked())
    {
      *clicked_node = child->id();
      hierarchy_selected_object_ = child.get();
    }

    if (node_open)
    {
      for (uint32 i = 0; i < child->transform()->num_children(); i++)
      {
        recursiveHierarchyTree(child->transform()->child(i)->gameObject(), clicked_node);
      }
      ImGui::TreePop();
    }
  }

  ref_ptr<GameObject> Scene::onGUI_SelectedObject() const
  {
    return hierarchy_selected_object_;
  }
}