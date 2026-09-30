#include <sys/timeb.h>

_Static_assert(sizeof(struct timeb) == 16, "struct timeb size differs from oracle");

_Static_assert(_Alignof(struct timeb) == 4, "struct timeb alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timeb, time) == 0, "struct timeb.time offset differs from oracle");

typedef long long slate_oracle_struct_timeb_time;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeb *)0)->time), slate_oracle_struct_timeb_time), "struct timeb.time field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timeb, millitm) == 8, "struct timeb.millitm offset differs from oracle");

typedef unsigned short slate_oracle_struct_timeb_millitm;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeb *)0)->millitm), slate_oracle_struct_timeb_millitm), "struct timeb.millitm field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timeb, timezone) == 10, "struct timeb.timezone offset differs from oracle");

typedef short slate_oracle_struct_timeb_timezone;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeb *)0)->timezone), slate_oracle_struct_timeb_timezone), "struct timeb.timezone field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timeb, dstflag) == 12, "struct timeb.dstflag offset differs from oracle");

typedef short slate_oracle_struct_timeb_dstflag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeb *)0)->dstflag), slate_oracle_struct_timeb_dstflag), "struct timeb.dstflag field type differs from oracle");

int main(void) { return 0; }
