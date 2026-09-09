#include <ar.h>

_Static_assert(sizeof(struct ar_hdr) == 60, "struct ar_hdr size differs from oracle");

_Static_assert(_Alignof(struct ar_hdr) == 1, "struct ar_hdr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct ar_hdr, ar_name) == 0, "struct ar_hdr.ar_name offset differs from oracle");

_Static_assert(__builtin_offsetof(struct ar_hdr, ar_date) == 16, "struct ar_hdr.ar_date offset differs from oracle");

_Static_assert(__builtin_offsetof(struct ar_hdr, ar_uid) == 28, "struct ar_hdr.ar_uid offset differs from oracle");

_Static_assert(__builtin_offsetof(struct ar_hdr, ar_gid) == 34, "struct ar_hdr.ar_gid offset differs from oracle");

_Static_assert(__builtin_offsetof(struct ar_hdr, ar_mode) == 40, "struct ar_hdr.ar_mode offset differs from oracle");

_Static_assert(__builtin_offsetof(struct ar_hdr, ar_size) == 48, "struct ar_hdr.ar_size offset differs from oracle");

_Static_assert(__builtin_offsetof(struct ar_hdr, ar_fmag) == 58, "struct ar_hdr.ar_fmag offset differs from oracle");

#ifndef ARFMAG
#error "ar.h:ARFMAG macro is missing from libc-shim"
#endif

#ifndef ARMAG
#error "ar.h:ARMAG macro is missing from libc-shim"
#endif

#ifndef SARMAG
#error "ar.h:SARMAG macro is missing from libc-shim"
#endif

int main(void) { return 0; }
