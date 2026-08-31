#ifndef _SLATE_SYS_UN_H
#define _SLATE_SYS_UN_H

#include <features.h>
#include <sys/types.h>

#define __NEED_sa_family_t
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_size_t
#endif

#include <bits/types.h>

#if defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/un.h>

#else

struct sockaddr_un {
  sa_family_t sun_family;
  char        sun_path[108];
};

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
size_t strlen(const char *);
#define SUN_LEN(s) (2 + strlen((s)->sun_path))
#endif

#endif 

#endif
