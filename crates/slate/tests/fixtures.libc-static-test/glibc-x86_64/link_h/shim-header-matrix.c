#include <link.h>

_Static_assert(__builtin_offsetof(struct r_debug, r_version) == 0, "struct r_debug.r_version offset differs from oracle");

typedef int slate_oracle_struct_r_debug_r_version;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct r_debug *)0)->r_version), slate_oracle_struct_r_debug_r_version), "struct r_debug.r_version field type differs from oracle");

typedef struct link_map * slate_oracle_struct_r_debug_r_map;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct r_debug *)0)->r_map), slate_oracle_struct_r_debug_r_map), "struct r_debug.r_map field type differs from oracle");

typedef unsigned long slate_oracle_struct_r_debug_r_brk;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct r_debug *)0)->r_brk), slate_oracle_struct_r_debug_r_brk), "struct r_debug.r_brk field type differs from oracle");

typedef unsigned long slate_oracle_struct_r_debug_r_ldbase;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct r_debug *)0)->r_ldbase), slate_oracle_struct_r_debug_r_ldbase), "struct r_debug.r_ldbase field type differs from oracle");

#ifndef ElfW
#error "link.h:ElfW macro is missing from libc-shim"
#endif

#ifndef LAV_CURRENT
#error "link.h:LAV_CURRENT macro is missing from libc-shim"
#endif

#ifndef La_x32_regs
#error "link.h:La_x32_regs macro is missing from libc-shim"
#endif

#ifndef La_x32_retval
#error "link.h:La_x32_retval macro is missing from libc-shim"
#endif

int main(void) { return 0; }
