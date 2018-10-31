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
* \file instancing.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief This example showcases how to render a bunch of instanced cubes on VXR using the lower level API.
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

  private:
    gpu::Buffer vertex_buffer_;
    gpu::Buffer index_buffer_;
    gpu::Buffer instance_positions_buffer_;
    gpu::Buffer instance_colors_buffer_;
    gpu::Buffer uniform_buffer_;
    gpu::Material material_;
    gpu::Texture texture_;

    unsigned char* tex_data_;

    static const uint32 kNUM_INSTANCES = 40000;
    vec3 instance_positions_[kNUM_INSTANCES];
    vec3 instance_colors_[kNUM_INSTANCES];

    struct UniformState_Static 
    {
      mat4 model;
      mat4 view;
      mat4 proj;
    };
    UniformState_Static u_state_static_;
  };

} /* end of vxr namespace */