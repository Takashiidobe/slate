#ifndef _SLATE_SYS_STATFS_H
#define _SLATE_SYS_STATFS_H

#include <sys/statvfs.h>

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
