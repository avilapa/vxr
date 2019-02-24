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
  namespace mesh { class Cube; class Quad; }

  class Composer;

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

    // Materials
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

    // Render Passes
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

    // Textures
    ref_ptr<Texture> loadTexture(const char* file, bool flip = false);
    ref_ptr<Texture> loadTexture(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft, bool flip = false);
    ref_ptr<Texture> loadTexture(const char* cubemap_folder_path, const char* extension, bool flip = false);

    ref_ptr<Texture> default_texture_white() const;
    ref_ptr<Texture> default_texture_black() const;
    ref_ptr<Texture> default_texture_normal() const;
    ref_ptr<Texture> default_cubemap() const;

    // Meshes
    ref_ptr<Mesh> loadMesh(const char* file, uint32 mesh_index = 0);

    ref_ptr<Mesh> default_cube() const;
    ref_ptr<Mesh> default_quad() const;

    // Composer
    ref_ptr<Composer> default_camera_composer() const;

  private:
    void initializeMaterials();
    void initializeRenderPasses();
    void initializeTextures();
    void initializeMeshes();

  private:
    std::vector<ref_ptr<mat::Material>> materials_;
    std::vector<ref_ptr<mat::RenderPass>> render_passes_;
    std::vector<ref_ptr<Texture>> textures_;
    std::vector<ref_ptr<Mesh>> meshes_;

    ref_ptr<Composer> default_composer_;
	};

} /* end of vxr namespace */