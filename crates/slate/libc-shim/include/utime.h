#ifndef _SLATE_UTIME_H
#define _SLATE_UTIME_H

#include <features.h>

#define __NEED_time_t
#include <bits/types.h>

struct utimbuf {
  time_t actime;
  time_t modtime;
};

int utime(const char *, const struct utimbuf *);

#if _REDIR_TIME64
__REDIR(utime, __utime64);
#endif

#endif
