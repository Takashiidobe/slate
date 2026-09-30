#include <utime.h>

_Static_assert(sizeof(struct utimbuf) == 16, "struct utimbuf size differs from oracle");

_Static_assert(_Alignof(struct utimbuf) == 4, "struct utimbuf alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct utimbuf, actime) == 0, "struct utimbuf.actime offset differs from oracle");

typedef long long slate_oracle_struct_utimbuf_actime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utimbuf *)0)->actime), slate_oracle_struct_utimbuf_actime), "struct utimbuf.actime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct utimbuf, modtime) == 8, "struct utimbuf.modtime offset differs from oracle");

typedef long long slate_oracle_struct_utimbuf_modtime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utimbuf *)0)->modtime), slate_oracle_struct_utimbuf_modtime), "struct utimbuf.modtime field type differs from oracle");

int main(void) { return 0; }
