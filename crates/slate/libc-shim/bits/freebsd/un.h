#ifndef _SLATE_BITS_FREEBSD_UN_H
#define _SLATE_BITS_FREEBSD_UN_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/un.h> directly; include a public header instead."
#endif

#define SUNPATHLEN 104

struct sockaddr_un {
  unsigned char sun_len;
  sa_family_t   sun_family;
  char          sun_path[SUNPATHLEN];
};

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
size_t strlen(const char *);
#define SUN_LEN(s) (2 + strlen((s)->sun_path))
#endif

#endif
