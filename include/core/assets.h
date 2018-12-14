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

#include "object.h"

/**
* \file assets.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief AssetManager class.
*
*/
namespace vxr 
{

  class GameObject;
  class Material;
  class Mesh;

  namespace Asset
  {

    ref_ptr<GameObject> loadModelOBJ(const char* file);

    ref_ptr<Mesh> loadMeshOBJ(const char* file, uint32 mesh = 0);
  }

	class AssetManager : public Object
  {
    VXR_OBJECT(AssetManager, Object);
	public:
    AssetManager();
		~AssetManager();

    void init();

    void addMaterial(ref_ptr<Material> material);
    template<class T> 
    void addMaterial()
    {
      ref_ptr<T> m;
      m.alloc();
      materials_.push_back(m.get());
    }

    ref_ptr<Material> getSharedMaterial(const char* shared_material_name) const;
    std::vector<ref_ptr<Material>> getSharedMaterials() const;

  private:
    void initializeMaterials();
    ///void initializeTextures();

  private:
    std::vector<ref_ptr<Material>> materials_;
	};

} /* end of vxr namespace */