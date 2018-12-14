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

#include "material.h"
#include "material_instance.h"

/**
* \file default_materials.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Engine default material definitions.
*
*/
namespace vxr
{

  class Screen : public Material
  {
    VXR_OBJECT(Screen, Material);
  public:
    Screen();
  };

  class Unlit : public Material
  {
    VXR_OBJECT(Unlit, Material);
  public:
    Unlit();

    class Instance : public MaterialInstance
    {
      VXR_OBJECT(Instance, MaterialInstance);
    public:
      Instance();

      virtual void onGUI() override;

      void set_color(Color color);
      Color color() const;
    };
  };

  class Wireframe : public Material
  {
    VXR_OBJECT(Wireframe, Material);
  public:
    Wireframe();

    class Instance : public MaterialInstance
    {
      VXR_OBJECT(Instance, MaterialInstance);
    public:
      Instance();

      virtual void onGUI() override;

      void set_color(Color color);
      Color color() const;
    };
  };

  class Standard : public Material
  {
    VXR_OBJECT(Standard, Material);
  public:
    Standard();

    class Textured : public Material
    {
      VXR_OBJECT(Textured, Material);
    public:
      Textured();
    };

    class TexturedCubemap : public Material
    {
      VXR_OBJECT(TexturedCubemap, Material);
    public:
      TexturedCubemap();
    };

    class Instance : public MaterialInstance
    {
      VXR_OBJECT(Instance, MaterialInstance);
    public:
      Instance();

      virtual void onGUI() override;

      void set_color(Color color);
      void set_color_texture(const char* file);
      void set_color_texture(const char* folder, const char* extension);
      void set_color_texture(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft);
      
      Color color() const;
      ref_ptr<Texture> color_texture() const;

    protected:
      ref_ptr<Texture> color_texture_;
    };
  };

  class Skybox : public Material
  {
    VXR_OBJECT(Skybox, Material);
  public:
    Skybox();

    class Instance : public MaterialInstance
    {
      VXR_OBJECT(Instance, MaterialInstance);
    public:
      Instance();

      virtual void onGUI() override;

      void set_color_texture(const char* folder, const char* extension);
      void set_color_texture(const char* rt, const char* lf, const char* up, const char* dn, const char* bk, const char* ft);

      ref_ptr<Texture> color_texture() const;

    protected:
      ref_ptr<Texture> skybox_texture_;
    };
  };

} /* end of vxr namespace */