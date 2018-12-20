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

#include "../../include/graphics/gpu_instances.h"
#include "../engine/engine.h"

// ----------------------------------------------------------------------------------------
//  The following structures and functions have been partially extracted from px_render.h 
//  by Jose L. Hidalgo (PpluX), and later modified to fit vxr needs.
//  Link: https://github.com/pplux/px/blob/master/px_render.h
// ----------------------------------------------------------------------------------------

/**
* \file render_context.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Render Context class, in charge of managing the resource instances and the backend.
*
*/
namespace vxr
{
  class Texture;

  namespace gpu 
  {
    struct BackEnd;  
    void Render(DisplayList::RenderData& d);
    void SetupView(DisplayList::SetupViewData& d);
    void FillBuffer(DisplayList::FillBufferData& d);
    void FillTexture(DisplayList::FillTextureData& d);
    void SetupMaterial(DisplayList::SetupMaterialData& d);
  }

  class RenderContext 
  {
    friend class GPU;
    friend void gpu::Render(DisplayList::RenderData& d);
    friend void gpu::SetupView(DisplayList::SetupViewData& d);
    friend void gpu::FillBuffer(DisplayList::FillBufferData& d);
    friend void gpu::FillTexture(DisplayList::FillTextureData& d);
    friend void gpu::SetupMaterial(DisplayList::SetupMaterialData& d);
    friend class vxr::Texture;
  public:
    RenderContext();
    ~RenderContext();

    void init(const Params::GPU& params);

    // - Instances ----------------------------------------
    scoped_array<gpu::BufferInstance>      buffers_;
    scoped_array<gpu::TextureInstance>     textures_;
    scoped_array<gpu::MaterialInstance>    materials_;
    scoped_array<gpu::FramebufferInstance> framebuffers_;
    // - Render State -------------------------------------
    DisplayList::SetupMaterialData main_material = {};
    // - Back End -----------------------------------------
    gpu::BackEnd* back_end_ = nullptr;

  public:
    template<class T>
    static bool CheckValidResource(uint32 id, const scoped_array<T> *pool) {
      auto pv = indexAndVersion(id);
      uint32 pos = pv.first;
      uint32 version = pv.second;
      const T* result = &(*pool)[pos];
      uint32 real_version = (result->state.load() & 0xFFF);

      return real_version == version;
    }

    static uint32 index(uint32 id);
    static std::pair<uint32, uint32> indexAndVersion(uint32 id);

  private:
    template<class T, class B>
    static std::pair<T*, B*> GetResource(uint32 id, scoped_array<T>* instance_array, scoped_array<B>* backend_array) {
      if (CheckValidResource(id, instance_array))
      {
        uint32 i = index(id);
        return{ &(*instance_array)[i], &(*backend_array)[i] };
      }
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Invalid resource.\n");
      return{ nullptr, nullptr };
    }
  };

} /* end of vxr namespace */