#include <errno.h>

extern unsigned long * slate_oracle___doserrno(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___doserrno), __typeof__(__doserrno)),
    "errno.h:__doserrno declaration differs from oracle");

static __typeof__(__doserrno) *const slate_reference___doserrno = &__doserrno;

extern int * slate_oracle__errno(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__errno), __typeof__(_errno)),
    "errno.h:_errno declaration differs from oracle");

static __typeof__(_errno) *const slate_reference__errno = &_errno;

extern int slate_oracle__get_doserrno(unsigned long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_doserrno), __typeof__(_get_doserrno)),
    "errno.h:_get_doserrno declaration differs from oracle");

static __typeof__(_get_doserrno) *const slate_reference__get_doserrno = &_get_doserrno;

extern int slate_oracle__get_errno(int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_errno), __typeof__(_get_errno)),
    "errno.h:_get_errno declaration differs from oracle");

static __typeof__(_get_errno) *const slate_reference__get_errno = &_get_errno;

extern int slate_oracle__set_doserrno(unsigned long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_doserrno), __typeof__(_set_doserrno)),
    "errno.h:_set_doserrno declaration differs from oracle");

static __typeof__(_set_doserrno) *const slate_reference__set_doserrno = &_set_doserrno;

extern int slate_oracle__set_errno(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_errno), __typeof__(_set_errno)),
    "errno.h:_set_errno declaration differs from oracle");

static __typeof__(_set_errno) *const slate_reference__set_errno = &_set_errno;

#ifndef E2BIG
#error "errno.h:E2BIG macro is missing from libc-shim"
#endif

#ifndef EACCES
#error "errno.h:EACCES macro is missing from libc-shim"
#endif

#ifndef EADDRINUSE
#error "errno.h:EADDRINUSE macro is missing from libc-shim"
#endif

#ifndef EADDRNOTAVAIL
#error "errno.h:EADDRNOTAVAIL macro is missing from libc-shim"
#endif

#ifndef EAFNOSUPPORT
#error "errno.h:EAFNOSUPPORT macro is missing from libc-shim"
#endif

#ifndef EAGAIN
#error "errno.h:EAGAIN macro is missing from libc-shim"
#endif

#ifndef EALREADY
#error "errno.h:EALREADY macro is missing from libc-shim"
#endif

#ifndef EBADF
#error "errno.h:EBADF macro is missing from libc-shim"
#endif

#ifndef EBADMSG
#error "errno.h:EBADMSG macro is missing from libc-shim"
#endif

#ifndef EBUSY
#error "errno.h:EBUSY macro is missing from libc-shim"
#endif

#ifndef ECANCELED
#error "errno.h:ECANCELED macro is missing from libc-shim"
#endif

#ifndef ECHILD
#error "errno.h:ECHILD macro is missing from libc-shim"
#endif

#ifndef ECONNABORTED
#error "errno.h:ECONNABORTED macro is missing from libc-shim"
#endif

#ifndef ECONNREFUSED
#error "errno.h:ECONNREFUSED macro is missing from libc-shim"
#endif

#ifndef ECONNRESET
#error "errno.h:ECONNRESET macro is missing from libc-shim"
#endif

#ifndef EDEADLK
#error "errno.h:EDEADLK macro is missing from libc-shim"
#endif

#ifndef EDEADLOCK
#error "errno.h:EDEADLOCK macro is missing from libc-shim"
#endif

#ifndef EDESTADDRREQ
#error "errno.h:EDESTADDRREQ macro is missing from libc-shim"
#endif

#ifndef EDOM
#error "errno.h:EDOM macro is missing from libc-shim"
#endif

#ifndef EEXIST
#error "errno.h:EEXIST macro is missing from libc-shim"
#endif

#ifndef EFAULT
#error "errno.h:EFAULT macro is missing from libc-shim"
#endif

#ifndef EFBIG
#error "errno.h:EFBIG macro is missing from libc-shim"
#endif

#ifndef EHOSTUNREACH
#error "errno.h:EHOSTUNREACH macro is missing from libc-shim"
#endif

#ifndef EIDRM
#error "errno.h:EIDRM macro is missing from libc-shim"
#endif

#ifndef EILSEQ
#error "errno.h:EILSEQ macro is missing from libc-shim"
#endif

#ifndef EINPROGRESS
#error "errno.h:EINPROGRESS macro is missing from libc-shim"
#endif

#ifndef EINTR
#error "errno.h:EINTR macro is missing from libc-shim"
#endif

#ifndef EINVAL
#error "errno.h:EINVAL macro is missing from libc-shim"
#endif

#ifndef EIO
#error "errno.h:EIO macro is missing from libc-shim"
#endif

#ifndef EISCONN
#error "errno.h:EISCONN macro is missing from libc-shim"
#endif

#ifndef EISDIR
#error "errno.h:EISDIR macro is missing from libc-shim"
#endif

#ifndef ELOOP
#error "errno.h:ELOOP macro is missing from libc-shim"
#endif

#ifndef EMFILE
#error "errno.h:EMFILE macro is missing from libc-shim"
#endif

#ifndef EMLINK
#error "errno.h:EMLINK macro is missing from libc-shim"
#endif

#ifndef EMSGSIZE
#error "errno.h:EMSGSIZE macro is missing from libc-shim"
#endif

#ifndef ENAMETOOLONG
#error "errno.h:ENAMETOOLONG macro is missing from libc-shim"
#endif

#ifndef ENETDOWN
#error "errno.h:ENETDOWN macro is missing from libc-shim"
#endif

