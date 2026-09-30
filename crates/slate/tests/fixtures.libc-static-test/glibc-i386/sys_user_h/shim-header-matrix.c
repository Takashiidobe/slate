#include <sys/user.h>

_Static_assert(sizeof(struct user_fpregs_struct) == 108, "struct user_fpregs_struct size differs from oracle");

_Static_assert(_Alignof(struct user_fpregs_struct) == 4, "struct user_fpregs_struct alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, cwd) == 0, "struct user_fpregs_struct.cwd offset differs from oracle");

typedef long slate_oracle_struct_user_fpregs_struct_cwd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->cwd), slate_oracle_struct_user_fpregs_struct_cwd), "struct user_fpregs_struct.cwd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, swd) == 4, "struct user_fpregs_struct.swd offset differs from oracle");

typedef long slate_oracle_struct_user_fpregs_struct_swd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->swd), slate_oracle_struct_user_fpregs_struct_swd), "struct user_fpregs_struct.swd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, twd) == 8, "struct user_fpregs_struct.twd offset differs from oracle");

typedef long slate_oracle_struct_user_fpregs_struct_twd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->twd), slate_oracle_struct_user_fpregs_struct_twd), "struct user_fpregs_struct.twd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, fip) == 12, "struct user_fpregs_struct.fip offset differs from oracle");

typedef long slate_oracle_struct_user_fpregs_struct_fip;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->fip), slate_oracle_struct_user_fpregs_struct_fip), "struct user_fpregs_struct.fip field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, fcs) == 16, "struct user_fpregs_struct.fcs offset differs from oracle");

typedef long slate_oracle_struct_user_fpregs_struct_fcs;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->fcs), slate_oracle_struct_user_fpregs_struct_fcs), "struct user_fpregs_struct.fcs field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, foo) == 20, "struct user_fpregs_struct.foo offset differs from oracle");

typedef long slate_oracle_struct_user_fpregs_struct_foo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->foo), slate_oracle_struct_user_fpregs_struct_foo), "struct user_fpregs_struct.foo field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, fos) == 24, "struct user_fpregs_struct.fos offset differs from oracle");

typedef long slate_oracle_struct_user_fpregs_struct_fos;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs_struct *)0)->fos), slate_oracle_struct_user_fpregs_struct_fos), "struct user_fpregs_struct.fos field type differs from oracle");

_Static_assert(__builtin_offsetof(struct user_fpregs_struct, st_space) == 28, "struct user_fpregs_struct.st_space offset differs from oracle");

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

#ifndef PAGE_SHIFT
#error "sys/user.h:PAGE_SHIFT macro is missing from libc-shim"
#endif

#ifndef PAGE_SIZE
#error "sys/user.h:PAGE_SIZE macro is missing from libc-shim"
#endif

#ifndef UPAGES
#error "sys/user.h:UPAGES macro is missing from libc-shim"
#endif

int main(void) { return 0; }
