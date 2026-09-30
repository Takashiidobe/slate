#include <assert.h>

#ifndef assert
#error "assert.h:assert macro is missing from libc-shim"
#endif

#ifndef static_assert
#error "assert.h:static_assert macro is missing from libc-shim"
#endif

int main(void) { return 0; }
