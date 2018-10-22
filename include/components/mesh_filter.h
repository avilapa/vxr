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
#include "../graphics/mesh.h"

/**
* \file mesh_filter.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Mesh Filter Component and Mesh Filter System classes. Mesh Filter serves as a reference container to a mesh for a GameObject.
*
*/
namespace vxr 
{

	class MeshFilter : public Component
  {

    VXR_OBJECT(MeshFilter, Component);

	public:
    MeshFilter();
		virtual ~MeshFilter();

    virtual void onGUI() override;

    ref_ptr<Mesh> mesh;
	};

  namespace System 
  {
    class MeshFilter : public ComponentSystem
    {

      VXR_OBJECT(System::MeshFilter, ComponentSystem);

    public:
      MeshFilter();
      virtual ~MeshFilter();

      virtual void init() override;

    private:
      std::vector<ref_ptr<vxr::MeshFilter>> components_;

    public:
      template<typename T> ref_ptr<T> createInstance()
      {
        ref_ptr<T> c;
        c.alloc();
        components_.push_back(c.get());
        return c.get();
      }
    };

    template<> class Getter<vxr::MeshFilter>
    {
    public:
      static ref_ptr<System::MeshFilter> get();
    };

  }

} /* end of vxr namespace */