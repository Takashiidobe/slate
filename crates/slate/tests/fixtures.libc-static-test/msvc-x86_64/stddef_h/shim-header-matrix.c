#include <stddef.h>

extern unsigned long long slate_oracle___threadhandle(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___threadhandle), __typeof__(__threadhandle)),
    "stddef.h:__threadhandle declaration differs from oracle");

static __typeof__(__threadhandle) *const slate_reference___threadhandle = &__threadhandle;

extern unsigned long slate_oracle___threadid(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___threadid), __typeof__(__threadid)),
    "stddef.h:__threadid declaration differs from oracle");

static __typeof__(__threadid) *const slate_reference___threadid = &__threadid;

extern int * slate_oracle__errno(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__errno), __typeof__(_errno)),
    "stddef.h:_errno declaration differs from oracle");

static __typeof__(_errno) *const slate_reference__errno = &_errno;

extern int slate_oracle__get_errno(int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_errno), __typeof__(_get_errno)),
    "stddef.h:_get_errno declaration differs from oracle");

static __typeof__(_get_errno) *const slate_reference__get_errno = &_get_errno;

extern int slate_oracle__set_errno(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_errno), __typeof__(_set_errno)),
    "stddef.h:_set_errno declaration differs from oracle");

static __typeof__(_set_errno) *const slate_reference__set_errno = &_set_errno;

#ifndef _INC_STDDEF
#error "stddef.h:_INC_STDDEF macro is missing from libc-shim"
#endif

#ifndef _threadid
#error "stddef.h:_threadid macro is missing from libc-shim"
#endif

#ifndef errno
#error "stddef.h:errno macro is missing from libc-shim"
#endif

#ifndef offsetof
#error "stddef.h:offsetof macro is missing from libc-shim"
#endif

int main(void) { return 0; }