#ifndef ENETRESET
#error "errno.h:ENETRESET macro is missing from libc-shim"
#endif

#ifndef ENETUNREACH
#error "errno.h:ENETUNREACH macro is missing from libc-shim"
#endif

#ifndef ENFILE
#error "errno.h:ENFILE macro is missing from libc-shim"
#endif

#ifndef ENOBUFS
#error "errno.h:ENOBUFS macro is missing from libc-shim"
#endif

#ifndef ENODATA
#error "errno.h:ENODATA macro is missing from libc-shim"
#endif

#ifndef ENODEV
#error "errno.h:ENODEV macro is missing from libc-shim"
#endif

#ifndef ENOENT
#error "errno.h:ENOENT macro is missing from libc-shim"
#endif

#ifndef ENOEXEC
#error "errno.h:ENOEXEC macro is missing from libc-shim"
#endif

#ifndef ENOLCK
#error "errno.h:ENOLCK macro is missing from libc-shim"
#endif

#ifndef ENOLINK
#error "errno.h:ENOLINK macro is missing from libc-shim"
#endif

#ifndef ENOMEM
#error "errno.h:ENOMEM macro is missing from libc-shim"
#endif

#ifndef ENOMSG
#error "errno.h:ENOMSG macro is missing from libc-shim"
#endif

#ifndef ENOPROTOOPT
#error "errno.h:ENOPROTOOPT macro is missing from libc-shim"
#endif

#ifndef ENOSPC
#error "errno.h:ENOSPC macro is missing from libc-shim"
#endif

#ifndef ENOSR
#error "errno.h:ENOSR macro is missing from libc-shim"
#endif

#ifndef ENOSTR
#error "errno.h:ENOSTR macro is missing from libc-shim"
#endif

#ifndef ENOSYS
#error "errno.h:ENOSYS macro is missing from libc-shim"
#endif

#ifndef ENOTCONN
#error "errno.h:ENOTCONN macro is missing from libc-shim"
#endif

#ifndef ENOTDIR
#error "errno.h:ENOTDIR macro is missing from libc-shim"
#endif

#ifndef ENOTEMPTY
#error "errno.h:ENOTEMPTY macro is missing from libc-shim"
#endif

#ifndef ENOTRECOVERABLE
#error "errno.h:ENOTRECOVERABLE macro is missing from libc-shim"
#endif

#ifndef ENOTSOCK
#error "errno.h:ENOTSOCK macro is missing from libc-shim"
#endif

#ifndef ENOTSUP
#error "errno.h:ENOTSUP macro is missing from libc-shim"
#endif

#ifndef ENOTTY
#error "errno.h:ENOTTY macro is missing from libc-shim"
#endif

#ifndef ENXIO
#error "errno.h:ENXIO macro is missing from libc-shim"
#endif

#ifndef EOPNOTSUPP
#error "errno.h:EOPNOTSUPP macro is missing from libc-shim"
#endif

#ifndef EOTHER
#error "errno.h:EOTHER macro is missing from libc-shim"
#endif

#ifndef EOVERFLOW
#error "errno.h:EOVERFLOW macro is missing from libc-shim"
#endif

#ifndef EOWNERDEAD
#error "errno.h:EOWNERDEAD macro is missing from libc-shim"
#endif

#ifndef EPERM
#error "errno.h:EPERM macro is missing from libc-shim"
#endif

#ifndef EPIPE
#error "errno.h:EPIPE macro is missing from libc-shim"
#endif

#ifndef EPROTO
#error "errno.h:EPROTO macro is missing from libc-shim"
#endif

#ifndef EPROTONOSUPPORT
#error "errno.h:EPROTONOSUPPORT macro is missing from libc-shim"
#endif

#ifndef EPROTOTYPE
#error "errno.h:EPROTOTYPE macro is missing from libc-shim"
#endif

#ifndef ERANGE
#error "errno.h:ERANGE macro is missing from libc-shim"
#endif

#ifndef EROFS
#error "errno.h:EROFS macro is missing from libc-shim"
#endif

#ifndef ESPIPE
#error "errno.h:ESPIPE macro is missing from libc-shim"
#endif

#ifndef ESRCH
#error "errno.h:ESRCH macro is missing from libc-shim"
#endif

#ifndef ETIME
#error "errno.h:ETIME macro is missing from libc-shim"
#endif

#ifndef ETIMEDOUT
#error "errno.h:ETIMEDOUT macro is missing from libc-shim"
#endif

#ifndef ETXTBSY
#error "errno.h:ETXTBSY macro is missing from libc-shim"
#endif

#ifndef EWOULDBLOCK
#error "errno.h:EWOULDBLOCK macro is missing from libc-shim"
#endif

#ifndef EXDEV
#error "errno.h:EXDEV macro is missing from libc-shim"
#endif

#ifndef STRUNCATE
#error "errno.h:STRUNCATE macro is missing from libc-shim"
#endif

#ifndef _INC_ERRNO
#error "errno.h:_INC_ERRNO macro is missing from libc-shim"
#endif

#ifndef _SECURECRT_ERRCODE_VALUES_DEFINED
#error "errno.h:_SECURECRT_ERRCODE_VALUES_DEFINED macro is missing from libc-shim"
#endif

#ifndef _doserrno
#error "errno.h:_doserrno macro is missing from libc-shim"
#endif

#ifndef errno
#error "errno.h:errno macro is missing from libc-shim"
#endif

int main(void) { return 0; }
