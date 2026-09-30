#include <printf.h>

_Static_assert(sizeof(struct printf_info) == 20, "struct printf_info size differs from oracle");

_Static_assert(_Alignof(struct printf_info) == 4, "struct printf_info alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct printf_info, prec) == 0, "struct printf_info.prec offset differs from oracle");

typedef int slate_oracle_struct_printf_info_prec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct printf_info *)0)->prec), slate_oracle_struct_printf_info_prec), "struct printf_info.prec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct printf_info, width) == 4, "struct printf_info.width offset differs from oracle");

typedef int slate_oracle_struct_printf_info_width;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct printf_info *)0)->width), slate_oracle_struct_printf_info_width), "struct printf_info.width field type differs from oracle");

_Static_assert(__builtin_offsetof(struct printf_info, spec) == 8, "struct printf_info.spec offset differs from oracle");

typedef int slate_oracle_struct_printf_info_spec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct printf_info *)0)->spec), slate_oracle_struct_printf_info_spec), "struct printf_info.spec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct printf_info, user) == 14, "struct printf_info.user offset differs from oracle");

typedef unsigned short slate_oracle_struct_printf_info_user;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct printf_info *)0)->user), slate_oracle_struct_printf_info_user), "struct printf_info.user field type differs from oracle");

_Static_assert(__builtin_offsetof(struct printf_info, pad) == 16, "struct printf_info.pad offset differs from oracle");

typedef int slate_oracle_struct_printf_info_pad;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct printf_info *)0)->pad), slate_oracle_struct_printf_info_pad), "struct printf_info.pad field type differs from oracle");

#ifndef PA_FLAG_LONG
#error "printf.h:PA_FLAG_LONG macro is missing from libc-shim"
#endif

#ifndef PA_FLAG_LONG_DOUBLE
#error "printf.h:PA_FLAG_LONG_DOUBLE macro is missing from libc-shim"
#endif

#ifndef PA_FLAG_LONG_LONG
#error "printf.h:PA_FLAG_LONG_LONG macro is missing from libc-shim"
#endif

#ifndef PA_FLAG_MASK
#error "printf.h:PA_FLAG_MASK macro is missing from libc-shim"
#endif

#ifndef PA_FLAG_PTR
#error "printf.h:PA_FLAG_PTR macro is missing from libc-shim"
#endif

#ifndef PA_FLAG_SHORT
#error "printf.h:PA_FLAG_SHORT macro is missing from libc-shim"
#endif

int main(void) { return 0; }
