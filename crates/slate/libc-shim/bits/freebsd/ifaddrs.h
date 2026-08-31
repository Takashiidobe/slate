#ifndef _SLATE_BITS_FREEBSD_IFADDRS_H
#define _SLATE_BITS_FREEBSD_IFADDRS_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/ifaddrs.h> directly; include a public header instead."
#endif

struct ifaddrs {
  struct ifaddrs  *ifa_next;
  char            *ifa_name;
  unsigned int     ifa_flags;
  struct sockaddr *ifa_addr;
  struct sockaddr *ifa_netmask;
  struct sockaddr *ifa_dstaddr;
  void            *ifa_data;
};
#define ifa_broadaddr ifa_dstaddr

void freeifaddrs(struct ifaddrs *);
int  getifaddrs(struct ifaddrs **);

#endif
