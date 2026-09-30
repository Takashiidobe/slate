#include <sys/sysmacros.h>

#ifndef major
#error "sys/sysmacros.h:major macro is missing from libc-shim"
#endif

#ifndef makedev
#error "sys/sysmacros.h:makedev macro is missing from libc-shim"
#endif

#ifndef minor
#error "sys/sysmacros.h:minor macro is missing from libc-shim"
#endif

int main(void) { return 0; }
