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
#include "../graphics/materials/material_instance.h"

/**
* \file renderer.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Renderer Component and Renderer System classes. Renderer Component serves as a reference container to a material for a GameObject.
*
*/
namespace vxr 
{

	class Renderer : public Component
  {
    VXR_OBJECT(Renderer, Component);
	public:
		Renderer();
		~Renderer();

    virtual void onGUI() override;

    ref_ptr<mat::MaterialInstance> material;
	};

  class DisplayList;

  namespace System 
  {
    class Renderer : public ComponentSystem
    {
      VXR_COMPONENT_SYSTEM(Renderer, ComponentSystem);
    public:
      Renderer();
      ~Renderer();

      void renderUpdate() override;
      void renderPostUpdate() override;

    private:
      bool setup(ref_ptr<vxr::Renderer> c);
      void render(ref_ptr<vxr::Renderer> c, DisplayList* frame);

      bool setupSkybox();
      void renderSkybox();

    private:
      std::vector<ref_ptr<vxr::Renderer>> transparent_;
    };

    template<> class Getter<vxr::Renderer>
    {
    public:
      static ref_ptr<System::Renderer> get();
    };

  }

} /* end of vxr namespace */