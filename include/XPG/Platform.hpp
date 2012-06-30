#ifndef XpgPlatformHpp
#define XpgPlatformHpp

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
#   define XpgFunction extern
#   define XpgClass class
#elif defined(XpgPlatformWindows)
#   ifdef XpgBuild
#       define XpgFunction extern __declspec(dllexport)
#       define XpgClass class __declspec(dllexport)
#   else
#       define XpgFunction extern __declspec(dllimport)
#       define XpgClass class __declspec(dllimport)
#   endif
#else
#   define XpgFunction extern __attribute__ ((visibility("default")))
#   define XpgClass class __attribute__ ((visibility("default")))
#endif

#endif
