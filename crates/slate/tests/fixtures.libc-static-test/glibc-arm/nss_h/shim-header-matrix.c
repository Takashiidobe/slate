#include <nss.h>

_Static_assert(NSS_STATUS_TRYAGAIN == (-2), "enum NSS_STATUS_TRYAGAIN value differs from oracle");

#ifndef NSS_DECLARE_MODULE_FUNCTIONS
#error "nss.h:NSS_DECLARE_MODULE_FUNCTIONS macro is missing from libc-shim"
#endif

int main(void) { return 0; }
