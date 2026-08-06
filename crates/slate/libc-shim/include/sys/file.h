#ifndef _SLATE_SYS_FILE_H
#define _SLATE_SYS_FILE_H

enum {
  LOCK_SH = 1,
  LOCK_EX = 2,
  LOCK_NB = 4,
  LOCK_UN = 8,
};

enum {
  L_SET  = 0,
  L_INCR = 1,
  L_XTND = 2,
};

int flock(int, int);

#endif
