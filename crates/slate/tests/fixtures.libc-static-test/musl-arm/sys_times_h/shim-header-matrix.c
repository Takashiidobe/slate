#include <sys/times.h>

_Static_assert(sizeof(struct tms) == 16, "struct tms size differs from oracle");

_Static_assert(_Alignof(struct tms) == 4, "struct tms alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct tms, tms_utime) == 0, "struct tms.tms_utime offset differs from oracle");

typedef long slate_oracle_struct_tms_tms_utime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tms *)0)->tms_utime), slate_oracle_struct_tms_tms_utime), "struct tms.tms_utime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tms, tms_stime) == 4, "struct tms.tms_stime offset differs from oracle");

typedef long slate_oracle_struct_tms_tms_stime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tms *)0)->tms_stime), slate_oracle_struct_tms_tms_stime), "struct tms.tms_stime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tms, tms_cutime) == 8, "struct tms.tms_cutime offset differs from oracle");

typedef long slate_oracle_struct_tms_tms_cutime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tms *)0)->tms_cutime), slate_oracle_struct_tms_tms_cutime), "struct tms.tms_cutime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tms, tms_cstime) == 12, "struct tms.tms_cstime offset differs from oracle");

typedef long slate_oracle_struct_tms_tms_cstime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tms *)0)->tms_cstime), slate_oracle_struct_tms_tms_cstime), "struct tms.tms_cstime field type differs from oracle");

int main(void) { return 0; }
