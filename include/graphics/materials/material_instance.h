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

#include "shader.h"
#include "../../core/object.h"

#include <initializer_list>
#include <functional>

/**
* \file material_instance.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Material Instance base class.
*
*/
namespace vxr
{
  namespace System { class Renderer; }
  class Composer;
  class Texture;

  namespace mat
  {

    class Material;

    class MaterialInstance : public Object
    {
      VXR_OBJECT(MaterialInstance, Object);
      friend class System::Renderer;
      friend class AssetManager;
      friend class Composer;
    public:
      MaterialInstance();
      virtual ~MaterialInstance();

      virtual void onGUI() override;

      void init(string shared_material_name, uint32 index = 0);
      void init(std::initializer_list<string> shared_material_names);

      void set_active_material(uint32 index);
      uint32 active_material() const;

      ref_ptr<Material> sharedMaterial() const;

      void set_texture(uint32 index, ref_ptr<Texture> texture);
      ref_ptr<Texture> texture(uint32 index = 0) const;
      std::vector<ref_ptr<Texture>> textures() const;

    protected:
      Shader::UniformData uniforms_;

    private:
      uint32 active_material_;
      std::vector<ref_ptr<Material>> shared_materials_;
      std::vector<std::vector<ref_ptr<Texture>>> textures_;
    };

  } /* end of mat namespace */

} /* end of vxr namespace */