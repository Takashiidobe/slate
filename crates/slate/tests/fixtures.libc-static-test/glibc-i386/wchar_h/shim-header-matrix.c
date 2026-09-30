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

#ifndef wcschr
#error "wchar.h:wcschr macro is missing from libc-shim"
#endif

#ifndef wcspbrk
#error "wchar.h:wcspbrk macro is missing from libc-shim"
#endif

#ifndef wcsrchr
#error "wchar.h:wcsrchr macro is missing from libc-shim"
#endif

#ifndef wcsstr
#error "wchar.h:wcsstr macro is missing from libc-shim"
#endif

#ifndef wmemchr
#error "wchar.h:wmemchr macro is missing from libc-shim"
#endif

int main(void) { return 0; }
