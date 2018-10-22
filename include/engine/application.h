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

#include "../engine/engine.h"

/**
* \file application.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Application framework and main() function macro.
*
*/
namespace vxr 
{
  class Application : public Object 
  {
    
    VXR_OBJECT(Application, Object);

  public:
    Application();
    virtual ~Application();

    virtual void init();
    virtual void start();
    virtual void update();
    virtual void postUpdate();
    virtual void renderUpdate();
    virtual void renderPostUpdate();
    virtual void stop();

    virtual bool is_exiting();

    void set_time_step(double time_step);
    void set_max_steps(uint32 steps);

    float deltaTime();
    uint32 fps();

    uint32 run(int argc, char** argv);

  private:
    bool exit_ = false;
    
    struct Framerate 
    {
      double time_step = 1.0 / 120.0;
      uint32 max_steps = 15;
      uint32 fps = 0;
    } framerate_;
  };

} /* end of vxr namespace */

#define VXR_DEFINE_MAIN(function) \
int main(int argc, char** argv) \
{ \
	return function(argc, argv); \
}

#define VXR_DEFINE_APP_MAIN(className) \
int runApp(int argc, char** argv) \
{ \
	className app; \
	return app.run(argc, argv); \
} \
VXR_DEFINE_MAIN(runApp);