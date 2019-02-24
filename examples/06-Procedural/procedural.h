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
#include "planet.h"

/**
* \file mesh.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief This example showcases use the higher level API to create a procedural planet editor.
*
* This example takes on creating a custom component, using dynamic buffers for the procedural planet,
* dynamic textures, using a camera transformation, creating custom editor features, and much more.
*
* This is an implementation of Sebastian Lague's procedural planet tutorial on Unity.
* Link: https://www.youtube.com/watch?v=QN39W020LqU
*
*/
namespace vxr 
{

  class Main : public Application
  {

    VXR_OBJECT(Main, Application);

  public:
    Main();

    virtual void start() override;
    virtual void update() override;

  private:
    ref_ptr<GameObject> cam_;
    ref_ptr<GameObject> planet_;
    ref_ptr<GameObject> light_;

    vec3 zoom_on = vec3(0.0f, 1.0f, 1.5f);
    vec3 zoom_off = vec3(0.0f, 0.0f, 2.5f);
  };

} /* end of vxr namespace */