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

#include "../../../include/graphics/ui/log.h"

namespace vxr
{

  void ui::EditorLog::Clear()
  {
    buffer.clear(); line_offsets.clear(); last_msg = "";
  }

  void ui::EditorLog::AddLog(const char* fmt, ...) IM_FMTARGS(2)
  {
    if (fmt == last_msg.c_str())
    {
      return;
    }
    last_msg = fmt;
    int old_size = buffer.size();
    va_list args;
    va_start(args, fmt);
    buffer.appendfv(fmt, args);
    va_end(args);
    for (int new_size = buffer.size(); old_size < new_size; old_size++)
    {
      if (buffer[old_size] == '\n')
      {
        line_offsets.push_back(old_size);
      }
    }
    scroll_to_bottom = true;
  }

  void ui::EditorLog::Draw()
  {
    if (ImGui::Button("Clear")) Clear();
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    filter.Draw("Filter", -100.0f);
    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (copy)
    {
      ImGui::LogToClipboard();
    }

    if (filter.IsActive())
    {
      const char* buf_begin = buffer.begin();
      const char* line = buf_begin;
      for (int line_no = 0; line != NULL; line_no++)
      {
        const char* line_end = (line_no < line_offsets.Size) ? buf_begin + line_offsets[line_no] : NULL;
        if (filter.PassFilter(line, line_end))
        {
          ImGui::TextUnformatted(line, line_end);
        }
        line = line_end && line_end[1] ? line_end + 1 : NULL;
      }
    }
    else
    {
      ImGui::TextUnformatted(buffer.begin());
    }

    if (scroll_to_bottom)
    {
      ImGui::SetScrollHere(1.0f);
    }
    scroll_to_bottom = false;
    ImGui::EndChild();
  }

} /* end of vxr namespace */