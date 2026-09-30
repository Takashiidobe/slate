#include <string.h>

extern void slate_oracle_explicit_bzero(void *, unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_explicit_bzero), __typeof__(explicit_bzero)),
    "string.h:explicit_bzero declaration differs from oracle");

static __typeof__(explicit_bzero) *const slate_reference_explicit_bzero = &explicit_bzero;

extern void * slate_oracle_memcpy(void *, const void *, __SIZE_TYPE__);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memcpy), __typeof__(memcpy)),
    "string.h:memcpy declaration differs from oracle");

static __typeof__(memcpy) *const slate_reference_memcpy = &memcpy;

extern int slate_oracle_strcoll_l(const char *, const char *, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcoll_l), __typeof__(strcoll_l)),
    "string.h:strcoll_l declaration differs from oracle");

static __typeof__(strcoll_l) *const slate_reference_strcoll_l = &strcoll_l;

#ifndef strdupa
#error "string.h:strdupa macro is missing from libc-shim"
#endif

#ifndef strndupa
#error "string.h:strndupa macro is missing from libc-shim"
#endif

int main(void) { return 0; }
