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

#include "../../include/graphics/dl_command.h"

#include "../../include/graphics/render_context.h"

#if defined (VXR_OPENGL)
#  include "../graphics/backend/opengl/gl_backend.h"
#elif defined (VXR_DX11)
#  include "../graphics/backend/dx11/dx11_backend.h"
#else
#  error Backend must be defined on GENie.lua (e.g. try parameters --gl OR --dx11).
#endif

namespace vxr
{
  DisplayList::Command::Command()
  {
  }

  DisplayList::Command::~Command()
  {
  }

  void SetupViewCommand::execute()
  {
    VXR_TRACE_SCOPE("VXR", "Setup View");
    gpu::SetupView(data_);
  }

  void ClearCommand::execute()
  {
    VXR_TRACE_SCOPE("VXR", "Clear");
    gpu::ClearScreen(data_);
  }

  void FillBufferCommand::execute()
  {
    VXR_TRACE_SCOPE("VXR", "Fill Buffer");
    gpu::FillBuffer(data_);
  }

  void FillTextureCommand::execute()
  {
    VXR_TRACE_SCOPE("VXR", "Fill Texture");
    gpu::FillTexture(data_);
  }

  void SetupMaterialCommand::execute()
  {
    VXR_TRACE_SCOPE("VXR", "Setup Material");
    gpu::SetupMaterial(data_);
  }

  void RenderCommand::execute()
  {
    VXR_TRACE_SCOPE("VXR", "Render");
    gpu::Render(data_);
  }
}