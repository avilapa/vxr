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

#include "../core/object.h"
#include "../graphics/gpu_resources.h"

#include "../engine/ignore_warnings.h" // Ignore "same type qualifier used more than once" warning (C4114)

/**
* \file display_list.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Display List structure storing render commands.
*
*/
namespace vxr
{

  class DisplayList : public Object
  {

    VXR_OBJECT(DisplayList, Object)

    friend class GPU;
  public:
    DisplayList();
    ~DisplayList();

    struct Command;

    void update();

// ----------------------------------------------------------------------------------------
//  The following structures have been partially extracted from px_render.h 
//  by Jose L. Hidalgo (PpluX), and later modified to fit vxr needs.
//  Link: https://github.com/pplux/px/blob/master/px_render.h

#define PROPERTY(type, name, ...) \
      type name = __VA_ARGS__;\
      Self& set_##name(const type &c) { name = c; return *this; }

#define PROPERTY_PTR(type, name) \
      const type *name = nullptr;\
      Self& set_##name(const type *c) { name = c; return *this; }

#define PROP_ARRAY(type, count, name) \
      type name[count] = {};\
      Self& set_##name(size_t i, const type &c) { name[i] = c; return *this; }\
      Self& set_v_##name(std::vector<type> c) { for (uint32 i = 0; i < c.size(); ++i) { set_##name(i, c[i]); } return *this; }

    struct SetupViewData 
    {
      typedef SetupViewData Self;
      struct Viewport 
      {
        uint16 x, y, width, height;
      };
      PROPERTY(Viewport, viewport, {});
      PROPERTY(mat4, view_matrix, mat4());
      PROPERTY(mat4, projection_matrix, mat4());
      PROPERTY(gpu::Framebuffer, framebuffer, {});
      PROPERTY(uvec2, resolution, {});
    };

    struct ClearData 
    {
      typedef ClearData Self;
      PROPERTY(Color, color, { 0.0f, 0.0f, 0.0f, 1.0f });
      PROPERTY(float, depth, 1.0f);
      PROPERTY(int32, stencil, 0);
      PROPERTY(bool, clear_color, true);
      PROPERTY(bool, clear_depth, true);
      PROPERTY(bool, clear_stencil, false);
    };
    
    struct FillBufferData 
    {
      typedef FillBufferData Self;
      PROPERTY(const char*, name, "");
      PROPERTY(gpu::Buffer, buffer, {});
      PROPERTY(uint32, offset, 0);
      PROPERTY(uint32, size, 0);
      PROPERTY_PTR(void, data);
    };

    struct FillTextureData 
    {
      typedef FillTextureData Self;
      PROPERTY(gpu::Texture, texture, {});
      PROPERTY(uint16, offset_x, 0);
      PROPERTY(uint16, offset_y, 0);
      PROPERTY(uint16, offset_z, 0);
      PROPERTY(uint16, width, 0);
      PROPERTY(uint16, height, 0);
      PROPERTY(uint16, depth, 0);
      PROPERTY(bool, build_mipmap, false);
      PROPERTY_PTR(void, data);
      PROPERTY_PTR(void, data_1);
      PROPERTY_PTR(void, data_2);
      PROPERTY_PTR(void, data_3);
      PROPERTY_PTR(void, data_4);
      PROPERTY_PTR(void, data_5);
    };

    struct SetupMaterialData 
    {
      typedef SetupMaterialData Self;
      PROPERTY(gpu::Material, material, {});
      PROP_ARRAY(gpu::Texture, kMaxTextureUnits, texture);
      PROP_ARRAY(gpu::Buffer, kMaxVertexAttribs, buffer);
      PROP_ARRAY(gpu::Buffer, kMaxUniformBuffers + 1, uniform_buffer);
      PROPERTY(vec4, scissor, {});
      PROPERTY(mat4, model_matrix, mat4());
    };

    struct RenderData 
    {
      typedef RenderData Self;
      PROPERTY(gpu::Buffer, index_buffer, {});
      PROPERTY(uint32, offset, 0);
      PROPERTY(uint32, count, 0);
      PROPERTY(uint32, instances, 1);
      PROPERTY(IndexFormat::Enum, type, IndexFormat::UInt16);
    };

#undef PROPERTY
#undef PROPERTY_PTR
#undef PROP_ARRAY

    SetupViewData&      setupViewCommand();
    ClearData&          clearCommand();
    FillBufferData&     fillBufferCommand();
    FillTextureData&    fillTextureCommand();
    SetupMaterialData&  setupMaterialCommand();
    RenderData&         renderCommand();

// ----------------------------------------------------------------------------------------

  private:
    std::vector<scoped_ptr<Command>> commands_;

  };
} /* end of vxr namespace */

#include "../engine/ignore_warnings.h"