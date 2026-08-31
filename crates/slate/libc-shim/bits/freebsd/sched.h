#ifndef _SLATE_BITS_FREEBSD_SCHED_H
#define _SLATE_BITS_FREEBSD_SCHED_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/sched.h> directly; include a public header instead."
#endif

#define __NEED_pid_t
#include <bits/types.h>

struct sched_param {
  int sched_priority;
};

#define SCHED_FIFO  1
#define SCHED_OTHER 2
#define SCHED_RR    3

int sched_yield(void);
int sched_get_priority_min(int);
int sched_get_priority_max(int);
int sched_getparam(pid_t, struct sched_param *);
int sched_setparam(pid_t, const struct sched_param *);
int sched_getscheduler(pid_t);
int sched_setscheduler(pid_t, int, const struct sched_param *);
int sched_getcpu(void);

#endif
