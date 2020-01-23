
#ifndef GAINPUTWINDOWS_H_
#define GAINPUTWINDOWS_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#if defined(__MINGW32__)
#include <windows.h>
#include <windowsx.h>
#else
#include <Windows.h>
#include <Windowsx.h>
#endif

#ifdef DrawText
#undef DrawText
#endif

#endif
