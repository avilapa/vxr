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

#include "../../include/graphics/ui.h"

#if defined (VXR_OPENGL)
#  include "../graphics/opengl/gl_imgui.h"
#elif defined (VXR_DX11)
#  include "../graphics/dx11/dx11_imgui.h"
#else
#  error Backend must be defined on GENie.lua (e.g. try parameters --gl OR --dx11).
#endif

namespace vxr
{

  bool ui::Init(Window::Data* data)
  {
    VXR_TRACE_SCOPE("VXR", "UI Init");
    ui::impl::Init(data);
    return true;
  }

  void ui::Update(Window::Data* data)
  {
    VXR_TRACE_SCOPE("VXR", "UI Update");
    ui::impl::Update(data);
    ImGui::NewFrame();
  }

  void ui::Draw()
  {
    VXR_TRACE_SCOPE("VXR", "UI Draw");
    ImGui::Render();
    ui::impl::Draw(ImGui::GetDrawData());
  }

  void ui::Stop(Window::Data* data)
  {
    VXR_TRACE_SCOPE("VXR", "UI Stop");
    ui::impl::Stop(data);
    ImGui::DestroyContext();
  }

  void ui::Test()
  {
    {
      static bool show_demo_window = false;
      static bool show_another_window = false;
      static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
      if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

      // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
      {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
          counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
      }

      // 3. Show another simple window.
      if (show_another_window)
      {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
          show_another_window = false;
        ImGui::End();
      }
    }
  }

  static uint32 hierarchy_selected_node_ = 0;
  static ref_ptr<GameObject> hierarchy_selected_object_ = nullptr;
  static void RecursiveHierarchyTree(ref_ptr<GameObject> child, int *clicked_node) 
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
        RecursiveHierarchyTree(child->transform()->child(i)->gameObject(), clicked_node);
      }
      ImGui::TreePop();
    }
  }

  void ui::Editor()
  {
#ifndef VXR_UI_EDITOR
#define VXR_UI_EDITOR
#endif
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    if (!ImGui::Begin("VXR", 0, ImGuiWindowFlags_MenuBar
                              | ImGuiWindowFlags_NoTitleBar 
                              | ImGuiWindowFlags_NoResize 
                              | ImGuiWindowFlags_NoMove 
                              | ImGuiWindowFlags_NoScrollbar 
                              | ImGuiWindowFlags_NoScrollWithMouse 
                              | ImGuiWindowFlags_NoCollapse 
                              | ImGuiWindowFlags_AlwaysAutoResize))
    {
      ImGui::End();
      ImGui::PopStyleVar();
      return;
    }

    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Close")) {};
        ImGui::EndMenu();
      }
      ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 330);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

      ImGui::EndMenuBar();
    }

    ImGui::Spacing();

    ImGui::BeginGroup();

    ImGui::BeginChild("Hierarchy##Editor", ImVec2(io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.7f), true);
    ImGui::Text("Hierarchy");
    ImGui::Separator();
    ImGui::Spacing();
   
    int32 clicked_node = -1;
    RecursiveHierarchyTree(Engine::ref().scene()->root(), &clicked_node);
    if (clicked_node != -1)
    {
      hierarchy_selected_node_ = clicked_node;
    }

    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::BeginChild("Scene##Editor", ImVec2(io.DisplaySize.x * 0.6f, io.DisplaySize.y * 0.7f), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::Text("Scene");
    ImGui::Separator();
    ImGui::Spacing();
    if (Engine::ref().camera()->main() != nullptr)
    {
      ImVec2 scene_size = ImVec2(io.DisplaySize.x * 0.6f - 16.0f, io.DisplaySize.y * 0.7f - 41.0f);
      ImGui::Image((void*)(intptr_t)Engine::ref().camera()->screen_id(), scene_size, ImVec2(0,1), ImVec2(1,0));
      Engine::ref().camera()->set_render_to_screen(false);
      Engine::ref().camera()->set_render_size({ scene_size.x, scene_size.y });
      Engine::ref().camera()->main()->set_aspect(scene_size.x / (scene_size.y));
    }
    ImGui::EndChild();

    ImGui::BeginChild("Project##Editor", ImVec2(io.DisplaySize.x * 0.25f, 0), true);
    ImGui::Text("Project");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Console##Editor", ImVec2(io.DisplaySize.x * 0.5f, 0), true);
    ImGui::Text("Console");
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::EndChild();

    ImGui::EndGroup();
    ImGui::SameLine();

    ImGui::BeginChild("Inspector##Editor", ImVec2(0, 0), true);
    ImGui::Text("Inspector");
    ImGui::Separator();
    ImGui::Spacing();

    if (hierarchy_selected_object_ != nullptr)
    {
      hierarchy_selected_object_->onGUI();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::End();
  }

} /* end of vxr namespace */