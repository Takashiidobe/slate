#include <netatalk/at.h>

#ifndef SOL_ATALK
#error "netatalk/at.h:SOL_ATALK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
