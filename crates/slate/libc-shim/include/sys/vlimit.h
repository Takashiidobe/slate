#ifndef _SLATE_SYS_VLIMIT_H
#define _SLATE_SYS_VLIMIT_H

#if defined(__SLATE_LIBC_GLIBC)
enum __vlimit_resource {
  LIM_NORAISE,
  LIM_CPU,
  LIM_FSIZE,
  LIM_DATA,
  LIM_STACK,
  LIM_CORE,
  LIM_MAXRSS
};

#define INFINITY 0x7fffffff
int vlimit(enum __vlimit_resource, int);
#endif

#endif
