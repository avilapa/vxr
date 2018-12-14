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

#include "../../../include/graphics/materials/material_instance.h"
#include "../../../include/graphics/materials/material.h"
#include "../../../include/engine/engine.h"
#include "../../../include/graphics/ui.h"

namespace vxr
{

  MaterialInstance::MaterialInstance() :
    active_material_(0)/// vars
  {

  }

  MaterialInstance::~MaterialInstance()
  {

  }

  void MaterialInstance::onGUI()
  {
    ImGui::Text((name() + "(Shader: " + type() + ")").c_str());
    ImGui::Spacing();
  }

  void MaterialInstance::init(string shared_material_name, uint32 i)
  {
    uint32 num_materials = i + 1;
    if (shared_materials_.size() < num_materials)
    {
      shared_materials_.resize(num_materials);
      textures_.resize(num_materials);
    }

    shared_materials_[i] = Engine::ref().assetManager()->getSharedMaterial(shared_material_name.c_str());

    if (!shared_materials_[i])
    {
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not intialize textures for %s (%s). Unknown shared material.\n", name().c_str(), type().c_str());
      return;
    }

    textures_[i].resize(shared_materials_[i]->num_textures());
  }

  void MaterialInstance::init(std::initializer_list<string> shared_material_names)
  {
    uint32 i = 0;
    for (auto material : shared_material_names)
    {
      init(material, i++);
    }
  }

  void MaterialInstance::set_active_material(uint32 index)
  {
    active_material_ = index;
  }

  uint32 MaterialInstance::active_material() const
  {
    return active_material_;
  }

  ref_ptr<Material> MaterialInstance::sharedMaterial() const
  {
    return shared_materials_[active_material_];
  }

  void MaterialInstance::set_texture(uint32 index, ref_ptr<Texture> texture)
  {
    textures_[active_material_][index] = texture;
  }

  ref_ptr<Texture> MaterialInstance::texture(uint32 index /* = 0 */) const
  {
    return textures_[active_material_][index];
  }
}