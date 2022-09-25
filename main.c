#define WIN32_LEAN_AND_MEAN
#define DEBUG_BREAK() __debugbreak()
#include<windows.h>

#include"header.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")

static void
OpenDebugConsole(void)
{
  #if _DEBUG
  AllocConsole();
  HWND ConsoleWindow = GetConsoleWindow();
  SetWindowPos(ConsoleWindow, 0, 1920, 200, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  DWORD ProcessID = GetCurrentProcessId();
  AttachConsole(ProcessID);
  freopen("CON", "w", stdout);
  #endif
}

static void
DisableDPIScaling(void)
{
  HMODULE User32 = LoadLibraryW(L"user32.dll");
  void* FunctionPointer = GetProcAddress(User32, "SetProcessDpiAwarenessContext");
  if(FunctionPointer)
  {
    void (*SetProcessDpiAwarenessContext)(DPI_AWARENESS_CONTEXT) = FunctionPointer;
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
    return;
  }
  FunctionPointer = GetProcAddress(User32, "SetProcessDpiAware");
  if(FunctionPointer)
  {
    void (*SetProcessDpiAware)(void) = FunctionPointer;
    SetProcessDpiAware();
  }
}

static LRESULT CALLBACK
WindowProc(HWND Window, UINT Message, WPARAM Wparam, LPARAM Lparam)
{
  LRESULT Result = 0;
  switch(Message)
  {
    case WM_CLOSE:
    {
      PostQuitMessage(0);
    } break;
    default:
    {
      Result = DefWindowProcW(Window, Message, Wparam, Lparam);
    } break;
  }
  return Result;
}

static HWND
CreateOutputWindow(void)
{
  WNDCLASSEXW WindowClass = {0};
  WindowClass.cbSize = sizeof(WindowClass);
  WindowClass.hInstance = GetModuleHandleW(NULL);
  WindowClass.lpfnWndProc = WindowProc;
  WindowClass.style = CS_HREDRAW | CS_VREDRAW;
  WindowClass.lpszClassName = L"non_euclidian_wndclass";
  RegisterClassExW(&WindowClass);
  DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
  HWND Result = CreateWindowExW(0, WindowClass.lpszClassName, L"NonEuclidian Demo", WindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, NULL, NULL, WindowClass.hInstance, NULL);
  ASSERT(IsWindow(Result));
  return Result;
}

static HGLRC
CreateOpenGLContext(HWND Window)
{
  PIXELFORMATDESCRIPTOR Descriptor = {0};
  Descriptor.nSize = sizeof(Descriptor);
  Descriptor.nVersion = 1;
  Descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  Descriptor.iPixelType = PFD_TYPE_RGBA;
  Descriptor.cColorBits = 32;
  Descriptor.cDepthBits = 32;
  Descriptor.iLayerType = PFD_MAIN_PLANE;
  HDC DeviceContext = GetDC(Window);
  int PixelFormat = ChoosePixelFormat(DeviceContext, &Descriptor);
  SetPixelFormat(DeviceContext, PixelFormat, &Descriptor);
  DescribePixelFormat(DeviceContext, PixelFormat, sizeof(Descriptor), &Descriptor);
  HGLRC Result = wglCreateContext(DeviceContext);
  wglMakeCurrent(DeviceContext, Result);
  return Result;
}

int APIENTRY
wWinMain(HINSTANCE Instance, HINSTANCE Unused, LPWSTR CmdLine, int CmdShow)
{
  DisableDPIScaling();
  HWND Window = CreateOutputWindow();
  HGLRC OpenGLContext = CreateOpenGLContext(Window);

  MSG Message = {0};
  for(;;)
  {
    GetMessageW(&Message, NULL, 0, 0);
    if(Message.message == WM_QUIT)
    {
      break;
    }
    TranslateMessage(&Message);
    DispatchMessageW(&Message);
  }
  int ExitCode = (int)Message.wParam;
  return ExitCode;
}