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
#include "../../include/engine/core_minimal.h"

/**
* \file dev.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Development example.
*
*/
namespace vxr 
{

  class Main : public Application
  {
    VXR_OBJECT(Main, Application);
  public:
    Main();

    virtual void init() override;
    virtual void start() override;
    virtual void update() override;
    virtual void renderUpdate() override;
    virtual void stop() override;

  private:
    static const uint32 NUM_LIGHTS = 10;
    ref_ptr<GameObject> cam_;
    ref_ptr<GameObject> light_node_;
    ref_ptr<GameObject> objects_floor_;
    ref_ptr<GameObject> light_[NUM_LIGHTS];
    ref_ptr<GameObject> skybox_;
  };

} /* end of vxr namespace */