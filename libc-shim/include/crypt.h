#ifndef _SLATE_CRYPT_H
#define _SLATE_CRYPT_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)

struct crypt_data {
  char output[384];
  char setting[384];
  char input[512];
  char reserved[767];
  char initialized;
  char internal[30720];
};

#else

struct crypt_data {
  int  initialized;
  char __buf[256];
};

#endif

char *crypt(const char *, const char *);
char *crypt_r(const char *, const char *, struct crypt_data *);

#endif
