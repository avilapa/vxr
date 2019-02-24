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

#include "../../include/core/assets.h"

#include "../../include/engine/engine.h"
#include "../../include/core/gameobject.h"
#include "../../include/components/renderer.h"
#include "../../include/components/mesh_filter.h"
#include "../../include/graphics/texture.h"
#include "../../include/graphics/materials/standard.h"
#include "../../include/graphics/materials/skybox.h"
#include "../../include/graphics/materials/unlit.h"
#include "../../include/graphics/materials/pass_standard.h"
#include "../../include/graphics/materials/pass_filters.h"
#include "../../include/graphics/materials/pass_ibl.h"
#include "../../include/graphics/composer.h"

#include "../../deps/mesh/tiny_obj_loader.h"


namespace vxr
{

  ref_ptr<GameObject> Asset::loadModelOBJ(const char* file)
  {
    VXR_TRACE_SCOPE("VXR", "Load Model OBJ");
    std::vector<tinyobj::shape_t> m_shapes;
    std::vector<tinyobj::material_t> m_materials;

    string name = file;
    string path = name.substr(0, name.find_last_of("/") + 1);
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: Loading mesh: %s.\n", name.c_str());
    string err = tinyobj::LoadObj(m_shapes, m_materials, name.c_str(), path.c_str());

    if (!err.empty())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not load mesh %s (%s).\n", name.c_str(), err.c_str());
      return nullptr;
    }

    ref_ptr<GameObject> obj;
    obj.alloc();

    for (uint32 i = 0; i < m_shapes.size(); i++)
    {
      ref_ptr<GameObject> child;
      
      if (m_shapes.size() > 1)
      {
        // Create empty parent container if there's multiple parts in the asset.
        child.alloc();
        child->transform()->set_parent(obj->transform());
      }
      else
      {
        child = obj;
      }

      ref_ptr<Mesh> mesh;
      ref_ptr<mat::Std::Instance> mat;

      mesh.alloc();
      mat.alloc();

      child->addComponent<MeshFilter>()->mesh = mesh;
      child->addComponent<Renderer>()->material = mat.get();

      std::vector<vec3> vertices;
      std::vector<vec3> normals;
      std::vector<vec2> uv;
      std::vector<uint32> indices;

      for (uint32 j = 0; j < m_shapes[i].mesh.indices.size(); ++j)
      {
        indices.push_back(m_shapes[i].mesh.indices[j]);
      }

      for (uint32 j = 0; j < m_shapes[i].mesh.positions.size(); j += 3)
      {
        vertices.push_back(vec3(m_shapes[i].mesh.positions[j], m_shapes[i].mesh.positions[j + 1], m_shapes[i].mesh.positions[j + 2]));
      }

      for (uint32 j = 0; j < m_shapes[i].mesh.normals.size(); j += 3)
      {
        normals.push_back(vec3(m_shapes[i].mesh.normals[j], m_shapes[i].mesh.normals[j + 1], m_shapes[i].mesh.normals[j + 2]));
      }

      for (uint32 j = 0; j < m_shapes[i].mesh.texcoords.size(); j += 2)
      {
        uv.push_back(vec2(m_shapes[i].mesh.texcoords[j], 1.0f - m_shapes[i].mesh.texcoords[j + 1]));
      }

      mesh->set_indices(indices);
      mesh->set_vertices(vertices);
      mesh->set_normals(normals);
      mesh->set_uv(uv);
    }

    m_shapes.clear();
    m_materials.clear();

