#include <sys/user.h>

_Static_assert(sizeof(struct user_regs_struct) == 272, "struct user_regs_struct size differs from oracle");

_Static_assert(_Alignof(struct user_regs_struct) == 8, "struct user_regs_struct alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct user_regs_struct, regs) == 0, "struct user_regs_struct.regs offset differs from oracle");

_Static_assert(__builtin_offsetof(struct user_regs_struct, sp) == 248, "struct user_regs_struct.sp offset differs from oracle");

typedef unsigned long long slate_oracle_struct_user_regs_struct_sp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_regs_struct *)0)->sp), slate_oracle_struct_user_regs_struct_sp), "struct user_regs_struct.sp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_regs_struct, pc) == 256, "struct user_regs_struct.pc offset differs from oracle");

typedef unsigned long long slate_oracle_struct_user_regs_struct_pc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_regs_struct *)0)->pc), slate_oracle_struct_user_regs_struct_pc), "struct user_regs_struct.pc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_regs_struct, pstate) == 264, "struct user_regs_struct.pstate offset differs from oracle");

typedef unsigned long long slate_oracle_struct_user_regs_struct_pstate;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_regs_struct *)0)->pstate), slate_oracle_struct_user_regs_struct_pstate), "struct user_regs_struct.pstate field type differs from oracle");

#ifndef ELF_NREG
#error "sys/user.h:ELF_NREG macro is missing from libc-shim"
#endif

int main(void) { return 0; }
