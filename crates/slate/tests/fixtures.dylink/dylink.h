#ifndef DYLINK_H
#define DYLINK_H

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#include <windows.h>
#undef small
#define dl_open(path) LoadLibraryA(path)
#define dl_sym(handle, name) GetProcAddress(handle, name)
#define dl_close(handle) FreeLibrary(handle)
#else
#include <dlfcn.h>
#define EXPORT __attribute__((visibility("default")))
#define dl_open(path) dlopen(path, RTLD_NOW | RTLD_LOCAL)
#define dl_sym(handle, name) dlsym(handle, name)
#define dl_close(handle) (dlclose(handle) == 0)
#endif

#endif
