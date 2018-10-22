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

#include "../../include/core/gameobject.h"

#include "../../include/graphics/ui.h"

namespace vxr
{

  GameObject::GameObject()
  {
    set_name("GameObject");
    transform_ = System::Getter<Transform>::get()->createInstance<Transform>().get();
    transform_->transform_ = transform_;
    transform_->obj_ = this;
    components_.push_back(transform_.get());
  }

  GameObject::~GameObject()
  {

  }

  void GameObject::onGUI() 
  {
    if (ImGui::Checkbox(uiText("##Active").c_str(), &active_))
    {
      set_active(active_);
    }
    ImGui::SameLine(); Object::onGUI();
    ImGui::Spacing();
    ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)id()));

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_CollapsingHeader
      | ImGuiTreeNodeFlags_OpenOnArrow
      | ImGuiTreeNodeFlags_OpenOnDoubleClick
      | ImGuiTreeNodeFlags_DefaultOpen;

    for (auto &c : components_)
    {
      if (c != nullptr)
      {
        if (ImGui::CollapsingHeader(uiText(c->name()).c_str(), node_flags))
        {
          ImGui::Spacing();
          c->onGUI();
        }
        ImGui::Spacing();
      }
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    if (ImGui::Button(uiText("Add Component").c_str()))
    {

    }
    ImGui::EndChild();
  }

  ref_ptr<Transform> GameObject::transform()
  {
    return transform_.get();
  }

  void GameObject::set_active(bool enabled)
  {
    active_ = enabled;
    for (uint32 i = 0; i < transform_->num_children(); i++)
    {
      transform_->child(i)->gameObject()->set_active(enabled);
    }
  }

  bool GameObject::active()
  {
    return active_;
  }

  uint32 GameObject::scene_id()
  {
    return scene_id_;
  }

  std::vector<ref_ptr<Component>> GameObject::getComponents()
  {
    return components_;
  }

}