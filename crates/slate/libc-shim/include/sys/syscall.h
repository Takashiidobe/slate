#ifndef _SLATE_SYS_SYSCALL_H
#define _SLATE_SYS_SYSCALL_H

#include <bits/syscall.h>

#if defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_WORDSIZE_32)
#define SYS_clock_getres __NR_clock_getres
#define SYS_clock_gettime __NR_clock_gettime
#define SYS_clock_nanosleep __NR_clock_nanosleep
#define SYS_clock_settime __NR_clock_settime
#define SYS_gettimeofday __NR_gettimeofday
#define SYS_settimeofday __NR_settimeofday
#define SYS_timer_gettime __NR_timer_gettime
#define SYS_timer_settime __NR_timer_settime
#define SYS_timerfd_gettime __NR_timerfd_gettime
#define SYS_timerfd_settime __NR_timerfd_settime
#endif
#if defined(__SLATE_LIBC_GLIBC) && (defined(__SLATE_ARCH_X86) || defined(__SLATE_ARCH_X86_64))
#define SYS_rseq_slice_yield __NR_rseq_slice_yield
#endif

#endif
