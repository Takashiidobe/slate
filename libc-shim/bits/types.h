#ifndef _SLATE_BITS_TYPES_H
#define _SLATE_BITS_TYPES_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/types.h> directly; include a standard header like <sys/types.h> or <unistd.h> instead."
#endif

typedef unsigned int       __mode_t;
typedef int                __pid_t;
typedef unsigned int       __uid_t;
typedef unsigned int       __gid_t;
typedef unsigned long long __dev_t;
typedef unsigned int       __nlink_t;
typedef unsigned int       __useconds_t;
typedef int                __clockid_t;
typedef void              *__timer_t;
typedef long               __clock_t;
typedef int                __key_t;
typedef int                __id_t;
typedef int                __daddr_t;
typedef char              *__caddr_t;
typedef unsigned char      __sa_family_t;
typedef unsigned int       __socklen_t;
typedef long               __regoff_t;

#ifdef __WINT_TYPE__
typedef __WINT_TYPE__ __wint_t;
#else
typedef unsigned int __wint_t;
#endif

#ifdef __WCHAR_TYPE__
typedef __WCHAR_TYPE__ __wchar_t;
#else
typedef int __wchar_t;
#endif

typedef struct {
  unsigned long __bits[128 / sizeof(unsigned long)];
} __sigset_t;

typedef const int    *__wctrans_t;
typedef unsigned long __wctype_t;

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

#if defined(__LP64__)
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

struct __locale_map;

struct __locale_struct {
  const struct __locale_map *cat[6];
};

