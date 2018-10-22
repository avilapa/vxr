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
#include "../../include/graphics/mesh.h"
#include "../../include/graphics/material.h"
#include "../../include/core/gameobject.h"

/**
* \file mesh.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Development example.
*
*/
namespace vxr 
{

  class BehaviourSon : public Custom
  {

    VXR_OBJECT(BehaviourSon, Component);

  public:
    BehaviourSon() {};
    virtual ~BehaviourSon() {};

    virtual void start() { printf("aaa: %0.3f", aaa); };
    virtual void update() { printf("bb: %0.3f", aaa); };

    float aaa = 1.0f;
  };

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

    void updateWindowTitle();

    static const uint32 kNUM_CUBES_ROW = 7;

  private:
    char* window_title_;
    
    ref_ptr<GameObject> cam_;

    
  };

} /* end of vxr namespace */