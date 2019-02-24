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

#include "shader.h"
#include "../../core/object.h"

#include <initializer_list>
#include <functional>

/**
* \file render_pass_instance.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Render Pass Instance base class.
*
*/
namespace vxr
{
  class Texture;

  namespace mat
  {

    class RenderPass;
    class RenderPassInstance : public Object
    {
      VXR_OBJECT(RenderPassInstance, Object);
    public:
      RenderPassInstance();
      virtual ~RenderPassInstance();

      virtual void onGUI() override;

      /// TODO: Depth buffer format and size are fixed at the moment. Size should be output texture size.
      void init(string shared_render_pass_name, uint32 index = 0);
      void init(std::initializer_list<string> shared_render_pass_name);

      void set_active_render_pass(uint32 index);
      uint32 active_render_pass() const;

      ref_ptr<RenderPass> sharedRenderPass() const;

      void set_input_texture(uint32 index, ref_ptr<Texture> texture);
      ref_ptr<Texture> input_texture(uint32 index = 0) const;
      std::vector<ref_ptr<Texture>> input_textures() const;

      void set_output_texture(uint32 index, ref_ptr<Texture> texture);
      ref_ptr<Texture> output_texture(uint32 index = 0) const;
      std::vector<ref_ptr<Texture>> output_textures() const;
      
      ref_ptr<Texture> depth_texture() const;

      Shader::UniformData uniforms_;

    private:
      uint32 active_render_pass_;
      std::vector<ref_ptr<RenderPass>> shared_render_passes_;
      std::vector<std::vector<ref_ptr<Texture>>> input_textures_;
      std::vector<std::vector<ref_ptr<Texture>>> output_textures_;
      ref_ptr<Texture> depth_texture_;
    };

  } /* end of mat namespace */

} /* end of vxr namespace */