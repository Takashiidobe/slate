#ifndef _SLATE_BITS_SOCKADDR_H
#define _SLATE_BITS_SOCKADDR_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/sockaddr.h> directly; include <sys/socket.h> instead."
#endif

#include <features.h>

#if !defined(__SLATE_LIBC_GLIBC)
#error "<bits/sockaddr.h> is unavailable for this libc profile."
#endif

typedef unsigned short int sa_family_t;

#define __SOCKADDR_COMMON(sa_prefix) sa_family_t sa_prefix##family
#define __SOCKADDR_COMMON_SIZE (sizeof(unsigned short int))
#define _SS_SIZE 128

#endif
