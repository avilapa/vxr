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

#include "../../include/engine/gpu.h"

#include "../../include/graphics/ui.h"

#include <iterator>

namespace vxr 
{
  GPU::GPU()
  {
    set_name("GPU");
    ctx_ = new RenderContext();
    window_.alloc();
    logic_frame_.alloc();
	}

	GPU::~GPU() 
  {  
    delete ctx_;
  }

  void GPU::init()
  {
    ctx_->init(params_);
#ifndef VXR_THREADING
    window_->init();
#else
    thread_data_.thread = std::thread(&vxr::GPU::run, this);
    VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_INFO, "[INFO]: Launched rendering thread (VXR_THREADING).\n");
    std::unique_lock<std::mutex> lock(thread_data_.mx_l);
    thread_data_.cv_l.wait(lock, [this] { return thread_data_.initialized; });
#endif
  }

  void GPU::update()
  {
#ifndef VXR_THREADING
    bool swap = true;
    window_->update(&swap);
#  ifdef VXR_UI
    ui_();
#  endif
    render_frame_.update();
#endif
  }

  void GPU::stop()
  {
#ifndef VXR_THREADING
    window_->stop();
#else
    thread_data_.thread.join();
    VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_INFO, "[INFO]: Joined rendering thread (VXR_THREADING).\n");
#endif
  }

#ifdef VXR_THREADING
  void GPU::run()
  {
    VXR_TRACE_META_THREAD_NAME("Render Thread");
    VXR_TRACE_SCOPE("VXR", __FUNCTION__);
    window_->init();

    thread_data_.initialized = true;
    thread_data_.cv_l.notify_one();

    bool swap = false;

    while (!(is_exiting_ = window_->is_exiting()))
    {
      VXR_TRACE_BEGIN("VXR", "Frame");
      VXR_TRACE_BEGIN("VXR", "Window Update");
      window_->update(&swap);
      VXR_TRACE_END("VXR", "Window Update");
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: GPU Synchronization (Render Waiting).\n");
      VXR_TRACE_BEGIN("VXR", "WAITING (Render)");
      std::unique_lock<std::mutex> lock(thread_data_.mx_r); /// First condition stops engine when there are 0 commands. /*!thread_data_.next_frame.commands_.empty() &&*/
      thread_data_.cv_r.wait(lock, [this] { return !thread_data_.next_frame.commands_.empty() && render_frame_.commands_.empty(); });
      VXR_TRACE_END("VXR", "WAITING (Render)");
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: GPU Synchronization (Render Start).\n");
      if (!thread_data_.next_frame.commands_.empty())
      {
        render_frame_.commands_.swap(thread_data_.next_frame.commands_);
        swap = true;
      }
      //thread_data_.cv_l.notify_one();
      render_frame_.update();
#  ifdef VXR_UI /// UI needs to go before notify, but frame update could be done after that
      VXR_TRACE_BEGIN("VXR", "UI Function");
      ui_(); /// If changed: swap = true ?
      VXR_TRACE_END("VXR", "UI Function");
#  endif
      thread_data_.cv_l.notify_one();
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: GPU Synchronization (Render Ready).\n");
      VXR_TRACE_END("VXR", "Frame");
    }

    window_->stop();
    thread_data_.cv_l.notify_one();
  }

  void GPU::prepareRender()
  {
    VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: GPU Synchronization (Logic Waiting).\n");
    VXR_TRACE_BEGIN("VXR", "WAITING (Logic)");
    std::unique_lock<std::mutex> lock(thread_data_.mx_l);
    thread_data_.cv_l.wait(lock, [this] { return is_exiting_ || thread_data_.next_frame.commands_.empty(); });
    VXR_TRACE_END("VXR", "WAITING (Logic)");
    VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: GPU Synchronization (Logic Start).\n");
  }
#endif

  void GPU::execute()
  {
#ifdef VXR_THREADING
    if (logic_frame_ != NULL)
    {
      thread_data_.next_frame.commands_ = std::move(logic_frame_->commands_);
    }
    thread_data_.cv_r.notify_one();
    VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_DEBUG, "[DEBUG]: GPU Synchronization (Logic Ready).\n");
#else
    if (logic_frame_ != NULL)
    {
      render_frame_.commands_ = std::move(logic_frame_->commands_);
    }
    update();
#endif
  }

  void GPU::moveOrAppendCommands(DisplayList &&dl)
  {
    if (logic_frame_->commands_.empty())
    {
      logic_frame_->commands_ = std::move(dl.commands_);
    }
    else
    {
      logic_frame_->commands_.reserve(logic_frame_->commands_.size() + dl.commands_.size());
      std::move(std::begin(dl.commands_), std::end(dl.commands_), std::back_inserter(logic_frame_->commands_));
      dl.commands_.clear();
    }
  }
  
