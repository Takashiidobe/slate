#ifndef _SLATE_BITS_FREEBSD_IN_H
#define _SLATE_BITS_FREEBSD_IN_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/in.h> directly; include a public header instead."
#endif

typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

struct in_addr {
  in_addr_t s_addr;
};

struct sockaddr_in {
  uint8_t        sin_len;
  sa_family_t    sin_family;
  in_port_t      sin_port;
  struct in_addr sin_addr;
  char           sin_zero[8];
};

struct in6_addr {
  union {
    uint8_t  __s6_addr[16];
    uint16_t __s6_addr16[8];
    uint32_t __s6_addr32[4];
  } __in6_union;
};
#define s6_addr   __in6_union.__s6_addr
#define s6_addr16 __in6_union.__s6_addr16
#define s6_addr32 __in6_union.__s6_addr32

struct sockaddr_in6 {
  uint8_t         sin6_len;
  sa_family_t     sin6_family;
  in_port_t       sin6_port;
  uint32_t        sin6_flowinfo;
  struct in6_addr sin6_addr;
  uint32_t        sin6_scope_id;
};

struct ip_mreq {
  struct in_addr imr_multiaddr;
  struct in_addr imr_interface;
};

struct ipv6_mreq {
  struct in6_addr ipv6mr_multiaddr;
  unsigned int    ipv6mr_interface;
};

#define INADDR_ANY       ((in_addr_t)0x00000000)
#define INADDR_BROADCAST ((in_addr_t)0xffffffff)
#define INADDR_NONE      ((in_addr_t)0xffffffff)
#define INADDR_LOOPBACK  ((in_addr_t)0x7f000001)

#define IN6ADDR_ANY_INIT                                                      \
  {                                                                           \
    { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }                    \
  }
#define IN6ADDR_LOOPBACK_INIT                                                 \
  {                                                                           \
    { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } }                    \
  }

extern const struct in6_addr in6addr_any, in6addr_loopback;

#define INET_ADDRSTRLEN  16
#define INET6_ADDRSTRLEN 46

uint32_t htonl(uint32_t);
uint16_t htons(uint16_t);
uint32_t ntohl(uint32_t);
uint16_t ntohs(uint16_t);

#define IPPORT_RESERVED 1024

#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_IGMP    2
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17
#define IPPROTO_IPV6    41
#define IPPROTO_ICMPV6  58
#define IPPROTO_RAW     255

#define IP_OPTIONS         1
#define IP_HDRINCL         2
#define IP_TOS             3
#define IP_TTL             4
#define IP_RECVDSTADDR     7
#define IP_MULTICAST_IF    9
#define IP_MULTICAST_TTL   10
#define IP_MULTICAST_LOOP  11
#define IP_ADD_MEMBERSHIP  12
#define IP_DROP_MEMBERSHIP 13

#define IPV6_UNICAST_HOPS   4
#define IPV6_MULTICAST_IF   9
#define IPV6_MULTICAST_HOPS 10
#define IPV6_MULTICAST_LOOP 11
#define IPV6_JOIN_GROUP     12
#define IPV6_LEAVE_GROUP    13
#define IPV6_V6ONLY         27

#define IPV6_ADD_MEMBERSHIP  IPV6_JOIN_GROUP
#define IPV6_DROP_MEMBERSHIP IPV6_LEAVE_GROUP

#define IN_LOOPBACKNET 127

#endif
