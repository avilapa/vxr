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

#include "../core/component.h"
#include "../graphics/materials/shader.h"
#include "../graphics/materials/pass_standard.h"
#include "../graphics/materials/pass_ibl.h"

/**
* \file ibl.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief Indirect Light Component for Image Based Lighting (IBL).
*
*/
namespace vxr 
{
  namespace System { class IBL; }
  namespace mesh { class Cube; class Quad; }

	class IBL : public Component
  {
    VXR_OBJECT(IBL, Component);
    friend class System::IBL;
	public:
    IBL();
		~IBL();

    virtual void onGUI() override;

    // Input a cubemap texture, or a 2D equirectangular texture.
    void set_texture(ref_ptr<Texture> texture);

    ref_ptr<Texture> cubemap_texture() const;
    ref_ptr<Texture> irradiance_cubemap_texture() const;
    ref_ptr<Texture> prefiltered_cubemap_texture() const;
    ref_ptr<Texture> brdf_lut() const;

    bool initialized() const;

	private:
    uint32 initialization_level_;
    bool contributes_;

    ref_ptr<Texture> cubemap_texture_;
    ref_ptr<mat::ComputeIrradiance::Instance> compute_irradiance_;
    ref_ptr<mat::PrefilterCubemap::Instance> prefiltered_cubemap_;
    ref_ptr<mat::BRDFIntegration::Instance> brdf_integration_;
    ref_ptr<mat::BuildCubemap::Instance> build_cubemap_;
	};

  class Scene;

  namespace System 
  {
    class IBL : public ComponentSystem
    {
      VXR_OBJECT(System::IBL, ComponentSystem);
    public:
      IBL();
      ~IBL();

      void set_main(ref_ptr<vxr::IBL> light);
      ref_ptr<vxr::IBL> main() const;

      void init() override;
      void update() override;
      void renderUpdate() override;
      void renderPostUpdate() override;

      void subscribeMaterialForIBLTextures(const string& material_name);

    private:
      bool buildCubemap();
      bool computeIrradiance();
      bool computePrefiltering();
      bool integrateBRDF();

      void updateMaterialIBLTextures();
      void cleanMaterialIBLTextures();

    private:
      std::vector<ref_ptr<vxr::IBL>> components_;
      std::vector<string> pbr_materials_;
      ref_ptr<Scene> scene_;
      ref_ptr<vxr::IBL> main_;

    public:
      template<typename T> ref_ptr<T> createInstance()
      {
        ref_ptr<T> c;
        c.alloc();
        components_.push_back(c.get());
        return c.get();
      }
    };

    template<> class Getter<vxr::IBL>
    {
    public:
      static ref_ptr<System::IBL> get();
    };

  }

} /* end of vxr namespace */