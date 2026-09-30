#include <wchar.h>

#ifndef WCHAR_MAX
#error "wchar.h:WCHAR_MAX macro is missing from libc-shim"
#endif

#ifndef WCHAR_MIN
#error "wchar.h:WCHAR_MIN macro is missing from libc-shim"
#endif

#ifndef WEOF
#error "wchar.h:WEOF macro is missing from libc-shim"
#endif

int main(void) { return 0; }
