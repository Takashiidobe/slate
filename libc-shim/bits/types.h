#ifndef _SLATE_BITS_TYPES_H
#define _SLATE_BITS_TYPES_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/types.h> directly; include a standard header like <sys/types.h> or <unistd.h> instead."
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __NULL nullptr
#else
#define __NULL ((void *)0)
#endif

#if defined(__SLATE_LIBC_DARWIN)
typedef unsigned short __mode_t;
typedef int            __pid_t;
typedef unsigned int   __uid_t;
typedef unsigned int   __gid_t;
typedef int            __dev_t;
typedef unsigned short __nlink_t;
typedef unsigned int   __useconds_t;
typedef unsigned int   __clockid_t;
typedef void          *__timer_t;
typedef unsigned long  __clock_t;
typedef int            __key_t;
typedef unsigned int   __id_t;
typedef int            __daddr_t;
typedef char          *__caddr_t;
typedef unsigned char  __sa_family_t;
typedef unsigned int   __socklen_t;
typedef long long      __regoff_t;
typedef int            __error_t;
#elif defined(__SLATE_LIBC_FREEBSD)
typedef unsigned short __mode_t;
typedef int            __pid_t;
typedef unsigned int   __uid_t;
typedef unsigned int   __gid_t;
typedef unsigned long  __dev_t;
typedef unsigned long  __nlink_t;
typedef unsigned int   __useconds_t;
typedef int            __clockid_t;
typedef void          *__timer_t;
typedef int            __clock_t;
typedef long           __key_t;
typedef long           __id_t;
typedef long           __daddr_t;
typedef char          *__caddr_t;
typedef unsigned char  __sa_family_t;
typedef unsigned int   __socklen_t;
typedef long           __regoff_t;
typedef int            __error_t;
#else
typedef unsigned int       __mode_t;
typedef int                __pid_t;
typedef unsigned int       __uid_t;
typedef unsigned int       __gid_t;
typedef unsigned long long __dev_t;
#if defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_WORDSIZE_64)
typedef unsigned long __nlink_t;
#else
typedef unsigned int __nlink_t;
#endif
typedef unsigned int   __useconds_t;
typedef int            __clockid_t;
typedef void          *__timer_t;
typedef long           __clock_t;
typedef int            __key_t;
typedef int            __id_t;
typedef int            __daddr_t;
typedef char          *__caddr_t;
typedef unsigned short __sa_family_t;
typedef unsigned int   __socklen_t;
typedef long           __regoff_t;
typedef int            __error_t;
#endif

#ifdef __WINT_TYPE__
typedef __WINT_TYPE__ __wint_t;
#else
typedef unsigned int __wint_t;
#endif

#ifdef __WCHAR_TYPE__
typedef __WCHAR_TYPE__ __slate_wchar_t;
#else
typedef int __slate_wchar_t;
#endif

#if defined(__SLATE_LIBC_DARWIN)
typedef unsigned int __sigset_t;
#elif defined(__SLATE_LIBC_FREEBSD)
typedef struct {
  unsigned int __bits[4];
} __sigset_t;
#elif !defined(__SLATE_LIBC_MSVC)
typedef struct {
  unsigned long __bits[128 / sizeof(unsigned long)];
} __sigset_t;
#endif

#if defined(__SLATE_LIBC_DARWIN)
typedef int          __wctrans_t;
typedef unsigned int __wctype_t;
#elif defined(__SLATE_LIBC_BIONIC)
typedef const void *__wctrans_t;
typedef long        __wctype_t;
#elif defined(__SLATE_LIBC_FREEBSD)
typedef int           __wctrans_t;
typedef unsigned long __wctype_t;
#else
typedef const int    *__wctrans_t;
typedef unsigned long __wctype_t;
#endif

typedef signed char    __int8_t;
typedef unsigned char  __uint8_t;
typedef short          __int16_t;
typedef unsigned short __uint16_t;
typedef int            __int32_t;
typedef unsigned int   __uint32_t;

typedef signed char    __int_least8_t;
typedef unsigned char  __uint_least8_t;
typedef short          __int_least16_t;
typedef unsigned short __uint_least16_t;
typedef int            __int_least32_t;
typedef unsigned int   __uint_least32_t;

