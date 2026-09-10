#ifndef _SLATE_SYS_FSUID_H
#define _SLATE_SYS_FSUID_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/types.h>
#endif

#define __NEED_uid_t
#define __NEED_gid_t
#include <bits/types.h>

int setfsuid(uid_t);
int setfsgid(gid_t);

#endif
