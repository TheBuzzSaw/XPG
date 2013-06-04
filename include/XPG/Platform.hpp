#ifndef XpgPlatformHpp
#define XpgPlatformHpp

#define XpgLittleEndian

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define XpgPlatformWindows
#else
#   define XpgPlatformUnix
#   if defined(__APPLE__) || defined(__APPLE_CC__)
#       define XpgPlatformOsx
#   else
#       define XpgPlatformLinux
#       if defined(__ANDROID__) || defined(ANDROID)
#           define XpgPlatformAndroid
#       endif
#   endif
#endif

#ifdef XpgStatic
#   define XpgApi
#elif defined(XpgPlatformWindows)
#   ifdef XpgBuild
#       define XpgApi __declspec(dllexport)
#   else
#       define XpgApi __declspec(dllimport)
#   endif
#else
#   define XpgApi __attribute__ ((visibility("default")))
#endif

#endif
