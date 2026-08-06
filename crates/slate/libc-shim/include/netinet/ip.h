#ifndef _SLATE_NETINET_IP_H
#define _SLATE_NETINET_IP_H

#include <stdint.h>

struct timestamp {
  uint8_t len;
  uint8_t ptr;
#if __BYTE_ORDER == __LITTLE_ENDIAN
  unsigned int flags    : 4;
  unsigned int overflow : 4;
#else
  unsigned int overflow : 4;
  unsigned int flags    : 4;
#endif
  uint32_t data[9];
};

struct iphdr {
// bitfields require unsigned ints, otherwise this would be better uint8_t
// Older C requires all bitfields to be ints (unsigned/signed)
#if __BYTE_ORDER == __LITTLE_ENDIAN
  unsigned int ihl     : 4;
  unsigned int version : 4;
#else
  unsigned int version : 4;
  unsigned int ihl     : 4;
#endif
  uint8_t  tos;
  uint16_t tot_len;
  uint16_t id;
  uint16_t frag_off;
  uint8_t  ttl;
  uint8_t  protocol;
  uint16_t check;
  uint32_t saddr;
  uint32_t daddr;
};

#endif
