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

#include "../graphics/display_list.h"

/**
* \file dl_command.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Display List render command structures.
*
*/

namespace vxr
{

  struct DisplayList::Command : public Referenced
  {
    Command();
    virtual ~Command();
    
    virtual void execute() = 0;
  };

  struct SetupViewCommand : DisplayList::Command
  {
    friend class DisplayList;
    void execute() override;

    DisplayList::SetupViewData data_;
  };

  struct ClearCommand : DisplayList::Command
  {
    friend class DisplayList;
    void execute() override;

    DisplayList::ClearData data_;
  };

  struct FillBufferCommand : DisplayList::Command
  {
    friend class DisplayList;
    void execute() override;

    DisplayList::FillBufferData data_;
  };

  struct FillTextureCommand : DisplayList::Command
  {
    friend class DisplayList;
    void execute() override;

    DisplayList::FillTextureData data_;
  };

  struct SetupMaterialCommand : DisplayList::Command
  {
    friend class DisplayList;
    void execute() override;

    DisplayList::SetupMaterialData data_;
  };

  struct RenderCommand : DisplayList::Command
  {
    friend class DisplayList;
    void execute() override;

    DisplayList::RenderData data_;
  };

} /* end of vxr namespace */