typedef struct {
  int __count;
  union {
    int  __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;

typedef struct __fpos_t {
  __off_t     __pos;
  __mbstate_t __state;
} __fpos_t;

struct __timespec {
  __time_t tv_sec;
  long     tv_nsec;
} __timespec;

#if defined(__NEED_sigset_t) && !defined(__DEFINED_sigset_t)
typedef __sigset_t sigset_t;
#define __DEFINED_sigset_t
#endif

#if defined(__NEED_wchar_t) && !defined(__DEFINED_wchar_t)
typedef __wchar_t wchar_t;
#define __DEFINED_wchar_t
#endif

#if defined(__NEED_wctrans_t) && !defined(__DEFINED_wctrans_t)
typedef __wctrans_t wctrans_t;
#define __DEFINED_wctrans_t
#endif

#if defined(__NEED_wctype_t) && !defined(__DEFINED_wctype_t)
typedef __wctype_t wctype_t;
#define __DEFINED_wctype_t
#endif

#if defined(__NEED_wint_t) && !defined(__DEFINED_wint_t)
typedef __wint_t wint_t;
#define __DEFINED_wint_t
#endif

#if defined(__NEED_wctrans_t) && !defined(__DEFINED_wctrans_t)
typedef __wctrans_t wctrans_t;
#define __DEFINED_wctrans_t
#endif

#if defined(__NEED_regoff_t) && !defined(__DEFINED_regoff_t)
typedef __regoff_t regoff_t;
#define __DEFINED_regoff_t
#endif

#if defined(__NEED_struct_timespec) && !defined(__DEFINED_struct_timespec)
typedef struct __timespec timespec;
#define __DEFINED_struct_timespec
#endif

#if defined(__NEED_FILE) && !defined(__DEFINED_FILE)
typedef struct FILE FILE;
#define __DEFINED_FILE
#endif

#if defined(__NEED_fpos_t) && !defined(__DEFINED_fpos_t)
typedef __fpos_t fpos_t;
#define __DEFINED_fpos_t
#endif

#if defined(__NEED_size_t) && !defined(__DEFINED_size_t)
typedef __size_t size_t;
#define __DEFINED_size_t
#endif

#if defined(__NEED_ssize_t) && !defined(__DEFINED_ssize_t)
typedef __ssize_t ssize_t;
#define __DEFINED_ssize_t
#endif

#if defined(__NEED_off_t) && !defined(__DEFINED_off_t)
typedef __off_t off_t;
#define __DEFINED_off_t
#endif

#if defined(__NEED_off64_t) && !defined(__DEFINED_off64_t)
typedef __off64_t off64_t;
#define __DEFINED_off64_t
#endif

#if defined(__NEED_mode_t) && !defined(__DEFINED_mode_t)
typedef __mode_t mode_t;
#define __DEFINED_mode_t
#endif

#if defined(__NEED_pid_t) && !defined(__DEFINED_pid_t)
typedef __pid_t pid_t;
#define __DEFINED_pid_t
#endif

#if defined(__NEED_uid_t) && !defined(__DEFINED_uid_t)
typedef __uid_t uid_t;
#define __DEFINED_uid_t
#endif

#if defined(__NEED_gid_t) && !defined(__DEFINED_gid_t)
typedef __gid_t gid_t;
#define __DEFINED_gid_t
#endif

#if defined(__NEED_time_t) && !defined(__DEFINED_time_t)
typedef __time_t time_t;
#define __DEFINED_time_t
#endif

#if defined(__NEED_suseconds_t) && !defined(__DEFINED_suseconds_t)
typedef __suseconds_t suseconds_t;
#define __DEFINED_suseconds_t
#endif

#if defined(__NEED_useconds_t) && !defined(__DEFINED_useconds_t)
typedef __useconds_t useconds_t;
#define __DEFINED_useconds_t
#endif

#if defined(__NEED_ino_t) && !defined(__DEFINED_ino_t)
typedef __ino_t ino_t;
#define __DEFINED_ino_t
#endif

#if defined(__NEED_ino64_t) && !defined(__DEFINED_ino64_t)
typedef __ino64_t ino64_t;
#define __DEFINED_ino64_t
#endif

#if defined(__NEED_dev_t) && !defined(__DEFINED_dev_t)
typedef __dev_t dev_t;
#define __DEFINED_dev_t
#endif

#if defined(__NEED_nlink_t) && !defined(__DEFINED_nlink_t)
typedef __nlink_t nlink_t;
#define __DEFINED_nlink_t
#endif

#if defined(__NEED_blksize_t) && !defined(__DEFINED_blksize_t)
typedef __blksize_t blksize_t;
#define __DEFINED_blksize_t
#endif

#if defined(__NEED_blkcnt_t) && !defined(__DEFINED_blkcnt_t)
typedef __blkcnt_t blkcnt_t;
#define __DEFINED_blkcnt_t
#endif

#if defined(__NEED_blkcnt64_t) && !defined(__DEFINED_blkcnt64_t)
typedef __blkcnt64_t blkcnt64_t;
#define __DEFINED_blkcnt64_t
#endif

#if defined(__NEED_fsblkcnt_t) && !defined(__DEFINED_fsblkcnt_t)
typedef __fsblkcnt_t fsblkcnt_t;
#define __DEFINED_fsblkcnt_t
#endif

#if defined(__NEED_fsfilcnt_t) && !defined(__DEFINED_fsfilcnt_t)
typedef __fsfilcnt_t fsfilcnt_t;
#define __DEFINED_fsfilcnt_t
#endif

#if defined(__NEED_clock_t) && !defined(__DEFINED_clock_t)
typedef __clock_t clock_t;
#define __DEFINED_clock_t
#endif

#if defined(__NEED_clockid_t) && !defined(__DEFINED_clockid_t)
typedef __clockid_t clockid_t;
#define __DEFINED_clockid_t
#endif

#if defined(__NEED_timer_t) && !defined(__DEFINED_timer_t)
typedef __timer_t timer_t;
#define __DEFINED_timer_t
#endif

#if defined(__NEED_key_t) && !defined(__DEFINED_key_t)
typedef __key_t key_t;
#define __DEFINED_key_t
#endif

#if defined(__NEED_id_t) && !defined(__DEFINED_id_t)
typedef __id_t id_t;
#define __DEFINED_id_t
#endif

#if defined(__NEED_daddr_t) && !defined(__DEFINED_daddr_t)
typedef __daddr_t daddr_t;
#define __DEFINED_daddr_t
#endif

#if defined(__NEED_caddr_t) && !defined(__DEFINED_caddr_t)
typedef __caddr_t caddr_t;
#define __DEFINED_caddr_t
#endif

#if defined(__NEED_sa_family_t) && !defined(__DEFINED_sa_family_t)
typedef __sa_family_t sa_family_t;
#define __DEFINED_sa_family_t
#endif

#if defined(__NEED_socklen_t) && !defined(__DEFINED_socklen_t)
typedef __socklen_t socklen_t;
#define __DEFINED_socklen_t
#endif

#if defined(__NEED_locale_t) && !defined(__DEFINED_locale_t)
typedef struct __locale_struct *locale_t;
#define __DEFINED_locale_t
#endif

#if defined(__NEED_int8_t) && !defined(__DEFINED_int8_t)
typedef __int8_t int8_t;
#define __DEFINED_int8_t
#endif

#if defined(__NEED_uint8_t) && !defined(__DEFINED_uint8_t)
typedef __uint8_t uint8_t;
#define __DEFINED_uint8_t
#endif

#if defined(__NEED_int16_t) && !defined(__DEFINED_int16_t)
typedef __int16_t int16_t;
#define __DEFINED_int16_t
#endif

#if defined(__NEED_uint16_t) && !defined(__DEFINED_uint16_t)
typedef __uint16_t uint16_t;
#define __DEFINED_uint16_t
#endif

#if defined(__NEED_int32_t) && !defined(__DEFINED_int32_t)
typedef __int32_t int32_t;
#define __DEFINED_int32_t
#endif

#if defined(__NEED_uint32_t) && !defined(__DEFINED_uint32_t)
typedef __uint32_t uint32_t;
#define __DEFINED_uint32_t
#endif

#if defined(__NEED_int64_t) && !defined(__DEFINED_int64_t)
typedef __int64_t int64_t;
#define __DEFINED_int64_t
#endif

#if defined(__NEED_uint64_t) && !defined(__DEFINED_uint64_t)
typedef __uint64_t uint64_t;
#define __DEFINED_uint64_t
#endif

#if defined(__NEED_int_least8_t) && !defined(__DEFINED_int_least8_t)
typedef __int_least8_t int_least8_t;
#define __DEFINED_int_least8_t
#endif

#if defined(__NEED_uint_least8_t) && !defined(__DEFINED_uint_least8_t)
typedef __uint_least8_t uint_least8_t;
#define __DEFINED_uint_least8_t
#endif

#if defined(__NEED_int_least16_t) && !defined(__DEFINED_int_least16_t)
typedef __int_least16_t int_least16_t;
#define __DEFINED_int_least16_t
#endif

#if defined(__NEED_uint_least16_t) && !defined(__DEFINED_uint_least16_t)
typedef __uint_least16_t uint_least16_t;
#define __DEFINED_uint_least16_t
#endif

#if defined(__NEED_int_least32_t) && !defined(__DEFINED_int_least32_t)
typedef __int_least32_t int_least32_t;
#define __DEFINED_int_least32_t
#endif

#if defined(__NEED_uint_least32_t) && !defined(__DEFINED_uint_least32_t)
typedef __uint_least32_t uint_least32_t;
#define __DEFINED_uint_least32_t
#endif

#if defined(__NEED_int_least64_t) && !defined(__DEFINED_int_least64_t)
typedef __int_least64_t int_least64_t;
#define __DEFINED_int_least64_t
#endif

#if defined(__NEED_uint_least64_t) && !defined(__DEFINED_uint_least64_t)
typedef __uint_least64_t uint_least64_t;
#define __DEFINED_uint_least64_t
#endif

#if defined(__NEED_int_fast8_t) && !defined(__DEFINED_int_fast8_t)
typedef __int_fast8_t int_fast8_t;
#define __DEFINED_int_fast8_t
#endif

#if defined(__NEED_uint_fast8_t) && !defined(__DEFINED_uint_fast8_t)
typedef __uint_fast8_t uint_fast8_t;
#define __DEFINED_uint_fast8_t
#endif

#if defined(__NEED_int_fast16_t) && !defined(__DEFINED_int_fast16_t)
typedef __int_fast16_t int_fast16_t;
#define __DEFINED_int_fast16_t
#endif

#if defined(__NEED_uint_fast16_t) && !defined(__DEFINED_uint_fast16_t)
typedef __uint_fast16_t uint_fast16_t;
#define __DEFINED_uint_fast16_t
#endif

#if defined(__NEED_int_fast32_t) && !defined(__DEFINED_int_fast32_t)
typedef __int_fast32_t int_fast32_t;
#define __DEFINED_int_fast32_t
#endif

#if defined(__NEED_uint_fast32_t) && !defined(__DEFINED_uint_fast32_t)
typedef __uint_fast32_t uint_fast32_t;
#define __DEFINED_uint_fast32_t
#endif

#if defined(__NEED_int_fast64_t) && !defined(__DEFINED_int_fast64_t)
typedef __int_fast64_t int_fast64_t;
#define __DEFINED_int_fast64_t
#endif

#if defined(__NEED_uint_fast64_t) && !defined(__DEFINED_uint_fast64_t)
typedef __uint_fast64_t uint_fast64_t;
#define __DEFINED_uint_fast64_t
#endif

#if defined(__NEED_intptr_t) && !defined(__DEFINED_intptr_t)
typedef __intptr_t intptr_t;
#define __DEFINED_intptr_t
#endif

#if defined(__NEED_uintptr_t) && !defined(__DEFINED_uintptr_t)
typedef __uintptr_t uintptr_t;
#define __DEFINED_uintptr_t
#endif

#if defined(__NEED_intmax_t) && !defined(__DEFINED_intmax_t)
typedef __intmax_t intmax_t;
#define __DEFINED_intmax_t
#endif

#if defined(__NEED_uintmax_t) && !defined(__DEFINED_uintmax_t)
typedef __uintmax_t uintmax_t;
#define __DEFINED_uintmax_t
#endif

#undef __NEED_size_t
#undef __NEED_ssize_t
#undef __NEED_off_t
#undef __NEED_off64_t
#undef __NEED_mode_t
#undef __NEED_pid_t
#undef __NEED_uid_t
#undef __NEED_gid_t
#undef __NEED_time_t
#undef __NEED_suseconds_t
#undef __NEED_useconds_t
#undef __NEED_ino_t
#undef __NEED_ino64_t
#undef __NEED_dev_t
#undef __NEED_nlink_t
#undef __NEED_blksize_t
#undef __NEED_blkcnt_t
#undef __NEED_blkcnt64_t
#undef __NEED_fsblkcnt_t
#undef __NEED_fsfilcnt_t
#undef __NEED_clock_t
#undef __NEED_clockid_t
#undef __NEED_timer_t
#undef __NEED_key_t
#undef __NEED_id_t
#undef __NEED_daddr_t
#undef __NEED_caddr_t
#undef __NEED_sa_family_t
#undef __NEED_socklen_t
#undef __NEED_locale_t
#undef __NEED_int8_t
#undef __NEED_uint8_t
#undef __NEED_int16_t
#undef __NEED_uint16_t
#undef __NEED_int32_t
#undef __NEED_uint32_t
#undef __NEED_int64_t
#undef __NEED_uint64_t
#undef __NEED_int_least8_t
#undef __NEED_uint_least8_t
#undef __NEED_int_least16_t
#undef __NEED_uint_least16_t
#undef __NEED_int_least32_t
#undef __NEED_uint_least32_t
#undef __NEED_int_least64_t
#undef __NEED_uint_least64_t
#undef __NEED_int_fast8_t
#undef __NEED_uint_fast8_t
#undef __NEED_int_fast16_t
#undef __NEED_uint_fast16_t
#undef __NEED_int_fast32_t
#undef __NEED_uint_fast32_t
#undef __NEED_int_fast64_t
#undef __NEED_uint_fast64_t
#undef __NEED_intptr_t
#undef __NEED_uintptr_t
#undef __NEED_intmax_t
#undef __NEED_uintmax_t

#endif
