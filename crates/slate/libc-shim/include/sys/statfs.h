#ifndef _SLATE_SYS_STATFS_H
#define _SLATE_SYS_STATFS_H

#define __NEED_fsblkcnt_t
#define __NEED_fsfilcnt_t
#include <bits/types.h>

#if defined(__SLATE_LIBC_MUSL)
#include <sys/statvfs.h>
#endif

typedef struct __fsid_t {
  int __val[2];
} fsid_t;

#include <bits/statfs.h>

int statfs(const char *, struct statfs *);
int fstatfs(int, struct statfs *);

#if defined(_LARGEFILE64_SOURCE)
#define statfs64     statfs
#define fstatfs64    fstatfs
#define fsblkcnt64_t fsblkcnt_t
#define fsfilcnt64_t fsfilcnt_t
#endif

#endif
