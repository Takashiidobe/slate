#ifndef _SLATE_NET_IF_PACKET_H
#define _SLATE_NET_IF_PACKET_H

#include <features.h>
#include <bits/sockaddr.h>

struct __attribute_struct_may_alias__ sockaddr_pkt {
  __SOCKADDR_COMMON(spkt_);
  unsigned char spkt_device[14];
  unsigned short spkt_protocol;
};

#endif
