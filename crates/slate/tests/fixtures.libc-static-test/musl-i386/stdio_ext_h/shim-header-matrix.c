#include <stdio_ext.h>

#ifndef FSETLOCKING_BYCALLER
#error "stdio_ext.h:FSETLOCKING_BYCALLER macro is missing from libc-shim"
#endif

#ifndef FSETLOCKING_INTERNAL
#error "stdio_ext.h:FSETLOCKING_INTERNAL macro is missing from libc-shim"
#endif

#ifndef FSETLOCKING_QUERY
#error "stdio_ext.h:FSETLOCKING_QUERY macro is missing from libc-shim"
#endif

int main(void) { return 0; }
