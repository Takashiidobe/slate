#include <link.h>

_Static_assert(sizeof(struct La_i86_regs) == 20, "struct La_i86_regs size differs from oracle");

_Static_assert(_Alignof(struct La_i86_regs) == 4, "struct La_i86_regs alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct La_i86_regs, lr_edx) == 0, "struct La_i86_regs.lr_edx offset differs from oracle");

typedef unsigned int slate_oracle_struct_La_i86_regs_lr_edx;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct La_i86_regs *)0)->lr_edx), slate_oracle_struct_La_i86_regs_lr_edx), "struct La_i86_regs.lr_edx field type differs from oracle");

_Static_assert(__builtin_offsetof(struct La_i86_regs, lr_ecx) == 4, "struct La_i86_regs.lr_ecx offset differs from oracle");

typedef unsigned int slate_oracle_struct_La_i86_regs_lr_ecx;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct La_i86_regs *)0)->lr_ecx), slate_oracle_struct_La_i86_regs_lr_ecx), "struct La_i86_regs.lr_ecx field type differs from oracle");

_Static_assert(__builtin_offsetof(struct La_i86_regs, lr_eax) == 8, "struct La_i86_regs.lr_eax offset differs from oracle");

typedef unsigned int slate_oracle_struct_La_i86_regs_lr_eax;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct La_i86_regs *)0)->lr_eax), slate_oracle_struct_La_i86_regs_lr_eax), "struct La_i86_regs.lr_eax field type differs from oracle");

_Static_assert(__builtin_offsetof(struct La_i86_regs, lr_ebp) == 12, "struct La_i86_regs.lr_ebp offset differs from oracle");

typedef unsigned int slate_oracle_struct_La_i86_regs_lr_ebp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct La_i86_regs *)0)->lr_ebp), slate_oracle_struct_La_i86_regs_lr_ebp), "struct La_i86_regs.lr_ebp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct La_i86_regs, lr_esp) == 16, "struct La_i86_regs.lr_esp offset differs from oracle");

typedef unsigned int slate_oracle_struct_La_i86_regs_lr_esp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct La_i86_regs *)0)->lr_esp), slate_oracle_struct_La_i86_regs_lr_esp), "struct La_i86_regs.lr_esp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct r_debug, r_version) == 0, "struct r_debug.r_version offset differs from oracle");

typedef int slate_oracle_struct_r_debug_r_version;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct r_debug *)0)->r_version), slate_oracle_struct_r_debug_r_version), "struct r_debug.r_version field type differs from oracle");

typedef struct link_map * slate_oracle_struct_r_debug_r_map;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct r_debug *)0)->r_map), slate_oracle_struct_r_debug_r_map), "struct r_debug.r_map field type differs from oracle");

typedef unsigned int slate_oracle_struct_r_debug_r_brk;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct r_debug *)0)->r_brk), slate_oracle_struct_r_debug_r_brk), "struct r_debug.r_brk field type differs from oracle");

typedef unsigned int slate_oracle_struct_r_debug_r_ldbase;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct r_debug *)0)->r_ldbase), slate_oracle_struct_r_debug_r_ldbase), "struct r_debug.r_ldbase field type differs from oracle");

#ifndef ElfW
#error "link.h:ElfW macro is missing from libc-shim"
#endif

#ifndef LAV_CURRENT
#error "link.h:LAV_CURRENT macro is missing from libc-shim"
#endif

int main(void) { return 0; }
