#include <sys/gmon_out.h>

_Static_assert(sizeof(struct gmon_hdr) == 20, "struct gmon_hdr size differs from oracle");

_Static_assert(_Alignof(struct gmon_hdr) == 1, "struct gmon_hdr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct gmon_hdr, cookie) == 0, "struct gmon_hdr.cookie offset differs from oracle");

_Static_assert(__builtin_offsetof(struct gmon_hdr, version) == 4, "struct gmon_hdr.version offset differs from oracle");

_Static_assert(__builtin_offsetof(struct gmon_hdr, spare) == 8, "struct gmon_hdr.spare offset differs from oracle");

#ifndef GMON_MAGIC
#error "sys/gmon_out.h:GMON_MAGIC macro is missing from libc-shim"
#endif

#ifndef GMON_SHOBJ_VERSION
#error "sys/gmon_out.h:GMON_SHOBJ_VERSION macro is missing from libc-shim"
#endif

#ifndef GMON_VERSION
#error "sys/gmon_out.h:GMON_VERSION macro is missing from libc-shim"
#endif

int main(void) { return 0; }
