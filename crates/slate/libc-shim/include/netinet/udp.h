#ifndef _SLATE_NETINET_UDP_H
#define _SLATE_NETINET_UDP_H

#include <features.h>

#define __NEED_uint16_t
#include <bits/types.h>

struct udphdr {
  uint16_t uh_sport;
  uint16_t uh_dport;
  uint16_t uh_ulen;
  uint16_t uh_sum;
};

enum {
  UDP_CORK         = 1,
  UDP_ENCAP        = 100,
  UDP_NO_CHECK6_TX = 101,
  UDP_NO_CHECK6_RX = 102,
  UDP_SEGMENT      = 103,
  UDP_GRO          = 104,
};

enum {
  UDP_ENCAP_ESPINUDP_NON_IKE = 1,
  UDP_ENCAP_ESPINUDP         = 2,
  UDP_ENCAP_L2TPINUDP        = 3,
  UDP_ENCAP_GTP0             = 4,
  UDP_ENCAP_GTP1U            = 5,
  UDP_ENCAP_RXRPC            = 6,
  TCP_ENCAP_ESPINTCP         = 7,
};

enum {
  SOL_UDP = 17,
};

#ifdef _GNU_SOURCE
#define uh_sport source
#define uh_dport dest
#define uh_ulen  len
#define uh_sum   check
#endif

#endif
