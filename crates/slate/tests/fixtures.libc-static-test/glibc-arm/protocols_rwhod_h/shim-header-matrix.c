#include <protocols/rwhod.h>

_Static_assert(sizeof(struct outmp) == 20, "struct outmp size differs from oracle");

_Static_assert(_Alignof(struct outmp) == 4, "struct outmp alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct outmp, out_line) == 0, "struct outmp.out_line offset differs from oracle");

_Static_assert(__builtin_offsetof(struct outmp, out_name) == 8, "struct outmp.out_name offset differs from oracle");

_Static_assert(__builtin_offsetof(struct outmp, out_time) == 16, "struct outmp.out_time offset differs from oracle");

typedef int slate_oracle_struct_outmp_out_time;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct outmp *)0)->out_time), slate_oracle_struct_outmp_out_time), "struct outmp.out_time field type differs from oracle");

#ifndef WHODTYPE_STATUS
#error "protocols/rwhod.h:WHODTYPE_STATUS macro is missing from libc-shim"
#endif

#ifndef WHODVERSION
#error "protocols/rwhod.h:WHODVERSION macro is missing from libc-shim"
#endif

int main(void) { return 0; }
