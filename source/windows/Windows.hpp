#ifndef XpgWindowsHpp
#define XpgWindowsHpp

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

namespace XPG
{
    static const char* const ClassName = "XPG";

    LRESULT CALLBACK SetupCallback(HWND window, UINT message, WPARAM wparam,
        LPARAM lparam);
}

#endif
