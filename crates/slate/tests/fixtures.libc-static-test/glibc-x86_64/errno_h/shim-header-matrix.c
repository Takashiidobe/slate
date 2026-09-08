#include <errno.h>

extern int * slate_oracle___errno_location(void);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___errno_location), __typeof__(__errno_location)),
    "errno.h:__errno_location declaration differs from oracle");

static __typeof__(__errno_location) *const slate_reference___errno_location = &__errno_location;

extern char * slate_oracle_program_invocation_name;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_program_invocation_name), __typeof__(program_invocation_name)), "program_invocation_name object type differs from oracle");

static __typeof__(program_invocation_name) *const slate_reference_program_invocation_name = &program_invocation_name;

extern char * slate_oracle_program_invocation_short_name;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_program_invocation_short_name), __typeof__(program_invocation_short_name)), "program_invocation_short_name object type differs from oracle");

static __typeof__(program_invocation_short_name) *const slate_reference_program_invocation_short_name = &program_invocation_short_name;

typedef int slate_oracle_typedef_error_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_error_t, error_t), "typedef error_t differs from oracle");

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

#ifndef EADV
#error "errno.h:EADV macro is missing from libc-shim"
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

#ifndef EBADE
#error "errno.h:EBADE macro is missing from libc-shim"
#endif

#ifndef EBADF
#error "errno.h:EBADF macro is missing from libc-shim"
#endif

#ifndef EBADFD
#error "errno.h:EBADFD macro is missing from libc-shim"
#endif

#ifndef EBADMSG
#error "errno.h:EBADMSG macro is missing from libc-shim"
#endif

#ifndef EBADR
#error "errno.h:EBADR macro is missing from libc-shim"
#endif

#ifndef EBADRQC
#error "errno.h:EBADRQC macro is missing from libc-shim"
#endif

#ifndef EBADSLT
#error "errno.h:EBADSLT macro is missing from libc-shim"
#endif

#ifndef EBFONT
#error "errno.h:EBFONT macro is missing from libc-shim"
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

#ifndef ECHRNG
#error "errno.h:ECHRNG macro is missing from libc-shim"
#endif

#ifndef ECOMM
#error "errno.h:ECOMM macro is missing from libc-shim"
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

#ifndef EDOTDOT
#error "errno.h:EDOTDOT macro is missing from libc-shim"
#endif

#ifndef EDQUOT
#error "errno.h:EDQUOT macro is missing from libc-shim"
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

#ifndef EFSBADCRC
#error "errno.h:EFSBADCRC macro is missing from libc-shim"
#endif

#ifndef EFSCORRUPTED
#error "errno.h:EFSCORRUPTED macro is missing from libc-shim"
#endif

#ifndef EHOSTDOWN
#error "errno.h:EHOSTDOWN macro is missing from libc-shim"
#endif

#ifndef EHOSTUNREACH
#error "errno.h:EHOSTUNREACH macro is missing from libc-shim"
#endif

#ifndef EHWPOISON
#error "errno.h:EHWPOISON macro is missing from libc-shim"
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

#ifndef EISNAM
#error "errno.h:EISNAM macro is missing from libc-shim"
#endif

#ifndef EKEYEXPIRED
#error "errno.h:EKEYEXPIRED macro is missing from libc-shim"
#endif

#ifndef EKEYREJECTED
#error "errno.h:EKEYREJECTED macro is missing from libc-shim"
#endif

#ifndef EKEYREVOKED
#error "errno.h:EKEYREVOKED macro is missing from libc-shim"
#endif

#ifndef EL2HLT
#error "errno.h:EL2HLT macro is missing from libc-shim"
#endif

#ifndef EL2NSYNC
#error "errno.h:EL2NSYNC macro is missing from libc-shim"
#endif

#ifndef EL3HLT
#error "errno.h:EL3HLT macro is missing from libc-shim"
#endif

#ifndef EL3RST
#error "errno.h:EL3RST macro is missing from libc-shim"
#endif

#ifndef ELIBACC
#error "errno.h:ELIBACC macro is missing from libc-shim"
#endif

#ifndef ELIBBAD
#error "errno.h:ELIBBAD macro is missing from libc-shim"
#endif

#ifndef ELIBEXEC
#error "errno.h:ELIBEXEC macro is missing from libc-shim"
#endif

#ifndef ELIBMAX
#error "errno.h:ELIBMAX macro is missing from libc-shim"
#endif

#ifndef ELIBSCN
#error "errno.h:ELIBSCN macro is missing from libc-shim"
#endif

#ifndef ELNRNG
#error "errno.h:ELNRNG macro is missing from libc-shim"
#endif

#ifndef ELOOP
#error "errno.h:ELOOP macro is missing from libc-shim"
#endif

#ifndef EMEDIUMTYPE
#error "errno.h:EMEDIUMTYPE macro is missing from libc-shim"
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

#ifndef EMULTIHOP
#error "errno.h:EMULTIHOP macro is missing from libc-shim"
#endif

#ifndef ENAMETOOLONG
#error "errno.h:ENAMETOOLONG macro is missing from libc-shim"
#endif

#ifndef ENAVAIL
#error "errno.h:ENAVAIL macro is missing from libc-shim"
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

