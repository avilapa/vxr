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
  class Texture;
  class Mesh;
  namespace mat { class Material; class MaterialInstance; class RenderPass; class RenderPassInstance; }

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

    void addMaterial(ref_ptr<mat::Material> material);
    template<class T> 
    void addMaterial()
    {
      ref_ptr<T> m;
      m.alloc();
      materials_.push_back(m.get());
    }
    ref_ptr<mat::Material> shared_material(const char* shared_material_name) const;
    std::vector<ref_ptr<mat::Material>> shared_materials() const;

    void addRenderPass(ref_ptr<mat::RenderPass> render_pass);
    template<class T>
    void addRenderPass()
    {
      ref_ptr<T> rp;
      rp.alloc();
      render_passes_.push_back(rp.get());
    }
    ref_ptr<mat::RenderPass> shared_render_pass(const char* shared_render_pass_name) const;
    std::vector<ref_ptr<mat::RenderPass>> shared_render_passes() const;

    ref_ptr<Texture> default_texture() const;
    ref_ptr<Texture> default_cubemap() const;

  private:
    void initializeMaterials();
    void initializeRenderPasses();
    void initializeTextures();

  private:
    std::vector<ref_ptr<mat::Material>> materials_;
    std::vector<ref_ptr<mat::RenderPass>> render_passes_;

    ref_ptr<Texture> default_texture_;
    ref_ptr<Texture> default_cubemap_;
	};

} /* end of vxr namespace */