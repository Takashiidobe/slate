#include <byteswap.h>

#ifndef bswap_16
#error "byteswap.h:bswap_16 macro is missing from libc-shim"
#endif

#ifndef bswap_32
#error "byteswap.h:bswap_32 macro is missing from libc-shim"
#endif

#ifndef bswap_64
#error "byteswap.h:bswap_64 macro is missing from libc-shim"
#endif

int main(void) { return 0; }
