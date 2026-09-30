#include <string.h>

extern void * slate_oracle_memcpy(void *, const void *, __SIZE_TYPE__);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memcpy), __typeof__(memcpy)),
    "string.h:memcpy declaration differs from oracle");

static __typeof__(memcpy) *const slate_reference_memcpy = &memcpy;

extern char * slate_oracle_strtok_r(char *restrict, const char *restrict, char **restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtok_r), __typeof__(strtok_r)),
    "string.h:strtok_r declaration differs from oracle");

static __typeof__(strtok_r) *const slate_reference_strtok_r = &strtok_r;

#ifndef NULL
#error "string.h:NULL macro is missing from libc-shim"
#endif

#ifndef strdupa
#error "string.h:strdupa macro is missing from libc-shim"
#endif

int main(void) { return 0; }
