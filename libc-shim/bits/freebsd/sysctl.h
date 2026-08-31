#ifndef _SLATE_BITS_FREEBSD_SYSCTL_H
#define _SLATE_BITS_FREEBSD_SYSCTL_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/sysctl.h> directly; include a public header instead."
#endif

#define CTL_MAXNAME 24

#define CTL_SYSCTL  0
#define CTL_KERN    1
#define CTL_VM      2
#define CTL_VFS     3
#define CTL_NET     4
#define CTL_DEBUG   5
#define CTL_HW      6
#define CTL_MACHDEP 7

#define KERN_OSTYPE     1
#define KERN_OSRELEASE  2
#define KERN_OSREV      3
#define KERN_VERSION    4
#define KERN_HOSTNAME   10
#define KERN_HOSTID     11
#define KERN_PROC       14

#define KERN_PROC_ALL      0
#define KERN_PROC_PID      1
#define KERN_PROC_PGRP     2
#define KERN_PROC_SESSION  3
#define KERN_PROC_TTY      4
#define KERN_PROC_UID      5
#define KERN_PROC_RUID     6
#define KERN_PROC_ARGS     7
#define KERN_PROC_PROC     8
#define KERN_PROC_PATHNAME 12

#define HW_MACHINE  1
#define HW_MODEL    2
#define HW_NCPU     3
#define HW_PHYSMEM  5

int sysctl(const int *, unsigned int, void *, size_t *, const void *, size_t);
int sysctlbyname(const char *, void *, size_t *, const void *, size_t);
int sysctlnametomib(const char *, int *, size_t *);

#endif
