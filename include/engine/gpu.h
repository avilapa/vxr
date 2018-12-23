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

#include "../graphics/window.h"
#include "../graphics/display_list.h"
#include "../graphics/render_context.h"

#include <atomic>
#include <thread>
#include <mutex>

/// WIP: Resources are not destroyed yet!

/**
* \file gpu.h
*
* \author Victor Avila (avilapa.github.io)
*
* \brief GPU class in charge of all the threading and syncrhonization, resource creation, window handling and render commands execution.
*
*/
namespace vxr 
{

	class GPU : public Object 
  {
    VXR_OBJECT(GPU, Object);
    friend class Engine;
	public:
		GPU();
		virtual ~GPU();

    void set_preinit_params(const Params::GPU& params) { params_ = params; };

    gpu::Buffer createBuffer(const gpu::Buffer::Info& info);
    gpu::Texture createTexture(const gpu::Texture::Info& info);
    gpu::Material createMaterial(const gpu::Material::Info& info);
    gpu::Framebuffer createFramebuffer(const gpu::Framebuffer::Info &info);

    uint32 num_buffers() const;
    uint32 num_textures() const;
    uint32 num_materials() const;
    uint32 num_framebuffers() const;

    uint32 num_used_buffers() const;
    uint32 num_used_textures() const;
    uint32 num_used_materials() const;
    uint32 num_used_framebuffers() const;

    RenderContext* ctx_;

  protected:
    void init();
    void update();
    void stop();

#ifdef VXR_THREADING
    void run();
    void prepareRender();
#endif
    void execute();

    bool is_exiting() { return is_exiting_; }

  protected:
    bool is_exiting_ = false;
    Params::GPU params_ = { 128, 128, 128, 128 };
    
    ref_ptr<Window> window_;
#ifdef VXR_UI
    std::function<void()> ui_ = []() {};
#endif

    void moveOrAppendCommands(DisplayList &&dl);

    scoped_ptr<DisplayList> logic_frame_;
    DisplayList render_frame_;

#ifdef VXR_THREADING
    struct ThreadSync 
    {
      DisplayList next_frame;
      std::thread thread;
      std::mutex mx_r;
      std::mutex mx_l;
      std::condition_variable cv_r;
      std::condition_variable cv_l;
      bool initialized = false;
    } thread_data_;
#endif

  private:
    uint32 num_used_buffers_ = 0;
    uint32 num_used_textures_ = 0;
    uint32 num_used_materials_ = 0;
    uint32 num_used_framebuffers_ = 0;
	};

} /* end of vxr namespace */