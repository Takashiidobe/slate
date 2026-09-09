#ifndef _SLATE_IFADDRS_H
#define _SLATE_IFADDRS_H

#include <features.h>
#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/ifaddrs.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/ifaddrs.h>

#else

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/types.h>
#include <sys/socket.h>
#elif defined(__SLATE_LIBC_MUSL)
#include <netinet/in.h>
#include <inttypes.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#endif

struct ifaddrs {
  struct ifaddrs  *ifa_next;
  char            *ifa_name;
  unsigned         ifa_flags;
  struct sockaddr *ifa_addr;
  struct sockaddr *ifa_netmask;
  union {
    struct sockaddr *ifu_broadaddr;
    struct sockaddr *ifu_dstaddr;
  } ifa_ifu;
  void *ifa_data;
};
#define ifa_broadaddr ifa_ifu.ifu_broadaddr
#define ifa_dstaddr   ifa_ifu.ifu_dstaddr

void freeifaddrs(struct ifaddrs *);
int  getifaddrs(struct ifaddrs **);

#endif 

#endif
