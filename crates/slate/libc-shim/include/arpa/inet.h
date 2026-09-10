#ifndef _SLATE_ARPA_INET_H
#define _SLATE_ARPA_INET_H

#include <features.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#if defined(__SLATE_LIBC_MUSL)
#include <inttypes.h>
#endif
#if defined(__SLATE_LIBC_GLIBC)
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#endif

#if defined(__SLATE_LIBC_GLIBC)
#define __NEED_struct_osockaddr
#endif
#define __NEED_uint16_t
#define __NEED_uint32_t
#include <bits/types.h>

uint32_t htonl(uint32_t);
uint16_t htons(uint16_t);
uint32_t ntohl(uint32_t);
uint16_t ntohs(uint16_t);

in_addr_t   inet_addr(const char *);
in_addr_t   inet_network(const char *);
char       *inet_ntoa(struct in_addr);
int         inet_pton(int, const char *__restrict, void *__restrict);
const char *inet_ntop(int, const void *__restrict, char *__restrict, socklen_t);

int            inet_aton(const char *, struct in_addr *);
struct in_addr inet_makeaddr(in_addr_t, in_addr_t);
in_addr_t      inet_lnaof(struct in_addr);
in_addr_t      inet_netof(struct in_addr);

#endif
