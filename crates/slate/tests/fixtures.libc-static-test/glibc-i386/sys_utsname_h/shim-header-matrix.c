#include <sys/utsname.h>

_Static_assert(sizeof(struct utsname) == 390, "struct utsname size differs from oracle");

_Static_assert(_Alignof(struct utsname) == 1, "struct utsname alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct utsname, sysname) == 0, "struct utsname.sysname offset differs from oracle");

_Static_assert(__builtin_offsetof(struct utsname, nodename) == 65, "struct utsname.nodename offset differs from oracle");

_Static_assert(__builtin_offsetof(struct utsname, release) == 130, "struct utsname.release offset differs from oracle");

_Static_assert(__builtin_offsetof(struct utsname, version) == 195, "struct utsname.version offset differs from oracle");

_Static_assert(__builtin_offsetof(struct utsname, machine) == 260, "struct utsname.machine offset differs from oracle");

_Static_assert(__builtin_offsetof(struct utsname, domainname) == 325, "struct utsname.domainname offset differs from oracle");

#ifndef SYS_NMLN
#error "sys/utsname.h:SYS_NMLN macro is missing from libc-shim"
#endif

int main(void) { return 0; }
