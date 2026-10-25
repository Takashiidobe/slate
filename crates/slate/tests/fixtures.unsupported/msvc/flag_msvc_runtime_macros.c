/* { dg-options "/Zc:preprocessor /MTd /RTC1 /W4 /volatile:iso /fpcvt:IA /favor:ATOM /sdl" } */
void abort(void);

int main(void) {
#if !defined(_DEBUG) || defined(_DLL) || defined(_MSVC_MT)
  abort();
#endif
#if !defined(__MSVC_RUNTIME_CHECKS) || _MSVC_WARNING_LEVEL != 4
  abort();
#endif
#if !defined(_ISO_VOLATILE) || __FPCVT_IA__ != 1 || !defined(__ATOM__)
  abort();
#endif
#ifndef _GUARDOVERFLOW_CRT_ALLOCATORS
  abort();
#endif
  return 0;
}