// ----------------------------------------------------------------------------------------
//  The following functions have been partially extracted from px_render.h 
//  by Jose L. Hidalgo (PpluX), and later modified to fit vxr needs.
//  Link: https://github.com/pplux/px/blob/master/px_render.h

  template<class T>
  static uint32 AcquireResource(scoped_array<T>* pool) 
  {
    uint32 try_count = 10;
    while (try_count--) 
    {
      for (uint32 i = 0; i < pool->size(); ++i) 
      {
        if ((*pool)[i].acquire()) 
        {
          uint32 version = (*pool)[i].version;
          uint32 result = i | (version << 20);
          return result;
        }
      }
    }
    VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Assigned id 0 to resource instance.\n");
    return 0;
  }

  gpu::Buffer GPU::createBuffer(const gpu::Buffer::Info& info)
  {
    uint32 id = AcquireResource(&ctx_->buffers_);
    uint32 pos = RenderContext::index(id);
    gpu::BufferInstance &inst = ctx_->buffers_[pos];
    inst.info = info;
    return gpu::Buffer{ ctx_,id };
  }

  gpu::Texture GPU::createTexture(const gpu::Texture::Info &info)
  {
    if (info.format == TexelsFormat::None)
    {
      VXR_DEBUG_FUNC(VXR_DEBUG_LEVEL_ERROR, "[ERROR]: Could not create texture: texels format not found.\n");
      return gpu::Texture();
    }

    uint32_t id = AcquireResource(&ctx_->textures_);
    uint32_t pos = RenderContext::index(id);
    gpu::TextureInstance &i_obj = ctx_->textures_[pos];
    i_obj.info = info;

    switch (info.format) 
    {
    case TexelsFormat::R_U8:
      i_obj.bytes_per_pixel = 1;
      break;
    case TexelsFormat::RG_U8:
      i_obj.bytes_per_pixel = 2;
      break;
    case TexelsFormat::RGB_U8:
      i_obj.bytes_per_pixel = 3;
      break;
    case TexelsFormat::RGBA_U8:
      i_obj.bytes_per_pixel = 4;
      break;
    case TexelsFormat::Depth_U16:
      i_obj.bytes_per_pixel = 2;
      break;
    case TexelsFormat::DepthStencil_U16:
      i_obj.bytes_per_pixel = 4;
      break;
    }

    return gpu::Texture{ ctx_, id };
  }

  static size_t ComputeVertexSize(uint32 format) 
  {
    uint32 type = (format & VertexFormat::TypeMask) >> VertexFormat::TypeShift;
    uint32 result = (format & VertexFormat::NumComponentsMask) >> VertexFormat::NumComponentsShift;
    switch (type) {
      // 1
    case VertexFormat::Int8:  break;
    case VertexFormat::UInt8: break;
      // 2
    case VertexFormat::Int16:
    case VertexFormat::UInt16: result *= 2; break;
      // 4
    case VertexFormat::Int32:
    case VertexFormat::UInt32:
    case VertexFormat::Float: result *= 4; break;
    default:
      // ERROR
      return 0;
    }
    return result;
  }

  gpu::Material GPU::createMaterial(const gpu::Material::Info &info)
  {
    uint32 id = AcquireResource(&ctx_->materials_);
    uint32 pos = ctx_->index(id);
    gpu::MaterialInstance &inst = ctx_->materials_[pos];
    inst.info = info;

    inst.frag_shader.assign(info.shader.frag);
    inst.vert_shader.assign(info.shader.vert);
    
    inst.info.shader.frag = inst.frag_shader.c_str();
    inst.info.shader.vert = inst.vert_shader.c_str();

    // Compute strides/offsets
    size_t strides_by_buffer_index[kMaxVertexAttribs] = {};
    for (auto i = 0; i < kMaxVertexAttribs; ++i) 
    {
      const char *name = info.attribs[i].name;
      if (name) 
      {
        inst.attributes[i].assign(name);
        inst.info.attribs[i].name = inst.attributes[i].c_str();
      }
      if (!inst.info.attribs[i].offset) 
      {
        inst.info.attribs[i].offset = strides_by_buffer_index[inst.info.attribs[i].buffer_index];
      }
      strides_by_buffer_index[inst.info.attribs[i].buffer_index] += ComputeVertexSize(inst.info.attribs[i].format);
    }

    for (auto i = 0; i < kMaxVertexAttribs; ++i) 
    {
      auto &a = inst.info.attribs[i];
      if (!a.stride)
      {
        a.stride = strides_by_buffer_index[a.buffer_index];
      }
    }

    return gpu::Material{ ctx_, id };
  }

  gpu::Framebuffer GPU::createFramebuffer(const gpu::Framebuffer::Info &info)
  {
    uint32 id = AcquireResource(&ctx_->framebuffers_);
    uint32 pos = RenderContext::index(id);
    gpu::FramebufferInstance &i_obj = ctx_->framebuffers_[pos];
    i_obj.info = info;
    for (uint32_t i = 0; i < info.num_color_textures; ++i) {
      i_obj.color_textures[i] = createTexture(info.color_texture_info);/// Should this be an array of texture Infos?
    }
    i_obj.depth_texture = createTexture(info.depth_stencil_texture_info);

    return gpu::Framebuffer{ ctx_,id };
  }

// ----------------------------------------------------------------------------------------

} /* end of vxr namespace */