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

#include "../../include/graphics/material.h"

#include "../../include/engine/engine.h"
#include "../../include/graphics/ui.h"

namespace vxr
{

  Material::Material()
  {
    set_name("Material");
    if (gpu_.info.shader.vert == "")
    {
      gpu_.info.shader.vert = Shader::load("../../assets/shaders/glsl/standard.vert");
    }
    if (gpu_.info.shader.frag == "")
    {
      gpu_.info.shader.frag = Shader::load("../../assets/shaders/glsl/standard.frag");
    }
    gpu_.info.attribs[0] = { "a_position", VertexFormat::Float3 };
    gpu_.info.attribs[1] = { "a_normal",   VertexFormat::Float3 };
    gpu_.info.attribs[2] = { "a_uv",       VertexFormat::Float2 };

    uniforms_.u.specific.std.color = { 1.0f, 1.0f, 1.0f, 1.0f };
  }

  Material::~Material()
  {
    
  }

  void Material::onGUI()
  {
    ImGui::Text("Shader: Standard");
    ImGui::Spacing();
    ImGui::ColorEdit4("Color", (float*)&uniforms_.u.specific.std.color);
  }

  void Material::setup()
  {
    VXR_TRACE_SCOPE("VXR", "Material Setup");
    for (uint32 i = 0; i < textures_.size(); ++i)
    {
      gpu_.info.textures[i] = TextureType::T2D;
      gpu_.tex.push_back(textures_[i]->gpu_.tex);
    }

    gpu_.mat = Engine::ref().gpu()->createMaterial(gpu_.info);

    gpu_.uniform_buffer = Engine::ref().gpu()->createBuffer({ BufferType::Uniform, sizeof(uniforms_.u), uniforms_.usage, uniforms_.name });
    initialized_ = true;
  }

  void Material::setupTextures()
  {
    for (uint32 i = 0; i < textures_.size(); ++i)
    {
      if (textures_[i]->hasChanged())
      {
        textures_[i]->setup();
      }
    }
  }

  void Material::addTexture(ref_ptr<Texture> texture)
  {
    textures_.push_back(texture);
  }

  ref_ptr<Texture> Material::texture(uint32 index /* = 0 */)
  {
    return textures_[index];
  }

  void Material::set_uniforms(Shader::Data uniforms)
  {
    uniforms_ = uniforms;
  }

  void Material::set_shaders(const char* vert, const char* frag)
  {
    gpu_.info.shader.vert = Shader::load(vert);
    gpu_.info.shader.frag = Shader::load(frag);
  }

  void Material::set_cull(Cull::Enum cull)
  {
    gpu_.info.cull = cull;
  }

  void Material::set_render_mode(RenderMode::Enum render_mode)
  {
    gpu_.info.render_mode = render_mode;
  }

  void Material::set_blend_params(bool enabled, vec4 color, BlendFactor::Enum src_rgb, BlendFactor::Enum dst_rgb, BlendOp::Enum op_rgb, BlendFactor::Enum src_alpha, BlendFactor::Enum dst_alpha, BlendOp::Enum op_alpha)
  {
    gpu_.info.blend.enabled = enabled;
    gpu_.info.blend.color = color;
    gpu_.info.blend.src_rgb = src_rgb;
    gpu_.info.blend.dst_rgb = dst_rgb;
    gpu_.info.blend.op_rgb = op_rgb;
    gpu_.info.blend.src_alpha = src_alpha;
    gpu_.info.blend.dst_alpha = dst_alpha;
    gpu_.info.blend.op_alpha = op_alpha;
  }

  void Material::set_depth_func(CompareFunc::Enum depth_func)
  {
    gpu_.info.depth_func = depth_func;
  }

  void Material::set_rgba_write(bool enabled)
  {
    gpu_.info.rgba_write = enabled;
  }

  void Material::set_depth_write(bool enabled)
  {
    gpu_.info.depth_write = enabled;
  }

  bool Material::initialized()
  {
    return initialized_;
  }

  string Shader::load(const char* file)
  {
    string content;
    std::ifstream file_stream(file, std::ios::in);

    if (!file_stream.is_open())
    {
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not read file '%s', file does not exist.\n", file);
      return "";
    }

    string line = "";
    while (!file_stream.eof()) {
      std::getline(file_stream, line);
      content.append(line + "\n");
    }

    file_stream.close();
    return content;
  }

}