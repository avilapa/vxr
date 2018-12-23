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

#include "../../include/gameobjects/skybox.h"

#include "../../include/components/mesh_filter.h"
#include "../../include/components/renderer.h"
#include "../../include/components/ibl.h"

#include "../../include/graphics/materials/skybox.h"

namespace vxr
{

  Skybox::Skybox()
  {
    set_name("Skybox");

    material_.alloc()->set_name("Scene Skybox Material");
    addComponent<Renderer>()->material = material_.get();

    ref_ptr<mesh::Cube> skybox_mesh;
    skybox_mesh.alloc()->set_name("Scene Skybox Mesh");
    addComponent<MeshFilter>()->mesh = skybox_mesh.get();

    addComponent<IBL>();
  }

  Skybox::~Skybox()
  {

  }

  void Skybox::onGUI()
  {
    material_->onGUI();
  }

  ref_ptr<mat::Skybox::Instance> Skybox::material()
  {
    return material_;
  }

}