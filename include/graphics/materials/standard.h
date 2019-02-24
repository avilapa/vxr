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
* \file standard.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Engine default material definitions.
*
*/
namespace vxr
{

  namespace mat
  {
    class ScreenMaterial : public Material
    {
      VXR_OBJECT(ScreenMaterial, Material);
    public:
      ScreenMaterial();
    };

    class Std : public Material
    {
      VXR_OBJECT(Std, Material);
    public:
      Std();

      class Instance : public MaterialInstance
      {
        VXR_OBJECT(Instance, MaterialInstance);
      public:
        Instance();

        virtual void onGUI() override;

        void set_albedo(Color color);
        void set_albedo(ref_ptr<Texture> texture);
        void set_metallic(float value);
        void set_metallic(ref_ptr<Texture> texture);
        void set_roughness(float value);
        void set_roughness(ref_ptr<Texture> texture);
        void set_reflectance(float value);
        void set_ambient_occlusion(float value);
        void set_ambient_occlusion(ref_ptr<Texture> texture);
        void set_normal(ref_ptr<Texture> texture);

        void set_iridescence(float value);
        void set_iridescence_thickness(float value);
        void set_iridescence_thickness(ref_ptr<Texture> texture);
        void set_iridescence_base_ior(float value);
        void set_iridescence_extinction_k(float value);

        void set_clear_coat(float value);
        void set_clear_coat_roughness(float value);
        void set_clear_coat_normal(ref_ptr<Texture> texture);
      };
    };

  } /* end of mat namespace */

} /* end of vxr namespace */