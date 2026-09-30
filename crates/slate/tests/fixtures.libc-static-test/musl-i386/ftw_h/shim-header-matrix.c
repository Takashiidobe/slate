#include <ftw.h>

_Static_assert(sizeof(struct FTW) == 8, "struct FTW size differs from oracle");

_Static_assert(_Alignof(struct FTW) == 4, "struct FTW alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct FTW, base) == 0, "struct FTW.base offset differs from oracle");

typedef int slate_oracle_struct_FTW_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct FTW *)0)->base), slate_oracle_struct_FTW_base), "struct FTW.base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct FTW, level) == 4, "struct FTW.level offset differs from oracle");

typedef int slate_oracle_struct_FTW_level;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct FTW *)0)->level), slate_oracle_struct_FTW_level), "struct FTW.level field type differs from oracle");

#ifndef FTW_CHDIR
#error "ftw.h:FTW_CHDIR macro is missing from libc-shim"
#endif

#ifndef FTW_D
#error "ftw.h:FTW_D macro is missing from libc-shim"
#endif

#ifndef FTW_DEPTH
#error "ftw.h:FTW_DEPTH macro is missing from libc-shim"
#endif

#ifndef FTW_DNR
#error "ftw.h:FTW_DNR macro is missing from libc-shim"
#endif

#ifndef FTW_DP
#error "ftw.h:FTW_DP macro is missing from libc-shim"
#endif

#ifndef FTW_F
#error "ftw.h:FTW_F macro is missing from libc-shim"
#endif

#ifndef FTW_MOUNT
#error "ftw.h:FTW_MOUNT macro is missing from libc-shim"
#endif

#ifndef FTW_NS
#error "ftw.h:FTW_NS macro is missing from libc-shim"
#endif

#ifndef FTW_PHYS
#error "ftw.h:FTW_PHYS macro is missing from libc-shim"
#endif

#ifndef FTW_SL
#error "ftw.h:FTW_SL macro is missing from libc-shim"
#endif

#ifndef FTW_SLN
#error "ftw.h:FTW_SLN macro is missing from libc-shim"
#endif

int main(void) { return 0; }
