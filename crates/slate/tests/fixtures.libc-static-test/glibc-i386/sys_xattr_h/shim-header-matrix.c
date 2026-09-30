#include <sys/xattr.h>

#ifndef XATTR_CREATE
#error "sys/xattr.h:XATTR_CREATE macro is missing from libc-shim"
#endif

#ifndef XATTR_REPLACE
#error "sys/xattr.h:XATTR_REPLACE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
