#ifndef _SLATE_SYS_UTSNAME_H
#define _SLATE_SYS_UTSNAME_H

#include <features.h>

struct utsname {
  char sysname[65];
  char nodename[65];
  char release[65];
  char version[65];
  char machine[65];
#ifdef _GNU_SOURCE
  char domainname[65];
#else
  char __domainname[65];
#endif
};

int uname(struct utsname *);

#endif
