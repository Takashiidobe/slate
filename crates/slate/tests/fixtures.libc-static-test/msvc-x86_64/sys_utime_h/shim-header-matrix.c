#include <sys/utime.h>

extern int slate_oracle__futime32(int, struct __utimbuf32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__futime32), __typeof__(_futime32)),
    "sys/utime.h:_futime32 declaration differs from oracle");

static __typeof__(_futime32) *const slate_reference__futime32 = &_futime32;

extern int slate_oracle__futime64(int, struct __utimbuf64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__futime64), __typeof__(_futime64)),
    "sys/utime.h:_futime64 declaration differs from oracle");

static __typeof__(_futime64) *const slate_reference__futime64 = &_futime64;

extern int slate_oracle__utime32(const char *, struct __utimbuf32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__utime32), __typeof__(_utime32)),
    "sys/utime.h:_utime32 declaration differs from oracle");

static __typeof__(_utime32) *const slate_reference__utime32 = &_utime32;

extern int slate_oracle__utime64(const char *, struct __utimbuf64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__utime64), __typeof__(_utime64)),
    "sys/utime.h:_utime64 declaration differs from oracle");

static __typeof__(_utime64) *const slate_reference__utime64 = &_utime64;

extern int slate_oracle__wutime32(const unsigned short *, struct __utimbuf32 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wutime32), __typeof__(_wutime32)),
    "sys/utime.h:_wutime32 declaration differs from oracle");

static __typeof__(_wutime32) *const slate_reference__wutime32 = &_wutime32;

extern int slate_oracle__wutime64(const unsigned short *, struct __utimbuf64 *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wutime64), __typeof__(_wutime64)),
    "sys/utime.h:_wutime64 declaration differs from oracle");

static __typeof__(_wutime64) *const slate_reference__wutime64 = &_wutime64;

_Static_assert(sizeof(struct utimbuf) == 16, "struct utimbuf size differs from oracle");

_Static_assert(_Alignof(struct utimbuf) == 8, "struct utimbuf alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct utimbuf, actime) == 0, "struct utimbuf.actime offset differs from oracle");

typedef long long slate_oracle_struct_utimbuf_actime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utimbuf *)0)->actime), slate_oracle_struct_utimbuf_actime), "struct utimbuf.actime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct utimbuf, modtime) == 8, "struct utimbuf.modtime offset differs from oracle");

typedef long long slate_oracle_struct_utimbuf_modtime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utimbuf *)0)->modtime), slate_oracle_struct_utimbuf_modtime), "struct utimbuf.modtime field type differs from oracle");

_Static_assert(sizeof(struct utimbuf32) == 8, "struct utimbuf32 size differs from oracle");

_Static_assert(_Alignof(struct utimbuf32) == 4, "struct utimbuf32 alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct utimbuf32, actime) == 0, "struct utimbuf32.actime offset differs from oracle");

typedef long slate_oracle_struct_utimbuf32_actime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utimbuf32 *)0)->actime), slate_oracle_struct_utimbuf32_actime), "struct utimbuf32.actime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct utimbuf32, modtime) == 4, "struct utimbuf32.modtime offset differs from oracle");

typedef long slate_oracle_struct_utimbuf32_modtime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utimbuf32 *)0)->modtime), slate_oracle_struct_utimbuf32_modtime), "struct utimbuf32.modtime field type differs from oracle");

int main(void) { return 0; }
