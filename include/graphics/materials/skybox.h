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
* \file skybox.h
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

        void set_tint(Color color);
        Color tint() const;

        void set_color_texture(ref_ptr<Texture> texture);
        ref_ptr<Texture> color_texture() const;
      };
    };

  } /* end of mat namespace */

} /* end of vxr namespace */