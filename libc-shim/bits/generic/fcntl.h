#if !defined(_SLATE_LIBC)
#error "Never include <bits/generic/fcntl.h> directly; include a public header instead."
#endif

#define O_SEARCH   O_PATH
#define O_EXEC     O_PATH
#define O_TTY_INIT 0

#define O_EXCL      0200
#define O_NOCTTY    0400
#define O_DSYNC     010000
#define O_SYNC      04010000
#define O_RSYNC     04010000
#define O_DIRECTORY 0200000
#define O_NOFOLLOW  0400000
#define O_ASYNC     020000
#define O_DIRECT    040000
#define O_LARGEFILE 0100000
#define O_NOATIME   01000000
#define O_PATH      010000000
#define O_TMPFILE   020200000
#define O_NDELAY    O_NONBLOCK

#define O_ACCMODE (03 | O_SEARCH)
#define O_RDONLY  00
#define O_WRONLY  01
#define O_RDWR    02
