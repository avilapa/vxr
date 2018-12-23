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

#include "../graphics/materials/standard_pass.h"
#include "../graphics/materials/standard.h"

/**
* \file composer.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Camera composer class.
*
*/
namespace vxr
{

  namespace mesh { class Quad; }
  namespace mat { class MaterialInstance; class RenderPassInstance; }

  class Composer : public Object
  {
    VXR_OBJECT(Composer, Object);
    friend class System::Camera;
  public:
    Composer();
    virtual ~Composer();
    virtual void onGUI() override;

    void init();

    void addRenderPass(ref_ptr<mat::RenderPassInstance> pass);

    void set_render_size(uvec2 size);

    uint32 final_texture_id();

  private:
    void setupFirstPass();
    void setupLastPass();
    void renderToScreen();
    void applyPostprocessing();

  private:
    std::vector<ref_ptr<mat::RenderPassInstance>> render_passes_;

    uvec2 render_size_;

    ref_ptr<mat::MaterialInstance> render_to_screen_;
    ref_ptr<mat::RenderPassInstance> screen_prepass_;
    ref_ptr<mesh::Quad> screen_quad_;
    ref_ptr<Texture> screen_texture_;
    ref_ptr<Texture> displaytest;

    bool initialized_ = false;
  };

} /* end of vxr namespace */