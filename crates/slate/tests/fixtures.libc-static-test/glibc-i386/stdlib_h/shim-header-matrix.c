#include <stdlib.h>

extern void slate_oracle_call_once(struct __once_flag *, void (*)(void));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_call_once), __typeof__(call_once)),
    "stdlib.h:call_once declaration differs from oracle");

static __typeof__(call_once) *const slate_reference_call_once = &call_once;

extern long slate_oracle_random(void);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_random), __typeof__(random)),
    "stdlib.h:random declaration differs from oracle");

static __typeof__(random) *const slate_reference_random = &random;

extern long slate_oracle_strtol_l(const char *restrict, char **restrict, int, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtol_l), __typeof__(strtol_l)),
    "stdlib.h:strtol_l declaration differs from oracle");

static __typeof__(strtol_l) *const slate_reference_strtol_l = &strtol_l;

extern void * slate_oracle_valloc(unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_valloc), __typeof__(valloc)),
    "stdlib.h:valloc declaration differs from oracle");

static __typeof__(valloc) *const slate_reference_valloc = &valloc;

#ifndef EXIT_FAILURE
#error "stdlib.h:EXIT_FAILURE macro is missing from libc-shim"
#endif

#ifndef EXIT_SUCCESS
#error "stdlib.h:EXIT_SUCCESS macro is missing from libc-shim"
#endif

#ifndef MB_CUR_MAX
#error "stdlib.h:MB_CUR_MAX macro is missing from libc-shim"
#endif

#ifndef RAND_MAX
#error "stdlib.h:RAND_MAX macro is missing from libc-shim"
#endif

#ifndef WEXITSTATUS
#error "stdlib.h:WEXITSTATUS macro is missing from libc-shim"
#endif

#ifndef WIFCONTINUED
#error "stdlib.h:WIFCONTINUED macro is missing from libc-shim"
#endif

#ifndef WIFEXITED
#error "stdlib.h:WIFEXITED macro is missing from libc-shim"
#endif

#ifndef WIFSIGNALED
#error "stdlib.h:WIFSIGNALED macro is missing from libc-shim"
#endif

#ifndef WIFSTOPPED
#error "stdlib.h:WIFSTOPPED macro is missing from libc-shim"
#endif

#ifndef WSTOPSIG
#error "stdlib.h:WSTOPSIG macro is missing from libc-shim"
#endif

#ifndef WTERMSIG
#error "stdlib.h:WTERMSIG macro is missing from libc-shim"
#endif

#ifndef bsearch
#error "stdlib.h:bsearch macro is missing from libc-shim"
#endif

int main(void) { return 0; }
