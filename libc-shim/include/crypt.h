#ifndef _SLATE_CRYPT_H
#define _SLATE_CRYPT_H

struct crypt_data {
  int  initialized;
  char __buf[256];
};

char *crypt(const char *, const char *);
char *crypt_r(const char *, const char *, struct crypt_data *);

#endif
