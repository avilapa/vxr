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

#include "../../../include/graphics/materials/material.h"
#include "../../../include/graphics/materials/shader.h"
#include "../../../include/engine/engine.h"
#include "../../../include/engine/GPU.h"

namespace vxr
{

  namespace mat
  {

    Material::Material()
    {
      set_name("Material");
      if (gpu_.info.shader.vert == "")
      {
        gpu_.info.shader.vert = Shader::Load("unlit.vert");
      }
      if (gpu_.info.shader.frag == "")
      {
        gpu_.info.shader.frag = Shader::Load("unlit.frag");
      }
      gpu_.info.attribs[0] = { "attr_position", VertexFormat::Float3 };
      gpu_.info.attribs[1] = { "attr_normal",   VertexFormat::Float3 };
      gpu_.info.attribs[2] = { "attr_uv",       VertexFormat::Float2 };

      common_textures_ = 0;
    }

    Material::~Material()
    {
    }

    bool Material::setup()
    {
      if (!initialized_)
      {
        VXR_TRACE_SCOPE("VXR", "Material Setup");
        gpu_.mat = Engine::ref().gpu()->createMaterial(gpu_.info);

        if (use_uniforms_)
        {
          gpu_.uniform_buffer = Engine::ref().gpu()->createBuffer({ BufferType::Uniform, sizeof(Shader::UniformData), uniforms_usage_, uniforms_name_ });
        }
        initialized_ = true;
      }

      return true;
    }

    bool Material::setupTextureTypes(std::vector<ref_ptr<Texture>> textures)
    {
      for (uint32 i = common_textures_; i < gpu_.tex.size(); ++i)
      {
        if (!textures[i])
        {
          return false;
        }

        if (textures[i]->hasChanged())
        {
          if (!textures[i]->setup())
          {
            return false;
          }
        }
        gpu_.info.textures[i] = textures[i]->gpu_.info.type;
        gpu_.tex[i] = textures[i]->gpu_.tex;
      }

      return true;
    }

    uint32 Material::num_textures() const
    {
      return gpu_.tex.size();
    }

    void Material::set_uniforms_enabled(bool enabled)
    {
      use_uniforms_ = enabled;
    }

    void Material::set_uniforms_name(const char* name)
    {
      uniforms_name_ = name;
    }

    void Material::set_uniforms_usage(Usage::Enum usage)
    {
      uniforms_usage_ = usage;
    }

    void Material::set_num_textures(uint32 count)
    {
      gpu_.tex.resize(count);
    }

    void Material::set_shaders(const char* vert, const char* frag)
    {
      gpu_.info.shader.vert = Shader::Load(vert);
      gpu_.info.shader.frag = Shader::Load(frag);
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

    bool Material::uniforms_enabled() const
    {
      return use_uniforms_;
    }

    gpu::Material Material::material() const
    {
      return gpu_.mat;
    }

    gpu::Buffer Material::uniformBuffer() const
    {
      return gpu_.uniform_buffer;
    }

    std::vector<gpu::Texture> Material::textureInput() const
    {
      return gpu_.tex;
    }

    void Material::set_common_texture(uint32 index, ref_ptr<Texture> texture)
    {
      if (!texture)
      {
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [TEXTURE] Uninitialized material common texture.\n")
        return;
      }

      if ((index + 1) > common_textures_)
      {
        common_textures_ = index + 1;
      }

      if (texture->hasChanged())
      {
        if (!texture->setup())
        {
          VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [TEXTURE] Could not setup common texture.\n")
          return;
        }
      }

      gpu_.info.textures[index] = texture->gpu_.info.type;
      gpu_.tex[index] = texture->gpu_.tex;
    }

  }

  string Shader::Load(const char* file)
  {
#if defined (VXR_OPENGL)
    string path = "../../src/graphics/backend/opengl/shaders/";
#elif defined (VXR_DX11)
    string path = "";
#else
#  error Backend must be defined on GENie.lua (e.g. --gl OR --dx11).
#endif
    string content;
    std::ifstream file_stream(file, std::ios::in);

    if (!file_stream.is_open())
    {
      file_stream = std::ifstream(path + file, std::ios::in);
      if (!file_stream.is_open())
      {
        VXR_LOG(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: [MATERIAL] Could not read file '%s', file does not exist.\n", file);
        return "";
      }
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