typedef signed char   __int_fast8_t;
typedef unsigned char __uint_fast8_t;

#if defined(__SLATE_LIBC_DARWIN)
typedef unsigned long      __size_t;
typedef long               __ssize_t;
typedef long long          __off_t;
typedef long long          __off64_t;
typedef long               __time_t;
typedef int                __suseconds_t;
typedef unsigned long long __ino_t;
typedef unsigned long long __ino64_t;
typedef int                __blksize_t;
typedef long long          __blkcnt_t;
typedef long long          __blkcnt64_t;
typedef unsigned int       __fsblkcnt_t;
typedef unsigned int       __fsfilcnt_t;
typedef long long          __int64_t;
typedef unsigned long long __uint64_t;
typedef long long          __int_least64_t;
typedef unsigned long long __uint_least64_t;
typedef short              __int_fast16_t;
typedef unsigned short     __uint_fast16_t;
typedef int                __int_fast32_t;
typedef unsigned int       __uint_fast32_t;
typedef long long          __int_fast64_t;
typedef unsigned long long __uint_fast64_t;
typedef long               __intptr_t;
typedef unsigned long      __uintptr_t;
typedef long               __intmax_t;
typedef unsigned long      __uintmax_t;
#elif defined(__SLATE_LIBC_FREEBSD)
typedef unsigned long __size_t;
typedef long           __ssize_t;
typedef long           __off_t;
typedef long           __off64_t;
typedef long           __time_t;
typedef long           __suseconds_t;
typedef unsigned long  __ino_t;
typedef unsigned long  __ino64_t;
typedef int            __blksize_t;
typedef long           __blkcnt_t;
typedef long           __blkcnt64_t;
typedef unsigned long  __fsblkcnt_t;
typedef unsigned long  __fsfilcnt_t;
typedef long            __int64_t;
typedef unsigned long   __uint64_t;
typedef long            __int_least64_t;
typedef unsigned long   __uint_least64_t;
typedef long            __int_fast16_t;
typedef unsigned long   __uint_fast16_t;
typedef long            __int_fast32_t;
typedef unsigned long   __uint_fast32_t;
typedef long            __int_fast64_t;
typedef unsigned long   __uint_fast64_t;
typedef long            __intptr_t;
typedef unsigned long   __uintptr_t;
typedef long            __intmax_t;
typedef unsigned long   __uintmax_t;
#elif defined(__SLATE_LIBC_MSVC)
typedef __SIZE_TYPE__      __size_t;
typedef __PTRDIFF_TYPE__   __ssize_t;
typedef long long          __off_t;
typedef long long          __off64_t;
typedef long long          __time_t;
typedef long long          __suseconds_t;
typedef unsigned long long __ino_t;
typedef unsigned long long __ino64_t;
typedef long               __blksize_t;
typedef long long          __blkcnt_t;
typedef long long          __blkcnt64_t;
typedef unsigned long long __fsblkcnt_t;
typedef unsigned long long __fsfilcnt_t;
typedef long long          __int64_t;
typedef unsigned long long __uint64_t;
typedef long long          __int_least64_t;
typedef unsigned long long __uint_least64_t;
typedef int                __int_fast16_t;
typedef unsigned int       __uint_fast16_t;
typedef int                __int_fast32_t;
typedef unsigned int       __uint_fast32_t;
typedef long long          __int_fast64_t;
typedef unsigned long long __uint_fast64_t;
typedef __INTPTR_TYPE__    __intptr_t;
typedef __UINTPTR_TYPE__   __uintptr_t;
typedef long long          __intmax_t;
typedef unsigned long long __uintmax_t;
#elif defined(__LP64__)
typedef unsigned long __size_t;
typedef long          __ssize_t;
typedef long          __off_t;
typedef long          __off64_t;
typedef long          __time_t;
typedef long          __suseconds_t;
typedef unsigned long __ino_t;
typedef unsigned long __ino64_t;
typedef long          __blksize_t;
typedef long          __blkcnt_t;
typedef long          __blkcnt64_t;
typedef unsigned long __fsblkcnt_t;
typedef unsigned long __fsfilcnt_t;
typedef long          __int64_t;
typedef unsigned long __uint64_t;
typedef long          __int_least64_t;
typedef unsigned long __uint_least64_t;
typedef long          __int_fast16_t;
typedef unsigned long __uint_fast16_t;
typedef long          __int_fast32_t;
typedef unsigned long __uint_fast32_t;
typedef long          __int_fast64_t;
typedef unsigned long __uint_fast64_t;
typedef long          __intptr_t;
typedef unsigned long __uintptr_t;
typedef long          __intmax_t;
typedef unsigned long __uintmax_t;
#else
typedef unsigned int       __size_t;
typedef int                __ssize_t;
typedef long long          __off_t;
typedef long long          __off64_t;
typedef long               __time_t;
typedef long               __suseconds_t;
typedef unsigned long long __ino_t;
typedef unsigned long long __ino64_t;
typedef long               __blksize_t;
typedef long long          __blkcnt_t;
typedef long long          __blkcnt64_t;
typedef unsigned long long __fsblkcnt_t;
typedef unsigned long long __fsfilcnt_t;
typedef long long          __int64_t;
typedef unsigned long long __uint64_t;
typedef long long          __int_least64_t;
typedef unsigned long long __uint_least64_t;
typedef int                __int_fast16_t;
typedef unsigned int       __uint_fast16_t;
typedef int                __int_fast32_t;
typedef unsigned int       __uint_fast32_t;
typedef long long          __int_fast64_t;
typedef unsigned long long __uint_fast64_t;
typedef int                __intptr_t;
typedef unsigned int       __uintptr_t;
typedef long long          __intmax_t;
typedef unsigned long long __uintmax_t;
#endif

