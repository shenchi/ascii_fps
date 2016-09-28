#pragma once

// please include <Windows.h> before this

#ifdef _DEBUG
#define STRINGIFY(x) #x
#define FILE_AND_LINE(x) __FILE__ ":" STRINGIFY(x)
#define CHECK(x) if (!(x)) { int ret = GetLastError(); MessageBoxA(nullptr, FILE_AND_LINE(__LINE__), "Error", MB_OK); return ret; }
#else
#define CHECK(x) if (!(x)) { return GetLastError(); }
#endif
