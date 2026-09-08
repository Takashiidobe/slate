#include <assert.h>

extern void slate_oracle___assert(const char *, const char *, int) __attribute__((noreturn));
extern void slate_oracle___assert_fail(const char *, const char *, unsigned int, const char *) __attribute__((noreturn));
extern void slate_oracle___assert_perror_fail(int, const char *, unsigned int, const char *) __attribute__((noreturn));
extern bool slate_oracle___assert_single_arg(bool);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___assert), __typeof__(__assert)),
    "assert.h:__assert declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___assert_fail), __typeof__(__assert_fail)),
    "assert.h:__assert_fail declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___assert_perror_fail), __typeof__(__assert_perror_fail)),
    "assert.h:__assert_perror_fail declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___assert_single_arg), __typeof__(__assert_single_arg)),
    "assert.h:__assert_single_arg declaration differs from oracle");

static __typeof__(__assert) *const slate_reference___assert = &__assert;
static __typeof__(__assert_fail) *const slate_reference___assert_fail = &__assert_fail;
static __typeof__(__assert_perror_fail) *const slate_reference___assert_perror_fail = &__assert_perror_fail;
static __typeof__(__assert_single_arg) *const slate_reference___assert_single_arg = &__assert_single_arg;

#ifndef _ASSERT_H
#error "assert.h:_ASSERT_H macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "assert.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "assert.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "assert.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "assert.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "assert.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "assert.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "assert.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "assert.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "assert.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "assert.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "assert.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "assert.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "assert.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "assert.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "assert.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "assert.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "assert.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

#ifndef assert
#error "assert.h:assert macro is missing from libc-shim"
#endif

#ifndef assert_perror
#error "assert.h:assert_perror macro is missing from libc-shim"
#endif

#ifndef static_assert
#error "assert.h:static_assert macro is missing from libc-shim"
#endif

int main(void) { return 0; }