#if !defined(__SLATE_LIBC_MSVC)
struct __locale_map;

struct __locale_struct {
  const struct __locale_map *cat[6];
};

#if defined(__SLATE_LIBC_DARWIN)
typedef union {
  char      __mbstate8[128];
  long long __mbstateL;
} __mbstate_t;

typedef __off_t __fpos_t;
#elif defined(__SLATE_LIBC_FREEBSD)
typedef union {
  char      __mbstate8[128];
  long long __mbstateL;
} __mbstate_t;

typedef __off_t __fpos_t;
#elif defined(__SLATE_LIBC_MUSL)
typedef struct {
  unsigned __opaque1;
  unsigned __opaque2;
} __mbstate_t;

#elif defined(__SLATE_LIBC_BIONIC)
typedef struct {
  unsigned char __seq[4];
#if defined(__SLATE_WORDSIZE_64)
  unsigned char __reserved[4];
#endif
} __mbstate_t;

#else
typedef struct {
  int __count;
  union {
    int  __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;
#endif

#if !defined(__SLATE_LIBC_DARWIN) && !defined(__SLATE_LIBC_FREEBSD)
typedef struct __fpos_t {
  __off_t     __pos;
  __mbstate_t __state;
} __fpos_t;
#endif

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/pthread-types.h>

#else

typedef union {
#if defined(__SLATE_ARCH_AARCH64) && !defined(__SLATE_LIBC_MUSL)
  char __size[64];
#elif defined(__LP64__)
  char __size[56];
#else
  char __size[36];
#endif
  long __align;
} __pthread_attr_t;

typedef struct {
  char __size[40];
} __pthread_mutex_t;

typedef struct {
  char __size[48];
} __pthread_cond_t;

typedef struct {
  char __size[56];
} __pthread_rwlock_t;

typedef struct {
  char __size[32];
} __pthread_barrier_t;

typedef int __pthread_once_t;

typedef unsigned long __pthread_t;

typedef union {
  char __size[4];
  int  __align;
} __pthread_mutexattr_t;

typedef union {
  char __size[4];
  int  __align;
} __pthread_condattr_t;

typedef union {
  char __size[8];
  long __align;
} __pthread_rwlockattr_t;

typedef union {
  char __size[4];
  int  __align;
} __pthread_barrierattr_t;

typedef volatile int __pthread_spinlock_t;

typedef unsigned int __pthread_key_t;

#endif
#endif

typedef __builtin_va_list __va_list;

#endif /* _SLATE_BITS_TYPES_H */

#if defined(__NEED_va_list) && !defined(__DEFINED_va_list)
#define va_list __va_list
#define __DEFINED_va_list
#endif
#undef __NEED_va_list

#if defined(__NEED_NULL) && !defined(__DEFINED_NULL)
#define NULL __NULL
#define __DEFINED_NULL
#endif
#undef __NEED_NULL

#if defined(__NEED_pthread_t) && !defined(__DEFINED_pthread_t)
typedef __pthread_t pthread_t;
#define __DEFINED_pthread_t
#endif
#undef __NEED_pthread_t

#if defined(__NEED_pthread_attr_t) && !defined(__DEFINED_pthread_attr_t)
typedef __pthread_attr_t pthread_attr_t;
#define __DEFINED_pthread_attr_t
#endif
#undef __NEED_pthread_attr_t

#if defined(__NEED_pthread_mutex_t) && !defined(__DEFINED_pthread_mutex_t)
typedef __pthread_mutex_t pthread_mutex_t;
#define __DEFINED_pthread_mutex_t
#endif
#undef __NEED_pthread_mutex_t

#if defined(__NEED_pthread_cond_t) && !defined(__DEFINED_pthread_cond_t)
typedef __pthread_cond_t pthread_cond_t;
#define __DEFINED_pthread_cond_t
#endif
#undef __NEED_pthread_cond_t

#if defined(__NEED_pthread_rwlock_t) && !defined(__DEFINED_pthread_rwlock_t)
typedef __pthread_rwlock_t pthread_rwlock_t;
#define __DEFINED_pthread_rwlock_t
#endif
#undef __NEED_pthread_rwlock_t

#if defined(__NEED_pthread_barrier_t) && !defined(__DEFINED_pthread_barrier_t)
typedef __pthread_barrier_t pthread_barrier_t;
#define __DEFINED_pthread_barrier_t
#endif
#undef __NEED_pthread_barrier_t

#if defined(__NEED_pthread_once_t) && !defined(__DEFINED_pthread_once_t)
typedef __pthread_once_t pthread_once_t;
#define __DEFINED_pthread_once_t
#endif
#undef __NEED_pthread_once_t

#if defined(__NEED_pthread_mutexattr_t) &&                                     \
    !defined(__DEFINED_pthread_mutexattr_t)
typedef __pthread_mutexattr_t pthread_mutexattr_t;
#define __DEFINED_pthread_mutexattr_t
#endif
#undef __NEED_pthread_mutexattr_t

#if defined(__NEED_pthread_condattr_t) && !defined(__DEFINED_pthread_condattr_t)
typedef __pthread_condattr_t pthread_condattr_t;
#define __DEFINED_pthread_condattr_t
#endif
#undef __NEED_pthread_condattr_t

#if defined(__NEED_pthread_rwlockattr_t) &&                                    \
    !defined(__DEFINED_pthread_rwlockattr_t)
typedef __pthread_rwlockattr_t pthread_rwlockattr_t;
#define __DEFINED_pthread_rwlockattr_t
#endif
#undef __NEED_pthread_rwlockattr_t

#if defined(__NEED_pthread_barrierattr_t) &&                                   \
    !defined(__DEFINED_pthread_barrierattr_t)
typedef __pthread_barrierattr_t pthread_barrierattr_t;
#define __DEFINED_pthread_barrierattr_t
#endif
#undef __NEED_pthread_barrierattr_t

#if defined(__NEED_pthread_spinlock_t) && !defined(__DEFINED_pthread_spinlock_t)
typedef __pthread_spinlock_t pthread_spinlock_t;
#define __DEFINED_pthread_spinlock_t
#endif
#undef __NEED_pthread_spinlock_t

#if defined(__NEED_pthread_key_t) && !defined(__DEFINED_pthread_key_t)
typedef __pthread_key_t pthread_key_t;
#define __DEFINED_pthread_key_t
#endif
#undef __NEED_pthread_key_t

#if defined(__NEED_error_t) && !defined(__DEFINED_error_t)
typedef __error_t error_t;
#define __DEFINED_error_t
#endif
#undef __NEED_error_t

#if defined(__NEED_sigset_t) && !defined(__DEFINED_sigset_t)
typedef __sigset_t sigset_t;
#define __DEFINED_sigset_t
#endif
#undef __NEED_sigset_t

#if defined(__NEED_wchar_t) && !defined(__DEFINED_wchar_t)
typedef __slate_wchar_t wchar_t;
#define __DEFINED_wchar_t
#endif
#undef __NEED_wchar_t

#if defined(__NEED_wctrans_t) && !defined(__DEFINED_wctrans_t)
#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/types.h>
#else
typedef __wctrans_t wctrans_t;
#define __DEFINED_wctrans_t
#endif
#endif
#undef __NEED_wctrans_t

#if defined(__NEED_wctype_t) && !defined(__DEFINED_wctype_t)
#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/types.h>
#else
typedef __wctype_t wctype_t;
#define __DEFINED_wctype_t
#endif
#endif
#undef __NEED_wctype_t

#if defined(__NEED_wint_t) && !defined(__DEFINED_wint_t)
#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/types.h>
#else
typedef __wint_t wint_t;
#define __DEFINED_wint_t
#endif
#endif
#undef __NEED_wint_t

#if defined(__NEED_regoff_t) && !defined(__DEFINED_regoff_t)
typedef __regoff_t regoff_t;
#define __DEFINED_regoff_t
#endif
#undef __NEED_regoff_t

#if defined(__NEED_struct_timeval) && !defined(__DEFINED_struct_timeval)
struct timeval {
  __time_t      tv_sec;  /* seconds */
  __suseconds_t tv_usec; /* microseconds */
};
#define __DEFINED_struct_timeval
#endif
#undef __NEED_struct_timeval

#if defined(__NEED_struct_timespec) && !defined(__DEFINED_struct_timespec)
struct timespec {
  __time_t tv_sec;
  long     tv_nsec;
};
#define __DEFINED_struct_timespec
#endif
#undef __NEED_struct_timespec

#if defined(__NEED_struct_winsize) && !defined(__DEFINED_struct_winsize)
struct winsize {
  unsigned short ws_row;
  unsigned short ws_col;
  unsigned short ws_xpixel;
  unsigned short ws_ypixel;
};
#define __DEFINED_struct_winsize
#endif
#undef __NEED_struct_winsize

#if defined(__NEED_FILE) && !defined(__DEFINED_FILE)
#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/types.h>
#elif defined(__SLATE_LIBC_DARWIN) || defined(__SLATE_LIBC_FREEBSD)
typedef struct __sFILE FILE;
#define __DEFINED_FILE
#else
typedef struct FILE FILE;
#define __DEFINED_FILE
#endif
#endif
#undef __NEED_FILE

#if defined(__NEED_mbstate_t) && !defined(__DEFINED_mbstate_t)
#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/types.h>
#else
typedef __mbstate_t mbstate_t;
#define __DEFINED_mbstate_t
#endif
#endif
#undef __NEED_mbstate_t

#if defined(__NEED_fpos_t) && !defined(__DEFINED_fpos_t)
typedef __fpos_t fpos_t;
#define __DEFINED_fpos_t
#endif
#undef __NEED_fpos_t

#if defined(__NEED_float_t) && !defined(__DEFINED_float_t)
typedef float float_t;
#define __DEFINED_float_t
#endif
#undef __NEED_float_t

#if defined(__NEED_double_t) && !defined(__DEFINED_double_t)
typedef double double_t;
#define __DEFINED_double_t
#endif
#undef __NEED_double_t

#if defined(__NEED_size_t) && !defined(__DEFINED_size_t)
typedef __size_t size_t;
#define __DEFINED_size_t
#endif
#undef __NEED_size_t

#if defined(__NEED_ssize_t) && !defined(__DEFINED_ssize_t)
typedef __ssize_t ssize_t;
#define __DEFINED_ssize_t
#endif
#undef __NEED_ssize_t

#if defined(__NEED_struct_iovec) && !defined(__DEFINED_struct_iovec)
struct iovec {
  void  *iov_base;
  size_t iov_len;
};
#define __DEFINED_struct_iovec
#endif
#undef __NEED_struct_iovec

#if defined(__NEED_off_t) && !defined(__DEFINED_off_t)
typedef __off_t off_t;
#define __DEFINED_off_t
#endif
#undef __NEED_off_t

#if defined(__NEED_off64_t) && !defined(__DEFINED_off64_t)
typedef __off64_t off64_t;
#define __DEFINED_off64_t
#endif
#undef __NEED_off64_t

#if defined(__NEED_mode_t) && !defined(__DEFINED_mode_t)
typedef __mode_t mode_t;
#define __DEFINED_mode_t
#endif
#undef __NEED_mode_t

#if defined(__NEED_pid_t) && !defined(__DEFINED_pid_t)
typedef __pid_t pid_t;
#define __DEFINED_pid_t
#endif
#undef __NEED_pid_t

#if defined(__NEED_uid_t) && !defined(__DEFINED_uid_t)
typedef __uid_t uid_t;
#define __DEFINED_uid_t
#endif
#undef __NEED_uid_t

#if defined(__NEED_gid_t) && !defined(__DEFINED_gid_t)
typedef __gid_t gid_t;
#define __DEFINED_gid_t
#endif
#undef __NEED_gid_t

#if defined(__NEED_time_t) && !defined(__DEFINED_time_t)
typedef __time_t time_t;
#define __DEFINED_time_t
#endif
#undef __NEED_time_t

#if defined(__NEED_suseconds_t) && !defined(__DEFINED_suseconds_t)
typedef __suseconds_t suseconds_t;
#define __DEFINED_suseconds_t
#endif
#undef __NEED_suseconds_t

#if defined(__NEED_useconds_t) && !defined(__DEFINED_useconds_t)
typedef __useconds_t useconds_t;
#define __DEFINED_useconds_t
#endif
#undef __NEED_useconds_t

#if defined(__NEED_ino_t) && !defined(__DEFINED_ino_t)
typedef __ino_t ino_t;
#define __DEFINED_ino_t
#endif
#undef __NEED_ino_t

#if defined(__NEED_ino64_t) && !defined(__DEFINED_ino64_t)
typedef __ino64_t ino64_t;
#define __DEFINED_ino64_t
#endif
#undef __NEED_ino64_t

#if defined(__NEED_dev_t) && !defined(__DEFINED_dev_t)
typedef __dev_t dev_t;
#define __DEFINED_dev_t
#endif
#undef __NEED_dev_t

#if defined(__NEED_nlink_t) && !defined(__DEFINED_nlink_t)
typedef __nlink_t nlink_t;
#define __DEFINED_nlink_t
#endif
#undef __NEED_nlink_t

#if defined(__NEED_blksize_t) && !defined(__DEFINED_blksize_t)
typedef __blksize_t blksize_t;
#define __DEFINED_blksize_t
#endif
#undef __NEED_blksize_t

#if defined(__NEED_blkcnt_t) && !defined(__DEFINED_blkcnt_t)
typedef __blkcnt_t blkcnt_t;
#define __DEFINED_blkcnt_t
#endif
#undef __NEED_blkcnt_t

#if defined(__NEED_blkcnt64_t) && !defined(__DEFINED_blkcnt64_t)
typedef __blkcnt64_t blkcnt64_t;
#define __DEFINED_blkcnt64_t
#endif
#undef __NEED_blkcnt64_t

#if defined(__NEED_fsblkcnt_t) && !defined(__DEFINED_fsblkcnt_t)
typedef __fsblkcnt_t fsblkcnt_t;
#define __DEFINED_fsblkcnt_t
#endif
#undef __NEED_fsblkcnt_t

#if defined(__NEED_fsfilcnt_t) && !defined(__DEFINED_fsfilcnt_t)
typedef __fsfilcnt_t fsfilcnt_t;
#define __DEFINED_fsfilcnt_t
#endif
#undef __NEED_fsfilcnt_t

#if defined(__NEED_clock_t) && !defined(__DEFINED_clock_t)
typedef __clock_t clock_t;
#define __DEFINED_clock_t
#endif
#undef __NEED_clock_t

#if defined(__NEED_clockid_t) && !defined(__DEFINED_clockid_t)
typedef __clockid_t clockid_t;
#define __DEFINED_clockid_t
#endif
#undef __NEED_clockid_t

#if defined(__NEED_timer_t) && !defined(__DEFINED_timer_t)
typedef __timer_t timer_t;
#define __DEFINED_timer_t
#endif
#undef __NEED_timer_t

#if defined(__NEED_key_t) && !defined(__DEFINED_key_t)
typedef __key_t key_t;
#define __DEFINED_key_t
#endif
#undef __NEED_key_t

#if defined(__NEED_id_t) && !defined(__DEFINED_id_t)
typedef __id_t id_t;
#define __DEFINED_id_t
#endif
#undef __NEED_id_t

#if defined(__NEED_daddr_t) && !defined(__DEFINED_daddr_t)
typedef __daddr_t daddr_t;
#define __DEFINED_daddr_t
#endif
#undef __NEED_daddr_t

#if defined(__NEED_caddr_t) && !defined(__DEFINED_caddr_t)
typedef __caddr_t caddr_t;
#define __DEFINED_caddr_t
#endif
#undef __NEED_caddr_t

#if defined(__NEED_sa_family_t) && !defined(__DEFINED_sa_family_t)
typedef __sa_family_t sa_family_t;
#define __DEFINED_sa_family_t
#endif
#undef __NEED_sa_family_t

#if defined(__NEED_socklen_t) && !defined(__DEFINED_socklen_t)
typedef __socklen_t socklen_t;
#define __DEFINED_socklen_t
#endif
#undef __NEED_socklen_t

#if defined(__NEED_locale_t) && !defined(__DEFINED_locale_t)
#if defined(__SLATE_LIBC_DARWIN)
typedef struct _xlocale *locale_t;
#else
typedef struct __locale_struct *locale_t;
#endif
#define __DEFINED_locale_t
#endif
#undef __NEED_locale_t

#if defined(__NEED_int8_t) && !defined(__DEFINED_int8_t)
typedef __int8_t int8_t;
#define __DEFINED_int8_t
#endif
#undef __NEED_int8_t

#if defined(__NEED_uint8_t) && !defined(__DEFINED_uint8_t)
typedef __uint8_t uint8_t;
#define __DEFINED_uint8_t
#endif
#undef __NEED_uint8_t

#if defined(__NEED_int16_t) && !defined(__DEFINED_int16_t)
typedef __int16_t int16_t;
#define __DEFINED_int16_t
#endif
#undef __NEED_int16_t

#if defined(__NEED_uint16_t) && !defined(__DEFINED_uint16_t)
typedef __uint16_t uint16_t;
#define __DEFINED_uint16_t
#endif
#undef __NEED_uint16_t

#if defined(__NEED_int32_t) && !defined(__DEFINED_int32_t)
typedef __int32_t int32_t;
#define __DEFINED_int32_t
#endif
#undef __NEED_int32_t

#if defined(__NEED_uint32_t) && !defined(__DEFINED_uint32_t)
typedef __uint32_t uint32_t;
#define __DEFINED_uint32_t
#endif
#undef __NEED_uint32_t

#if defined(__NEED_int64_t) && !defined(__DEFINED_int64_t)
typedef __int64_t int64_t;
#define __DEFINED_int64_t
#endif
#undef __NEED_int64_t

#if defined(__NEED_uint64_t) && !defined(__DEFINED_uint64_t)
typedef __uint64_t uint64_t;
#define __DEFINED_uint64_t
#endif
#undef __NEED_uint64_t

#if defined(__NEED_int_least8_t) && !defined(__DEFINED_int_least8_t)
typedef __int_least8_t int_least8_t;
#define __DEFINED_int_least8_t
#endif
#undef __NEED_int_least8_t

#if defined(__NEED_uint_least8_t) && !defined(__DEFINED_uint_least8_t)
typedef __uint_least8_t uint_least8_t;
#define __DEFINED_uint_least8_t
#endif
#undef __NEED_uint_least8_t

#if defined(__NEED_int_least16_t) && !defined(__DEFINED_int_least16_t)
typedef __int_least16_t int_least16_t;
#define __DEFINED_int_least16_t
#endif
#undef __NEED_int_least16_t

#if defined(__NEED_uint_least16_t) && !defined(__DEFINED_uint_least16_t)
typedef __uint_least16_t uint_least16_t;
#define __DEFINED_uint_least16_t
#endif
#undef __NEED_uint_least16_t

#if defined(__NEED_int_least32_t) && !defined(__DEFINED_int_least32_t)
typedef __int_least32_t int_least32_t;
#define __DEFINED_int_least32_t
#endif
#undef __NEED_int_least32_t

#if defined(__NEED_uint_least32_t) && !defined(__DEFINED_uint_least32_t)
typedef __uint_least32_t uint_least32_t;
#define __DEFINED_uint_least32_t
#endif
#undef __NEED_uint_least32_t

#if defined(__NEED_int_least64_t) && !defined(__DEFINED_int_least64_t)
typedef __int_least64_t int_least64_t;
#define __DEFINED_int_least64_t
#endif
#undef __NEED_int_least64_t

#if defined(__NEED_uint_least64_t) && !defined(__DEFINED_uint_least64_t)
typedef __uint_least64_t uint_least64_t;
#define __DEFINED_uint_least64_t
#endif
#undef __NEED_uint_least64_t

#if defined(__NEED_int_fast8_t) && !defined(__DEFINED_int_fast8_t)
typedef __int_fast8_t int_fast8_t;
#define __DEFINED_int_fast8_t
#endif
#undef __NEED_int_fast8_t

#if defined(__NEED_uint_fast8_t) && !defined(__DEFINED_uint_fast8_t)
typedef __uint_fast8_t uint_fast8_t;
#define __DEFINED_uint_fast8_t
#endif
#undef __NEED_uint_fast8_t

#if defined(__NEED_int_fast16_t) && !defined(__DEFINED_int_fast16_t)
typedef __int_fast16_t int_fast16_t;
#define __DEFINED_int_fast16_t
#endif
#undef __NEED_int_fast16_t

#if defined(__NEED_uint_fast16_t) && !defined(__DEFINED_uint_fast16_t)
typedef __uint_fast16_t uint_fast16_t;
#define __DEFINED_uint_fast16_t
#endif
#undef __NEED_uint_fast16_t

#if defined(__NEED_int_fast32_t) && !defined(__DEFINED_int_fast32_t)
typedef __int_fast32_t int_fast32_t;
#define __DEFINED_int_fast32_t
#endif
#undef __NEED_int_fast32_t

#if defined(__NEED_uint_fast32_t) && !defined(__DEFINED_uint_fast32_t)
typedef __uint_fast32_t uint_fast32_t;
#define __DEFINED_uint_fast32_t
#endif
#undef __NEED_uint_fast32_t

#if defined(__NEED_int_fast64_t) && !defined(__DEFINED_int_fast64_t)
typedef __int_fast64_t int_fast64_t;
#define __DEFINED_int_fast64_t
#endif
#undef __NEED_int_fast64_t

#if defined(__NEED_uint_fast64_t) && !defined(__DEFINED_uint_fast64_t)
typedef __uint_fast64_t uint_fast64_t;
#define __DEFINED_uint_fast64_t
#endif
#undef __NEED_uint_fast64_t

#if defined(__NEED_intptr_t) && !defined(__DEFINED_intptr_t)
typedef __intptr_t intptr_t;
#define __DEFINED_intptr_t
#endif
#undef __NEED_intptr_t

#if defined(__NEED_uintptr_t) && !defined(__DEFINED_uintptr_t)
typedef __uintptr_t uintptr_t;
#define __DEFINED_uintptr_t
#endif
#undef __NEED_uintptr_t

#if defined(__NEED_intmax_t) && !defined(__DEFINED_intmax_t)
typedef __intmax_t intmax_t;
#define __DEFINED_intmax_t
#endif
#undef __NEED_intmax_t

#if defined(__NEED_uintmax_t) && !defined(__DEFINED_uintmax_t)
typedef __uintmax_t uintmax_t;
#define __DEFINED_uintmax_t
#endif
#undef __NEED_uintmax_t