    return obj;
  }

  ref_ptr<Mesh> Asset::loadMeshOBJ(const char* file, uint32 mesh)
  {
    std::vector<tinyobj::shape_t> m_shapes;
    std::vector<tinyobj::material_t> m_materials;

    string name = file;
    string path = name.substr(0, name.find_last_of("/") + 1);
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [MESH] Loading... (%s)\n", name.c_str());
    string err = tinyobj::LoadObj(m_shapes, m_materials, name.c_str(), path.c_str());

    if (!err.empty())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [MESH] Could not load mesh %s (%s).\n", name.c_str(), err.c_str());
      return nullptr;
    }

    ref_ptr<Mesh> m;
    m.alloc();

    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> uv;
    std::vector<uint32> indices;

    for (uint32 i = 0; i < m_shapes[mesh].mesh.indices.size(); ++i)
    {
      indices.push_back(m_shapes[mesh].mesh.indices[i]);
    }

    for (uint32 i = 0; i < m_shapes[mesh].mesh.positions.size(); i += 3)
    {
      vertices.push_back(vec3(m_shapes[mesh].mesh.positions[i], m_shapes[mesh].mesh.positions[i + 1], m_shapes[mesh].mesh.positions[i + 2]));
    }

    for (uint32 i = 0; i < m_shapes[mesh].mesh.normals.size(); i += 3)
    {
      normals.push_back(vec3(m_shapes[mesh].mesh.normals[i], m_shapes[mesh].mesh.normals[i + 1], m_shapes[mesh].mesh.normals[i + 2]));
    }

    for (uint32 i = 0; i < m_shapes[mesh].mesh.texcoords.size(); i += 2)
    {
      uv.push_back(vec2(m_shapes[mesh].mesh.texcoords[i], 1.0f - m_shapes[mesh].mesh.texcoords[i + 1]));
    }

    m->set_indices(indices);
    m->set_vertices(vertices);
    m->set_normals(normals);
    m->set_uv(uv);

    m_shapes.clear();
    m_materials.clear();
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [MESH] Loaded (%s).\n", name.c_str());
    return m;
  }

  AssetManager::AssetManager()
  {
  }

  AssetManager::~AssetManager()
  {
  }

  void AssetManager::init()
  {
    initializeTextures();
    initializeMaterials();
    initializeRenderPasses();
    initializeMeshes();

    default_composer_.alloc()->set_name("Default Camera Composer");
  }

  void AssetManager::initializeMaterials()
  {
    // Adding engine materials
    addMaterial<mat::ScreenMaterial>();
    addMaterial<mat::Std>();
    addMaterial<mat::Unlit>();
    addMaterial<mat::Wireframe>();
    addMaterial<mat::Skybox>();
  }

  void AssetManager::initializeRenderPasses()
  {
    addRenderPass<mat::Screen>();
    addRenderPass<mat::BuildCubemap>();
    addRenderPass<mat::ComputeIrradiance>();
    addRenderPass<mat::PrefilterCubemap>();
    addRenderPass<mat::BRDFIntegration>();
    addRenderPass<mat::Negative>();
    addRenderPass<mat::Grayscale>();
  }

  static const unsigned char t_white[]  = { 255, 255, 255 };
  static const unsigned char t_black[]  = { 0, 0, 0, };
  static const unsigned char t_normal[] = { 0, 0, 255, };

  void AssetManager::initializeTextures()
  {
    textures_.resize(4);

    textures_[0].alloc()->set_name("Default White Texture"); 
    textures_[0]->set_size(1, 1);
    textures_[0]->set_texels_format(TexelsFormat::RGB_U8);
    textures_[0]->set_type(TextureType::T2D);
    textures_[0]->set_data((void*)t_white);

    textures_[1].alloc()->set_name("Default Black Texture");
    textures_[1]->set_size(1, 1);
    textures_[1]->set_texels_format(TexelsFormat::RGB_U8);
    textures_[1]->set_type(TextureType::T2D);
    textures_[1]->set_data((void*)t_black);

    textures_[2].alloc()->set_name("Default Normal Texture");
    textures_[2]->set_size(1, 1);
    textures_[2]->set_texels_format(TexelsFormat::RGB_U8);
    textures_[2]->set_type(TextureType::T2D);
    textures_[2]->set_data((void*)t_normal);

    textures_[3].alloc()->set_name("Default Cubemap White Texture");
    textures_[3]->set_size(1, 1);
    textures_[3]->set_texels_format(TexelsFormat::RGB_U8);
    textures_[3]->set_type(TextureType::CubeMap);
    textures_[3]->set_data((void*)t_white, 0);
    textures_[3]->set_data((void*)t_white, 1);
    textures_[3]->set_data((void*)t_white, 2);
    textures_[3]->set_data((void*)t_white, 3);
    textures_[3]->set_data((void*)t_white, 4);
    textures_[3]->set_data((void*)t_white, 5);
  }

  void AssetManager::initializeMeshes()
  {
    meshes_.resize(2);

    meshes_[0].allocT<mesh::Cube>()->set_name("Default Cube");
    if (!meshes_[0]->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not set up default cubemap.\n");
      return;
    }

    meshes_[1].allocT<mesh::Quad>()->set_name("Default Quad");
    if (!meshes_[1]->setup())
    {
      VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not set up default quad.\n");
      return;
    }
  }

  void AssetManager::addMaterial(ref_ptr<mat::Material> material)
  {
    materials_.push_back(material);
  }

  ref_ptr<mat::Material> AssetManager::shared_material(const char* shared_material_name) const
  {
    for (uint32 i = 0; i < materials_.size(); ++i)
    {
      if (materials_[i]->name() == shared_material_name)
      {
        return materials_[i];
      }
    }
    VXR_LOG(VXR_DEBUG_LEVEL_WARNING, "[WARNING]: Unknown shared material '%s'.\n", shared_material_name);
    return nullptr;
  }

  std::vector<ref_ptr<mat::Material>> AssetManager::shared_materials() const
  {
    return materials_;
  }

  void AssetManager::addRenderPass(ref_ptr<mat::RenderPass> render_pass)
  {
    render_passes_.push_back(render_pass);
  }

  ref_ptr<mat::RenderPass> AssetManager::shared_render_pass(const char* shared_render_pass_name) const
  {
    for (uint32 i = 0; i < render_passes_.size(); ++i)
    {
      if (render_passes_[i]->name() == shared_render_pass_name)
      {
        return render_passes_[i];
      }
    }
    VXR_LOG(VXR_DEBUG_LEVEL_WARNING, "[WARNING]: Unknown shared render pass '%s'.\n", shared_render_pass_name);
    return nullptr;
  }

  std::vector<ref_ptr<mat::RenderPass>> AssetManager::shared_render_passes() const
  {
    return render_passes_;
  }

  ref_ptr<Texture> AssetManager::loadTexture(const char* file, bool flip)
  {
    // Check if texture exists.
    for (uint32 i = 0; i < textures_.size(); ++i)
    {
      if (textures_[i]->path() != "" && textures_[i]->path() == file)
      {
        return textures_[i];
      }
    }

    // Create new texture.
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [TEXTURE] Loading... (%s)\n", file);
    uint32 index = textures_.size();
    ref_ptr<Texture> t;
    t.alloc()->set_type(TextureType::T2D);
    t->path_ = file;
    t->set_hdr(t->path_.substr(t->path_.find_last_of(".") + 1) == "hdr");
    textures_.push_back(t);

#ifdef VXR_THREADING
    textures_[index]->loading_ = true;
    threading::Sync sync;
    threading::Task task = [this, index, file, flip]()
    {
#endif
      // Routine to execute.
      textures_[index]->set_data(gpu::Texture::loadFromFile(file, textures_[index]->gpu_.info, flip));
      textures_[index]->dirty_ = true;
#ifdef VXR_THREADING
      textures_[index]->loading_ = false;
    };
    Engine::ref().submitAsyncTask(task, &sync);
#endif
    return t;
  }

  ref_ptr<Texture> AssetManager::loadTexture(const char* cubemap_folder_path, const char* extension, bool flip)
  {
    // Check if texture exists.
    std::string folder = cubemap_folder_path;
    std::string paths[6] =
    {
      folder + "/rt." + extension,
      folder + "/lf." + extension,
      folder + "/up." + extension,
      folder + "/dn." + extension,
      folder + "/bk." + extension,
      folder + "/ft." + extension,
    };
    for (uint32 i = 0; i < textures_.size(); ++i)
    {
      if (textures_[i]->path() != "" && textures_[i]->path() == paths[0])
      {
        return textures_[i];
      }
    }

    // Create new texture.
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [TEXTURE] Loading... (%s)\n", cubemap_folder_path);
    uint32 index = textures_.size();
    ref_ptr<Texture> t;
    t.alloc()->set_type(TextureType::CubeMap);
    t->path_ = paths[0];
    t->set_hdr(strcmp(extension, "hdr"));
    textures_.push_back(t);


#ifdef VXR_THREADING
    textures_[index]->loading_ = true;
    threading::Sync sync;
    threading::Task task = [this, index, paths, flip]()
    {
#endif
      // Routine to execute.
      std::vector<void*> data = gpu::Texture::loadCubemapFromFile(paths[0].c_str(), paths[1].c_str(), paths[2].c_str(), paths[3].c_str(), paths[4].c_str(), paths[5].c_str(), textures_[index]->gpu_.info, flip);
      for (uint32 i = 0; i < 6; ++i)
      {
        textures_[index]->set_data(data[i], i);
      }
      textures_[index]->set_type(TextureType::CubeMap);
      textures_[index]->dirty_ = true;
#ifdef VXR_THREADING
      textures_[index]->loading_ = false;
    };
    Engine::ref().submitAsyncTask(task, &sync);
#endif
    return t;
  }

  ref_ptr<Texture> AssetManager::loadTexture(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft, bool flip)
  {
    // Check if texture exists.
    for (uint32 i = 0; i < textures_.size(); ++i)
    {
      if (textures_[i]->path() != "" && textures_[i]->path() == rt)
      {
        return textures_[i];
      }
    }

    // Create new texture.
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [TEXTURE] Loading... (%s)\n", rt);
    uint32 index = textures_.size();
    ref_ptr<Texture> t;
    t.alloc()->set_type(TextureType::CubeMap);
    t->path_ = rt;
    t->set_hdr(t->path_.substr(t->path_.find_last_of(".") + 1) == "hdr");
    textures_.push_back(t);

    textures_[index]->path_ = rt;
#ifdef VXR_THREADING
    textures_[index]->loading_ = true;
    threading::Sync sync;
    threading::Task task = [this, index, rt, lf, up, dn, bk, ft, flip]()
    {
#endif
      // Routine to execute.
      std::vector<void*> data = gpu::Texture::loadCubemapFromFile(rt, lf, up, dn, bk, ft, textures_[index]->gpu_.info, flip);
      for (uint32 i = 0; i < 6; ++i)
      {
        textures_[index]->set_data(data[i], i);
      }
      textures_[index]->set_type(TextureType::CubeMap);
      textures_[index]->dirty_ = true;
#ifdef VXR_THREADING
      textures_[index]->loading_ = false;
    };
    Engine::ref().submitAsyncTask(task, &sync);
#endif
    return t;
  }

  ref_ptr<Texture> AssetManager::default_texture_white() const
  {
    return textures_[0];
  }

  ref_ptr<Texture> AssetManager::default_texture_black() const
  {
    return textures_[1];
  }

  ref_ptr<Texture> AssetManager::default_texture_normal() const
  {
    return textures_[2];
  }

  ref_ptr<Texture> AssetManager::default_cubemap() const
  {
    return textures_[3];
  }

  ref_ptr<Mesh> AssetManager::loadMesh(const char* file, uint32 mesh)
  {
    // Check if mesh exists.
    for (uint32 i = 0; i < meshes_.size(); ++i)
    {
      if (meshes_[i]->path() != "" && meshes_[i]->path() == file)
      {
        return meshes_[i];
      }
    }

    // Create new mesh.
    VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [MESH] Loading... (%s)\n", file);
    uint32 index = meshes_.size();
    ref_ptr<Mesh> m;
    m.alloc();
    meshes_.push_back(m);

    meshes_[index]->path_ = file;
#ifdef VXR_THREADING
    meshes_[index]->loading_ = true;
    threading::Sync sync;
    threading::Task task = [this, index, file, mesh]()
    {
#endif
      // Routine to execute.
      /// TODO: Identify extension and execute functions accordingly.
      VXR_TRACE_BEGIN("VXR", "Mesh Loading");
      std::vector<tinyobj::shape_t> m_shapes;
      std::vector<tinyobj::material_t> m_materials;

      string name = file;
      string path = name.substr(0, name.find_last_of("/") + 1);
      string err = tinyobj::LoadObj(m_shapes, m_materials, name.c_str(), path.c_str());

      if (err.empty())
      {
        std::vector<vec3> vertices;
        std::vector<vec3> normals;
        std::vector<vec2> uv;
        std::vector<uint32> indices;

        for (uint32 i = 0; i < m_shapes[mesh].mesh.indices.size(); ++i)
        {
          indices.push_back(m_shapes[mesh].mesh.indices[i]);
        }

        for (uint32 i = 0; i < m_shapes[mesh].mesh.positions.size(); i += 3)
        {
          vertices.push_back(vec3(m_shapes[mesh].mesh.positions[i], m_shapes[mesh].mesh.positions[i + 1], m_shapes[mesh].mesh.positions[i + 2]));
        }

        for (uint32 i = 0; i < m_shapes[mesh].mesh.normals.size(); i += 3)
        {
          normals.push_back(vec3(m_shapes[mesh].mesh.normals[i], m_shapes[mesh].mesh.normals[i + 1], m_shapes[mesh].mesh.normals[i + 2]));
        }

        for (uint32 i = 0; i < m_shapes[mesh].mesh.texcoords.size(); i += 2)
        {
          uv.push_back(vec2(m_shapes[mesh].mesh.texcoords[i], 1.0f - m_shapes[mesh].mesh.texcoords[i + 1]));
        }

        m_shapes.clear();
        m_materials.clear();

        meshes_[index]->set_indices(indices);
        meshes_[index]->set_vertices(vertices);
        meshes_[index]->set_normals(normals);
        meshes_[index]->set_uv(uv);
      }
      else
      {
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [MESH] Could not load mesh %s (%s).\n", name.c_str(), err.c_str());
      }
      
      VXR_LOG(VXR_DEBUG_LEVEL_INFO, "[INFO]: [MESH] Loaded (%s).\n", name.c_str());
      VXR_TRACE_END("VXR", "Mesh Loading");
#ifdef VXR_THREADING
      meshes_[index]->loading_ = false;
    };
    Engine::ref().submitAsyncTask(task, &sync);
#endif
    return m;
  }

  ref_ptr<Mesh> AssetManager::default_cube() const
  {
    return meshes_[0];
  }

  ref_ptr<Mesh> AssetManager::default_quad() const
  {
    return meshes_[1];
  }

  ref_ptr<Composer> AssetManager::default_camera_composer() const
  {
    return default_composer_;
  }

}