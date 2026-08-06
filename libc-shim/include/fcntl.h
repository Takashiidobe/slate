#ifndef _SLATE_FCNTL_H
#define _SLATE_FCNTL_H

#include <features.h>

#include <sys/types.h>

enum {
  O_RDONLY   = 0,
  O_WRONLY   = 1,
  O_RDWR     = 2,
  O_CREAT    = 0100,
  O_TRUNC    = 01000,
  O_APPEND   = 02000,
  O_NONBLOCK = 04000,
  O_CLOEXEC  = 02000000,
};

enum {
  FD_CLOEXEC = 1,
};

enum {
  F_DUPFD         = 0,
  F_GETFD         = 1,
  F_SETFD         = 2,
  F_GETFL         = 3,
  F_SETFL         = 4,
  F_GETLK         = 5,
  F_SETLK         = 6,
  F_SETLKW        = 7,
  F_SETOWN        = 8,
  F_GETOWN        = 9,
  F_SETSIG        = 10,
  F_GETSIG        = 11,
  F_SETOWN_EX     = 15,
  F_GETOWN_EX     = 16,
  F_SETLEASE      = 1024,
  F_GETLEASE      = 1025,
  F_NOTIFY        = 1026,
  F_DUPFD_CLOEXEC = 1030,
  F_SETPIPE_SZ    = 1031,
  F_GETPIPE_SZ    = 1032,
  F_ADD_SEALS     = 1033,
  F_GET_SEALS     = 1034,
};

int fcntl(int fd, int cmd, ...);
int open(const char *pathname, int flags, ...);
int creat(const char *pathname, mode_t mode);

#endif
