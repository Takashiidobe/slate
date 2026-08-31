#ifndef _SLATE_BITS_FREEBSD_FCNTL_H
#define _SLATE_BITS_FREEBSD_FCNTL_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/fcntl.h> directly; include a public header instead."
#endif

enum {
  O_RDONLY   = 0x0000,
  O_WRONLY   = 0x0001,
  O_RDWR     = 0x0002,
  O_NONBLOCK = 0x0004,
  O_APPEND   = 0x0008,
  O_SHLOCK   = 0x0010,
  O_EXLOCK   = 0x0020,
  O_ASYNC    = 0x0040,
  O_SYNC     = 0x0080,
  O_NOFOLLOW = 0x0100,
  O_CREAT    = 0x0200,
  O_TRUNC    = 0x0400,
  O_EXCL     = 0x0800,
  O_NOCTTY   = 0x8000,
};

#define O_ACCMODE 0x0003
#define O_FSYNC   O_SYNC
#define O_NDELAY  O_NONBLOCK

#define O_DIRECT    0x00010000
#define O_DIRECTORY 0x00020000
#define O_EXEC      0x00040000
#define O_SEARCH    O_EXEC
#define O_CLOEXEC   0x00100000
#define O_PATH      0x00400000
#define O_DSYNC     0x01000000
#define O_EMPTY_PATH 0x02000000

enum {
  F_DUPFD         = 0,
  F_GETFD         = 1,
  F_SETFD         = 2,
  F_GETFL         = 3,
  F_SETFL         = 4,
  F_GETOWN        = 5,
  F_SETOWN        = 6,
  F_DUP2FD        = 10,
  F_GETLK         = 11,
  F_SETLK         = 12,
  F_SETLKW        = 13,
  F_DUPFD_CLOEXEC = 17,
};

#define AT_FDCWD             (-100)
#define AT_EACCESS           0x0100
#define AT_SYMLINK_NOFOLLOW  0x0200
#define AT_SYMLINK_FOLLOW    0x0400
#define AT_REMOVEDIR         0x0800
#define AT_RESOLVE_BENEATH   0x2000
#define AT_EMPTY_PATH        0x4000

#define F_RDLCK 1
#define F_UNLCK 2
#define F_WRLCK 3

#define FD_CLOEXEC 1

#endif
