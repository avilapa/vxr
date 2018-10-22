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

#include "dx11_window.h"

#ifdef VXR_DX11

namespace vxr
{
  HINSTANCE               g_hInst = NULL;
  HWND                    g_hWnd = NULL;

  LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
  }

  void window::init(Window::Data* data)
  {
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    RegisterClassEx(&wc);
    data->window = CreateWindow(L"ImGui Example", L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Show the window
    ShowWindow(data->window, SW_SHOWDEFAULT);
    UpdateWindow(data->window);

  }

  void window::swap(Window::Data* data)
  {

  }

  void window::events(Window::Data* data)
  {
    if (PeekMessage(&data->msg, NULL, 0U, 0U, PM_REMOVE))
    {
      TranslateMessage(&data->msg);
      DispatchMessage(&data->msg);
    }
  }

  void window::stop(Window::Data* data)
  {
    DestroyWindow(data->window);
  }

  bool window::is_exiting(Window::Data* data)
  {
    return data->msg.message == WM_QUIT;
  }

  double window::uptime(Window::Data* data)
  {
    static double t = 0.0;
    t += 0.0001;
    return t;
  }

  void window::forceExit(Window::Data* data)
  {

  }

  void window::set_title(Window::Data* data)
  {

  }

}

#endif