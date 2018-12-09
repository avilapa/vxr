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

#include "../graphics/material.h"

/**
* \file material.h
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

    virtual void onGUI() override;
  };

  class Unlit : public Material
  {
    VXR_OBJECT(Unlit, Material);
  public:
    Unlit();

    virtual void onGUI() override;

    void set_color(Color color);
    Color color() const;
  };

  class Standard : public Material
  {
    VXR_OBJECT(Standard, Material);
  public:
    Standard();

    virtual void onGUI() override;

    void set_color(Color color);
    Color color() const;
  };

} /* end of vxr namespace */