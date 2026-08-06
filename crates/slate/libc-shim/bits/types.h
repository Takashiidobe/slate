#ifndef _SLATE_BITS_TYPES_H
#define _SLATE_BITS_TYPES_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/types.h> directly; include a standard header like <sys/types.h> or <unistd.h> instead."
#endif

typedef unsigned int __mode_t;
typedef int __pid_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long long __dev_t;
typedef unsigned int __nlink_t;
typedef unsigned int __useconds_t;
typedef int __clockid_t;
typedef void *__timer_t;
typedef long __clock_t;
typedef int __key_t;
typedef int __id_t;
typedef int __daddr_t;
typedef char *__caddr_t;
typedef unsigned char __sa_family_t;
typedef unsigned int __socklen_t;

#if defined(__LP64__)
typedef unsigned long __size_t;
typedef long __ssize_t;
typedef long __off_t;
typedef long __off64_t;
typedef long __time_t;
typedef long __suseconds_t;
typedef unsigned long __ino_t;
typedef unsigned long __ino64_t;
typedef long __blksize_t;
typedef long __blkcnt_t;
typedef long __blkcnt64_t;
typedef unsigned long __fsblkcnt_t;
typedef unsigned long __fsfilcnt_t;
#else
typedef unsigned int __size_t;
typedef int __ssize_t;
typedef long long __off_t;
typedef long long __off64_t;
typedef long __time_t;
typedef long __suseconds_t;
typedef unsigned long long __ino_t;
typedef unsigned long long __ino64_t;
typedef long __blksize_t;
typedef long long __blkcnt_t;
typedef long long __blkcnt64_t;
typedef unsigned long long __fsblkcnt_t;
typedef unsigned long long __fsfilcnt_t;
#endif

struct __locale_map;

struct __locale_struct {
  const struct __locale_map *cat[6];
};

typedef struct {
  int __count;
  union {
    int __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;

typedef struct __fpos_t {
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;

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

#if defined(__NEED_locale_t) && !defined(__defined_locale_t)
typedef struct __locale_struct *locale_t;
#define __DEFINED_locale_t
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

#endif
