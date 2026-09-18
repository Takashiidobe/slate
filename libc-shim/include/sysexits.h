#ifndef _SLATE_SYSEXITS_H
#define _SLATE_SYSEXITS_H

enum {
  EX_OK          = 0,
  EX__BASE       = 64,
  EX_USAGE       = 64,
  EX_DATAERR     = 65,
  EX_NOINPUT     = 66,
  EX_NOUSER      = 67,
  EX_NOHOST      = 68,
  EX_UNAVAILABLE = 69,
  EX_SOFTWARE    = 70,
  EX_OSERR       = 71,
  EX_OSFILE      = 72,
  EX_CANTCREAT   = 73,
  EX_IOERR       = 74,
  EX_TEMPFAIL    = 75,
  EX_PROTOCOL    = 76,
  EX_NOPERM      = 77,
  EX_CONFIG      = 78,
  EX__MAX        = 78,
};

#define EX_OK 0
#define EX__BASE 64
#define EX_USAGE 64
#define EX_DATAERR 65
#define EX_NOINPUT 66
#define EX_NOUSER 67
#define EX_NOHOST 68
#define EX_UNAVAILABLE 69
#define EX_SOFTWARE 70
#define EX_OSERR 71
#define EX_OSFILE 72
#define EX_CANTCREAT 73
#define EX_IOERR 74
#define EX_TEMPFAIL 75
#define EX_PROTOCOL 76
#define EX_NOPERM 77
#define EX_CONFIG 78
#define EX__MAX 78

#endif