#ifndef ENOANO
#error "errno.h:ENOANO macro is missing from libc-shim"
#endif

#ifndef ENOBUFS
#error "errno.h:ENOBUFS macro is missing from libc-shim"
#endif

#ifndef ENOCSI
#error "errno.h:ENOCSI macro is missing from libc-shim"
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

#ifndef ENOKEY
#error "errno.h:ENOKEY macro is missing from libc-shim"
#endif

#ifndef ENOLCK
#error "errno.h:ENOLCK macro is missing from libc-shim"
#endif

#ifndef ENOLINK
#error "errno.h:ENOLINK macro is missing from libc-shim"
#endif

#ifndef ENOMEDIUM
#error "errno.h:ENOMEDIUM macro is missing from libc-shim"
#endif

#ifndef ENOMEM
#error "errno.h:ENOMEM macro is missing from libc-shim"
#endif

#ifndef ENOMSG
#error "errno.h:ENOMSG macro is missing from libc-shim"
#endif

#ifndef ENONET
#error "errno.h:ENONET macro is missing from libc-shim"
#endif

#ifndef ENOPKG
#error "errno.h:ENOPKG macro is missing from libc-shim"
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

#ifndef ENOTBLK
#error "errno.h:ENOTBLK macro is missing from libc-shim"
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

#ifndef ENOTNAM
#error "errno.h:ENOTNAM macro is missing from libc-shim"
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

#ifndef ENOTUNIQ
#error "errno.h:ENOTUNIQ macro is missing from libc-shim"
#endif

#ifndef ENXIO
#error "errno.h:ENXIO macro is missing from libc-shim"
#endif

#ifndef EOPNOTSUPP
#error "errno.h:EOPNOTSUPP macro is missing from libc-shim"
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

#ifndef EPFNOSUPPORT
#error "errno.h:EPFNOSUPPORT macro is missing from libc-shim"
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

#ifndef EREMCHG
#error "errno.h:EREMCHG macro is missing from libc-shim"
#endif

#ifndef EREMOTE
#error "errno.h:EREMOTE macro is missing from libc-shim"
#endif

#ifndef EREMOTEIO
#error "errno.h:EREMOTEIO macro is missing from libc-shim"
#endif

#ifndef ERESTART
#error "errno.h:ERESTART macro is missing from libc-shim"
#endif

#ifndef ERFKILL
#error "errno.h:ERFKILL macro is missing from libc-shim"
#endif

#ifndef EROFS
#error "errno.h:EROFS macro is missing from libc-shim"
#endif

#ifndef ESHUTDOWN
#error "errno.h:ESHUTDOWN macro is missing from libc-shim"
#endif

#ifndef ESOCKTNOSUPPORT
#error "errno.h:ESOCKTNOSUPPORT macro is missing from libc-shim"
#endif

#ifndef ESPIPE
#error "errno.h:ESPIPE macro is missing from libc-shim"
#endif

#ifndef ESRCH
#error "errno.h:ESRCH macro is missing from libc-shim"
#endif

#ifndef ESRMNT
#error "errno.h:ESRMNT macro is missing from libc-shim"
#endif

#ifndef ESTALE
#error "errno.h:ESTALE macro is missing from libc-shim"
#endif

#ifndef ESTRPIPE
#error "errno.h:ESTRPIPE macro is missing from libc-shim"
#endif

#ifndef ETIME
#error "errno.h:ETIME macro is missing from libc-shim"
#endif

#ifndef ETIMEDOUT
#error "errno.h:ETIMEDOUT macro is missing from libc-shim"
#endif

#ifndef ETOOMANYREFS
#error "errno.h:ETOOMANYREFS macro is missing from libc-shim"
#endif

#ifndef ETXTBSY
#error "errno.h:ETXTBSY macro is missing from libc-shim"
#endif

#ifndef EUCLEAN
#error "errno.h:EUCLEAN macro is missing from libc-shim"
#endif

#ifndef EUNATCH
#error "errno.h:EUNATCH macro is missing from libc-shim"
#endif

#ifndef EUSERS
#error "errno.h:EUSERS macro is missing from libc-shim"
#endif

#ifndef EWOULDBLOCK
#error "errno.h:EWOULDBLOCK macro is missing from libc-shim"
#endif

#ifndef EXDEV
#error "errno.h:EXDEV macro is missing from libc-shim"
#endif

#ifndef EXFULL
#error "errno.h:EXFULL macro is missing from libc-shim"
#endif

#ifndef _ASM_GENERIC_ERRNO_BASE_H
#error "errno.h:_ASM_GENERIC_ERRNO_BASE_H macro is missing from libc-shim"
#endif

#ifndef _ASM_GENERIC_ERRNO_H
#error "errno.h:_ASM_GENERIC_ERRNO_H macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "errno.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _BITS_ERRNO_H
#error "errno.h:_BITS_ERRNO_H macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "errno.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "errno.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ERRNO_H
#error "errno.h:_ERRNO_H macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "errno.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "errno.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "errno.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "errno.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "errno.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "errno.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "errno.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "errno.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "errno.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "errno.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "errno.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "errno.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "errno.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "errno.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

#ifndef errno
#error "errno.h:errno macro is missing from libc-shim"
#endif

int main(void) { return 0; }
