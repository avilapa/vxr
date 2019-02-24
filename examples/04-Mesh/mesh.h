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
* \file mesh.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief This example showcases how to load and render a mesh using the higher level API.
*
* This example takes on creating a scene, adding a camera and throwing in a rotating game object
* loaded from a mesh. Also, it showcases the engine WIP editor.
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
    ref_ptr<GameObject> teapot_;
    ref_ptr<GameObject> suzanne_;
    ref_ptr<GameObject> sphere_;
    ref_ptr<GameObject> light_;
  };

} /* end of vxr namespace */