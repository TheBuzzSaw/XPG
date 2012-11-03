#ifndef XpgWindowsHpp
#define XpgWindowsHpp

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#define WIN32_LEAN_AND_MEAN

#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN 0x020B
#endif
#ifndef WM_XBUTTONUP
#define WM_XBUTTONUP 0x020C
#endif
#ifndef GET_XBUTTON_WPARAM
#define GET_XBUTTON_WPARAM(w) (HIWORD(w))
#endif

#include <Windows.h>
#include <Windowsx.h>

namespace XPG
{
    static const char* const ClassName = "XPG";

    LRESULT CALLBACK SetupCallback(HWND window, UINT message, WPARAM wparam,
        LPARAM lparam);
}

#endif
