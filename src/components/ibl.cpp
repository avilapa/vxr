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

#include "../../include/components/ibl.h"

#include "../../include/engine/engine.h"
#include "../../include/core/gameobject.h"
#include "../../include/core/scene.h"

namespace vxr 
{

  IBL::IBL()
  {
    set_name("Image Based Light");
    contributes_ = false;
  }

  IBL::~IBL()
  {
  }

  void IBL::onGUI()
  {
    ImGui::Spacing();
    ImGui::Text("Is contributing to current scene:"); ImGui::SameLine();
    if (contributes_)
    {
      ImGui::PushStyleColor(0, ImVec4(0.12f, 0.9f, 0.3f, 1.0f));
    }
    else
    {
      ImGui::PushStyleColor(0, ImVec4(0.8f, 0.3f, 0.12f, 1.0f));
    }
    ImGui::Text(((contributes_) ? "YES" : "NO"));
    ImGui::PopStyleColor();
    ImGui::Spacing();
    
  }

  System::IBL::IBL()
  {
  }

  System::IBL::~IBL()
  {

  }

  void System::IBL::init()
  {
    /*light_uniforms_.buffer = Engine::ref().gpu()->createBuffer({ BufferType::Uniform,  
      sizeof(light_uniforms_.data), 
      Usage::Static, 
      "Lights" });*/
  }

  void System::IBL::update()
  {
    if (scene_ != Engine::ref().scene())
    {
      scene_ = Engine::ref().scene();
      // Scene changed
    }
  }

  void System::IBL::renderUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "IBL Render Update");
    if (!scene_)
    {
      return;
    }

    for (auto &c : components_)
    {
      if (scene_->id() != c->gameObject()->scene_id() || !c->gameObject()->active())
      {
        c->contributes_ = false;
        continue;
      }

    }

    DisplayList frame;
    /*frame.fillBufferCommand()
      .set_buffer(light_uniforms_.buffer)
      .set_data(&light_uniforms_.data)
      .set_size(sizeof(light_uniforms_.data));*/
    Engine::ref().submitDisplayList(std::move(frame));
  }

  void System::IBL::renderPostUpdate()
  {
    VXR_TRACE_SCOPE("VXR", "IBL Render Post Update");
    if (!scene_)
    {
      return;
    }
  }

  ref_ptr<System::IBL> System::Getter<IBL>::get()
  {
    return Engine::ref().ibl();
  }
}