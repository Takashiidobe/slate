#include <shadow.h>

_Static_assert(sizeof(struct spwd) == 36, "struct spwd size differs from oracle");

_Static_assert(_Alignof(struct spwd) == 4, "struct spwd alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_namp) == 0, "struct spwd.sp_namp offset differs from oracle");

typedef char * slate_oracle_struct_spwd_sp_namp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_namp), slate_oracle_struct_spwd_sp_namp), "struct spwd.sp_namp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_pwdp) == 4, "struct spwd.sp_pwdp offset differs from oracle");

typedef char * slate_oracle_struct_spwd_sp_pwdp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_pwdp), slate_oracle_struct_spwd_sp_pwdp), "struct spwd.sp_pwdp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_lstchg) == 8, "struct spwd.sp_lstchg offset differs from oracle");

typedef long slate_oracle_struct_spwd_sp_lstchg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_lstchg), slate_oracle_struct_spwd_sp_lstchg), "struct spwd.sp_lstchg field type differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_min) == 12, "struct spwd.sp_min offset differs from oracle");

typedef long slate_oracle_struct_spwd_sp_min;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_min), slate_oracle_struct_spwd_sp_min), "struct spwd.sp_min field type differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_max) == 16, "struct spwd.sp_max offset differs from oracle");

typedef long slate_oracle_struct_spwd_sp_max;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_max), slate_oracle_struct_spwd_sp_max), "struct spwd.sp_max field type differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_warn) == 20, "struct spwd.sp_warn offset differs from oracle");

typedef long slate_oracle_struct_spwd_sp_warn;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_warn), slate_oracle_struct_spwd_sp_warn), "struct spwd.sp_warn field type differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_inact) == 24, "struct spwd.sp_inact offset differs from oracle");

typedef long slate_oracle_struct_spwd_sp_inact;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_inact), slate_oracle_struct_spwd_sp_inact), "struct spwd.sp_inact field type differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_expire) == 28, "struct spwd.sp_expire offset differs from oracle");

typedef long slate_oracle_struct_spwd_sp_expire;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_expire), slate_oracle_struct_spwd_sp_expire), "struct spwd.sp_expire field type differs from oracle");

_Static_assert(__builtin_offsetof(struct spwd, sp_flag) == 32, "struct spwd.sp_flag offset differs from oracle");

typedef unsigned long slate_oracle_struct_spwd_sp_flag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct spwd *)0)->sp_flag), slate_oracle_struct_spwd_sp_flag), "struct spwd.sp_flag field type differs from oracle");

#ifndef SHADOW
#error "shadow.h:SHADOW macro is missing from libc-shim"
#endif

int main(void) { return 0; }
