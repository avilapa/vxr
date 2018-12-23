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

#include "../../include/engine/application.h"

/**
* \file framebuffers.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief This example showcases how to write a simple contrast changer post process material on VXR using the lower level API.
*
*/
namespace vxr 
{

  class Main : public Application
  {

    VXR_OBJECT(Main, Application);

  public:
    virtual void start() override;
    virtual void update() override;
    virtual void renderUpdate() override;
    virtual void stop() override;

    static const uint32 kNUM_CUBES_ROW = 5;

  private:
    // - Framebuffer data
    gpu::Framebuffer framebuffer_;

    gpu::Buffer vertex_buffer_fb_;
    gpu::Buffer index_buffer_fb_;

    struct UniformState_Contrast { float contrast = 3; float brightness = .6f; } u_state_contrast_;
    gpu::Buffer contrast_uniform_buffer_;

    gpu::Material material_fb_;

    // - Cubes data
    gpu::Buffer vertex_buffer_;
    gpu::Buffer index_buffer_;

    struct UniformState_Static { mat4 view; mat4 proj; } u_state_static_;
    struct UniformState_Stream { mat4 model; } u_state_stream_[kNUM_CUBES_ROW * kNUM_CUBES_ROW];

    gpu::Buffer stream_uniform_buffer_;
    gpu::Buffer static_uniform_buffer_;

    gpu::Texture texture_;
    void* tex_data_;

    gpu::Material material_;
  };

} /* end of vxr namespace */