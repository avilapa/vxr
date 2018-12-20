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

#include "../core/component.h"
#include "../graphics/materials/shader.h"

/**
* \file ibl.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Indirect Light Component (IBL).
*
*/
namespace vxr 
{
  namespace System { class IBL; }

	class IBL : public Component
  {
    VXR_OBJECT(IBL, Component);
    friend class System::IBL;
	public:
    IBL();
		~IBL();

    virtual void onGUI() override;

	private:
    bool contributes_;
	};

  class Scene;

  namespace System 
  {
    class IBL : public ComponentSystem
    {
      VXR_OBJECT(System::IBL, ComponentSystem);
      friend class Renderer;
    public:
      IBL();
      virtual ~IBL();

      void init() override;
      void update() override;
      void renderUpdate() override;
      void renderPostUpdate() override;

    private:
      std::vector<ref_ptr<vxr::IBL>> components_;
      ref_ptr<Scene> scene_;

      /*struct IBLUniforms
      {
        gpu::Buffer buffer;
        Shader::LightData data;
      } light_uniforms_;*/

    public:
      template<typename T> ref_ptr<T> createInstance()
      {
        ref_ptr<T> c;
        c.alloc();
        components_.push_back(c.get());
        return c.get();
      }
    };

    template<> class Getter<vxr::IBL>
    {
    public:
      static ref_ptr<System::IBL> get();
    };

  }

} /* end of vxr namespace */