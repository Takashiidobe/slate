#ifndef _SLATE_BITS_DARWIN_UN_H
#define _SLATE_BITS_DARWIN_UN_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/un.h> directly; include a public header instead."
#endif

#define SUNPATHLEN 104

struct sockaddr_un {
  unsigned char sun_len;
  sa_family_t   sun_family;
  char          sun_path[SUNPATHLEN];
};

#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
size_t strlen(const char *);
#define SUN_LEN(s) (sizeof(*(s)) - sizeof((s)->sun_path) + strlen((s)->sun_path))
#endif

#endif
