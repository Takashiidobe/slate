#ifndef _SLATE_BITS_MSVC_SYS_UTIME_H
#define _SLATE_BITS_MSVC_SYS_UTIME_H

#include <bits/msvc/types.h>

struct _utimbuf {
  time_t actime;
  time_t modtime;
};

struct __utimbuf32 {
  __time32_t actime;
  __time32_t modtime;
};

struct __utimbuf64 {
  __time64_t actime;
  __time64_t modtime;
};

int _utime32(const char *, struct __utimbuf32 *);
int _futime32(int, struct __utimbuf32 *);
int _wutime32(const wchar_t *, struct __utimbuf32 *);
int _utime64(const char *, struct __utimbuf64 *);
int _futime64(int, struct __utimbuf64 *);
int _wutime64(const wchar_t *, struct __utimbuf64 *);

#endif
