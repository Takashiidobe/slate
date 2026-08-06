#ifndef _SLATE_SYS_XATTR_H
#define _SLATE_SYS_XATTR_H

#include <features.h>

#define __NEED_ssize_t
#define __NEED_size_t
#include <bits/types.h>

enum {
  XATTR_CREATE  = 1,
  XATTR_REPLACE = 2,
};

ssize_t getxattr(const char *, const char *, void *, size_t);
ssize_t lgetxattr(const char *, const char *, void *, size_t);
ssize_t fgetxattr(int filedes, const char *, void *, size_t);
ssize_t listxattr(const char *, char *, size_t);
ssize_t llistxattr(const char *, char *, size_t);
ssize_t flistxattr(int filedes, char *, size_t);
int     setxattr(const char *, const char *, const void *, size_t, int);
int     lsetxattr(const char *, const char *, const void *, size_t, int);
int     fsetxattr(int, const char *, const void *, size_t, int);
int     removexattr(const char *, const char *);
int     lremovexattr(const char *, const char *);
int     fremovexattr(int, const char *);

enum { __UAPI_DEF_XATTR = 0 };

#endif
