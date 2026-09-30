#include <sys/user.h>

_Static_assert(sizeof(struct user_fpregs_struct) == 512, "struct user_fpregs_struct size differs from oracle");

_Static_assert(_Alignof(struct user_fpregs_struct) == 8, "struct user_fpregs_struct alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, cwd) == 0, "struct user_fpregs_struct.cwd offset differs from oracle");

typedef unsigned short slate_oracle_struct_user_fpregs_struct_cwd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->cwd), slate_oracle_struct_user_fpregs_struct_cwd), "struct user_fpregs_struct.cwd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, swd) == 2, "struct user_fpregs_struct.swd offset differs from oracle");

typedef unsigned short slate_oracle_struct_user_fpregs_struct_swd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->swd), slate_oracle_struct_user_fpregs_struct_swd), "struct user_fpregs_struct.swd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, ftw) == 4, "struct user_fpregs_struct.ftw offset differs from oracle");

typedef unsigned short slate_oracle_struct_user_fpregs_struct_ftw;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->ftw), slate_oracle_struct_user_fpregs_struct_ftw), "struct user_fpregs_struct.ftw field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, fop) == 6, "struct user_fpregs_struct.fop offset differs from oracle");

typedef unsigned short slate_oracle_struct_user_fpregs_struct_fop;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->fop), slate_oracle_struct_user_fpregs_struct_fop), "struct user_fpregs_struct.fop field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, rip) == 8, "struct user_fpregs_struct.rip offset differs from oracle");

typedef unsigned long slate_oracle_struct_user_fpregs_struct_rip;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->rip), slate_oracle_struct_user_fpregs_struct_rip), "struct user_fpregs_struct.rip field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, rdp) == 16, "struct user_fpregs_struct.rdp offset differs from oracle");

typedef unsigned long slate_oracle_struct_user_fpregs_struct_rdp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->rdp), slate_oracle_struct_user_fpregs_struct_rdp), "struct user_fpregs_struct.rdp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, mxcsr) == 24, "struct user_fpregs_struct.mxcsr offset differs from oracle");

typedef unsigned int slate_oracle_struct_user_fpregs_struct_mxcsr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->mxcsr), slate_oracle_struct_user_fpregs_struct_mxcsr), "struct user_fpregs_struct.mxcsr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, mxcr_mask) == 28, "struct user_fpregs_struct.mxcr_mask offset differs from oracle");

typedef unsigned int slate_oracle_struct_user_fpregs_struct_mxcr_mask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->mxcr_mask), slate_oracle_struct_user_fpregs_struct_mxcr_mask), "struct user_fpregs_struct.mxcr_mask field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, st_space) == 32, "struct user_fpregs_struct.st_space offset differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, xmm_space) == 160, "struct user_fpregs_struct.xmm_space offset differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, padding) == 416, "struct user_fpregs_struct.padding offset differs from oracle");

#ifndef ELF_NGREG
#error "sys/user.h:ELF_NGREG macro is missing from libc-shim"
#endif

#ifndef HOST_STACK_END_ADDR
#error "sys/user.h:HOST_STACK_END_ADDR macro is missing from libc-shim"
#endif

#ifndef HOST_TEXT_START_ADDR
#error "sys/user.h:HOST_TEXT_START_ADDR macro is missing from libc-shim"
#endif

#ifndef NBPG
#error "sys/user.h:NBPG macro is missing from libc-shim"
#endif

#ifndef PAGE_MASK
#error "sys/user.h:PAGE_MASK macro is missing from libc-shim"
#endif

#ifndef UPAGES
#error "sys/user.h:UPAGES macro is missing from libc-shim"
#endif

int main(void) { return 0; }
