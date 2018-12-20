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

#include "../../include/graphics/display_list.h"
#include "../../include/graphics/dl_command.h"
#include "../../include/graphics/render_context.h"
#include "../../include/engine/engine.h"

#include <chrono>
#include <thread>

namespace vxr
{
  DisplayList::DisplayList()
  {
    set_name("Display List");
  }

  DisplayList::~DisplayList()
  {

  }

  void DisplayList::update()
  {
    VXR_TRACE_SCOPE("VXR", "Display List Update");
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Executing Display List (Commands %u)\n", commands_.size());
    for (uint32 i = 0; i < commands_.size(); i++) {
      commands_[i].get()->execute();
    }
    commands_.clear();
    ///std::this_thread::sleep_for(std::chrono::milliseconds(16)); /// Perform a wait for test purposes.
    VXR_LOG(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: Display List Execution Succsessful.\n");
  }

  DisplayList::SetupViewData& DisplayList::setupViewCommand()
  {
    scoped_ptr<Command> c;
    SetupViewCommand* cmd = c.allocT<SetupViewCommand>();
    commands_.push_back(std::move(c));
    return cmd->data_;
  }

  DisplayList::ClearData& DisplayList::clearCommand()
  {
    scoped_ptr<Command> c;
    ClearCommand* cmd = c.allocT<ClearCommand>();
    commands_.push_back(std::move(c));
    return cmd->data_;
  }

  DisplayList::FillBufferData& DisplayList::fillBufferCommand()
  {
    scoped_ptr<Command> c;
    FillBufferCommand* cmd = c.allocT<FillBufferCommand>();
    commands_.push_back(std::move(c));
    return cmd->data_;
  }

  DisplayList::FillTextureData& DisplayList::fillTextureCommand()
  {
    scoped_ptr<Command> c;
    FillTextureCommand* cmd = c.allocT<FillTextureCommand>();
    commands_.push_back(std::move(c));
    return cmd->data_;
  }

  DisplayList::SetupMaterialData& DisplayList::setupMaterialCommand()
  {
    scoped_ptr<Command> c;
    SetupMaterialCommand* cmd = c.allocT<SetupMaterialCommand>();
    commands_.push_back(std::move(c));
    return cmd->data_;
  }

  DisplayList::RenderData& DisplayList::renderCommand()
  {
    scoped_ptr<Command> c;
    RenderCommand* cmd = c.allocT<RenderCommand>();
    commands_.push_back(std::move(c));
    return cmd->data_;
  }

}