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

#include "../../../include/graphics/ui/editor.h"
#include "../../../include/engine/engine.h"
#include "../../../include/engine/gpu.h"
#include "../../../include/components/camera.h"
#include "../../../include/core/scene.h"
#include "../../../include/graphics/composer.h"

namespace vxr
{

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

  static ImVec2 screen_pos = ImVec2(0, 0);

  void ui::Editor()
  {
    //Log.AddLog("asdasdasdasd");
    static bool show_editor = true;
    static bool show_statistics = false;
    static bool show_texture_viewer = false;

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
                              | ImGuiWindowFlags_AlwaysAutoResize
                              | ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
      ImGui::End();
      ImGui::PopStyleVar();
      return;
    }

    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View"))
      {
        ImGui::MenuItem("Show Statistics", "", &show_statistics);
        ImGui::MenuItem("Show Editor", "", &show_editor);
        ImGui::MenuItem("Show Texture Viewer", "", &show_texture_viewer);

        if (ImGui::MenuItem("Exit", "Alt+F4")) 
        {
          Engine::ref().window()->forceExit();
        }
        ImGui::EndMenu();
      }
      ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 330);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

      ImGui::EndMenuBar();
    }

    if (!show_editor)
    {
      ImGui::Image((void*)(intptr_t)Engine::ref().camera()->screen_texture_id(), { io.DisplaySize.x, io.DisplaySize.y }, ImVec2(0, 1), ImVec2(1, 0));
      Engine::ref().camera()->main()->composer()->set_render_size({ io.DisplaySize.x, io.DisplaySize.y });
      Engine::ref().camera()->main()->set_aspect(io.DisplaySize.x / io.DisplaySize.y);
      Engine::ref().camera()->set_render_to_screen(true);

      ImGui::End();
      ImGui::PopStyleVar();

      if (show_statistics) Statistics(&show_statistics, !show_editor);
      if (show_texture_viewer) TextureViewer(&show_texture_viewer, !show_editor);
      return;
    }

    ImGui::Spacing();

    ImGui::BeginGroup();

    ImGui::BeginChild("Hierarchy##Editor", ImVec2(io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.7f), true);
    ImGui::Text("Hierarchy");
    ImGui::Separator();
    ImGui::Spacing();

    Engine::ref().scene()->onGUI();

    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::BeginChild("Scene##Editor", ImVec2(io.DisplaySize.x * 0.6f, io.DisplaySize.y * 0.7f), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::Text("Scene");
    ImGui::Separator();
    ImGui::Spacing();
    if (Engine::ref().camera()->main() != nullptr)
    {
      ImVec2 scene_size = ImVec2(io.DisplaySize.x * 0.6f - 16.0f, io.DisplaySize.y * 0.7f - 41.0f);
      ImGui::Image((void*)(intptr_t)Engine::ref().camera()->screen_texture_id(), scene_size, ImVec2(0,1), ImVec2(1,0));
      Engine::ref().camera()->main()->composer()->set_render_size({ scene_size.x, scene_size.y });
      Engine::ref().camera()->main()->set_aspect(scene_size.x / (scene_size.y));
      Engine::ref().camera()->set_render_to_screen(false);
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
    Engine::ref().Log.Draw();
    ImGui::EndChild();

    ImGui::EndGroup();
    ImGui::SameLine();

    ImGui::BeginChild("Inspector##Editor", ImVec2(0, 0), true);
    ImGui::Text("Inspector");
    ImGui::Separator();
    ImGui::Spacing();

    if (Engine::ref().scene()->onGUI_SelectedObject() != nullptr)
    {
      Engine::ref().scene()->onGUI_SelectedObject()->onGUI();
    }

    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::End();

    if (show_statistics) Statistics(&show_statistics, !show_editor);
    if (show_texture_viewer) TextureViewer(&show_texture_viewer, !show_editor);
  }

  void ui::Statistics(bool *open, bool fullscreen)
  { 
    if (fullscreen)
    {
      ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.008f, ImGui::GetIO().DisplaySize.y * 0.035f), ImGuiCond_Always);
    }
    else
    {
      ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.172f, ImGui::GetIO().DisplaySize.y * 0.095f), ImGuiCond_Always);
    }

    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
    if (ImGui::Begin("Statistics",  open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
      ImGui::Text("GPU data:");
      ref_ptr<GPU> gpu = Engine::ref().gpu();
      ImGui::Text("Buffers:         %d / %d", gpu->num_used_buffers(), gpu->num_buffers());
      ImGui::Text("Textures:        %d / %d", gpu->num_used_textures(), gpu->num_textures());
      ImGui::Text("Materials:       %d / %d", gpu->num_used_materials(), gpu->num_materials());
      ImGui::Text("Framebuffers:    %d / %d", gpu->num_used_framebuffers(), gpu->num_framebuffers());
    }
    ImGui::End();
  }
  
  void ui::TextureViewer(bool *open, bool fullscreen)
  { 
    if (fullscreen)
    {
      ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.008f, ImGui::GetIO().DisplaySize.y * 0.035f), ImGuiCond_Always);
    }
    else
    {
      ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.172f, ImGui::GetIO().DisplaySize.y * 0.095f), ImGuiCond_Always);
    }

    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
    if (ImGui::Begin("Texture Viewer",  open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing))
    {
      static int id = 0;
      ImGui::InputInt("Texture ID", &id);
      ImGui::Image((void*)(intptr_t)id, { 256, 256 }, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
  }

} /* end of vxr namespace */