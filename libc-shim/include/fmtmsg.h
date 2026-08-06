#ifndef _SLATE_FMTMSG_H
#define _SLATE_FMTMSG_H

enum {
  MM_HARD = 1,
  MM_SOFT = 2,
  MM_FIRM = 4,

  MM_APPL    = 8,
  MM_UTIL    = 16,
  MM_OPSYS   = 32,
  MM_RECOVER = 64,
  MM_NRECOV  = 128,
  MM_PRINT   = 256,
  MM_CONSOLE = 512,
};

#define MM_NULLMC 0L

enum {
  MM_NOSEV   = 0,
  MM_HALT    = 1,
  MM_ERROR   = 2,
  MM_WARNING = 3,
  MM_INFO    = 4,
};

#define MM_OK    0
#define MM_NOTOK (-1)
#define MM_NOMSG 1
#define MM_NOCON 4

#define MM_NULLLBL ((char *)0)
#define MM_NULLTXT ((char *)0)
#define MM_NULLACT ((char *)0)
#define MM_NULLTAG ((char *)0)
#define MM_NULLSEV 0

int fmtmsg(long, const char *, int, const char *, const char *, const char *);

#endif
