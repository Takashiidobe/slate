#ifndef _SLATE_BITS_DARWIN_SCHED_H
#define _SLATE_BITS_DARWIN_SCHED_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/sched.h> directly; include a public header instead."
#endif

struct sched_param {
  int  sched_priority;
  char __opaque[4];
};

#define SCHED_OTHER 1
#define SCHED_RR    2
#define SCHED_FIFO  4

int sched_yield(void);
int sched_get_priority_min(int);
int sched_get_priority_max(int);

#endif
