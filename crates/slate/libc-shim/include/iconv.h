#ifndef _SLATE_ICONV_H
#define _SLATE_ICONV_H

#include <features.h>

#define __NEED_size_t
#include <bits/types.h>

typedef void *iconv_t;

iconv_t iconv_open(const char *, const char *);
size_t  iconv(iconv_t, char **__restrict, size_t *__restrict, char **__restrict,
              size_t *__restrict);
int     iconv_close(iconv_t);

#endif
