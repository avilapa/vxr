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

#include "../../../include/core/gameobject.h"
#include "../../../include/components/renderer.h"
#include "../../../include/utils/minmax.h"
#include "../../../include/utils/gradient.h"
#include "material/material.h"

namespace vxr 
{

  struct ColorSettings : public Object
  {
    VXR_OBJECT(ColorSettings, Object);

    Color wireframe_color = { 1,1,1,1 };

    virtual bool equals(ref_ptr<ColorSettings> o);
    virtual void assign(ref_ptr<ColorSettings> o);
  };

  class ColorGenerator : public Object
  {
    VXR_OBJECT(ColorGenerator, Object);
  public:
    void init(ref_ptr<ColorSettings> shapeSettings);
    void updateColors();
    void updateElevation(vec2 elevationMinMax);

    bool hasChanged();

    virtual void onGUI() override;

    ref_ptr<ColorSettings> settings;
    ref_ptr<ColorSettings> uiSettings;

    Shader::Data data;
    Gradient gradient;
    ref_ptr<Texture> gradientTexture;
    ref_ptr<PlanetMaterial> mat;

    Shader::Data wireframe_data;
    ref_ptr<Material> wireframe_mat;

  private:
    bool dirty_ = false;
  };

} /* end of vxr namespace */