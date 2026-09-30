#include <sys/timeb.h>

extern void slate_oracle__ftime32(struct __timeb32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ftime32), __typeof__(_ftime32)),
    "sys/timeb.h:_ftime32 declaration differs from oracle");

static __typeof__(_ftime32) *const slate_reference__ftime32 = &_ftime32;

extern int slate_oracle__ftime32_s(struct __timeb32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ftime32_s), __typeof__(_ftime32_s)),
    "sys/timeb.h:_ftime32_s declaration differs from oracle");

static __typeof__(_ftime32_s) *const slate_reference__ftime32_s = &_ftime32_s;

extern void slate_oracle__ftime64(struct __timeb64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ftime64), __typeof__(_ftime64)),
    "sys/timeb.h:_ftime64 declaration differs from oracle");

static __typeof__(_ftime64) *const slate_reference__ftime64 = &_ftime64;

extern int slate_oracle__ftime64_s(struct __timeb64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ftime64_s), __typeof__(_ftime64_s)),
    "sys/timeb.h:_ftime64_s declaration differs from oracle");

static __typeof__(_ftime64_s) *const slate_reference__ftime64_s = &_ftime64_s;

_Static_assert(sizeof(struct timeb) == 16, "struct timeb size differs from oracle");

_Static_assert(_Alignof(struct timeb) == 8, "struct timeb alignment differs from oracle");

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

#ifndef _ftime
#error "sys/timeb.h:_ftime macro is missing from libc-shim"
#endif

#ifndef _ftime_s
#error "sys/timeb.h:_ftime_s macro is missing from libc-shim"
#endif

#ifndef _timeb
#error "sys/timeb.h:_timeb macro is missing from libc-shim"
#endif

int main(void) { return 0; }
