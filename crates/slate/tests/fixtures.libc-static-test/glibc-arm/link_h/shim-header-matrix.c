#include <link.h>

_Static_assert(sizeof(struct La_arm_regs) == 192, "struct La_arm_regs size differs from oracle");

_Static_assert(_Alignof(struct La_arm_regs) == 4, "struct La_arm_regs alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct La_arm_regs, lr_reg) == 0, "struct La_arm_regs.lr_reg offset differs from oracle");

_Static_assert(__builtin_offsetof(struct La_arm_regs, lr_sp) == 16, "struct La_arm_regs.lr_sp offset differs from oracle");

typedef unsigned int slate_oracle_struct_La_arm_regs_lr_sp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct La_arm_regs *)0)->lr_sp), slate_oracle_struct_La_arm_regs_lr_sp), "struct La_arm_regs.lr_sp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct La_arm_regs, lr_lr) == 20, "struct La_arm_regs.lr_lr offset differs from oracle");

typedef unsigned int slate_oracle_struct_La_arm_regs_lr_lr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct La_arm_regs *)0)->lr_lr), slate_oracle_struct_La_arm_regs_lr_lr), "struct La_arm_regs.lr_lr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct La_arm_regs, lr_coproc) == 24, "struct La_arm_regs.lr_coproc offset differs from oracle");

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
