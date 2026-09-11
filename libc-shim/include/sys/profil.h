#ifndef _SLATE_SYS_PROFIL_H
#define _SLATE_SYS_PROFIL_H

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/time.h>
#include <sys/types.h>

struct prof {
  void         *pr_base;
  size_t        pr_size;
  size_t        pr_off;
  unsigned long pr_scale;
};

#define PROF_USHORT 0
#define PROF_UINT   (1 << 0)
#define PROF_FAST   (1 << 1)

int sprofil(struct prof *, int, struct timeval *, unsigned int);
#endif

#endif
