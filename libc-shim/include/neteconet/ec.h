#ifndef _SLATE_NETECONET_EC_H
#define _SLATE_NETECONET_EC_H

#include <features.h>
#include <bits/sockaddr.h>

#if !defined(__SLATE_LIBC_GLIBC)
#error "<neteconet/ec.h> is unavailable for this libc profile."
#endif

struct ec_addr { unsigned char station; unsigned char net; };
struct __attribute__((__may_alias__)) sockaddr_ec {
  __SOCKADDR_COMMON(sec_);
  unsigned char port;
  unsigned char cb;
  unsigned char type;
  struct ec_addr addr;
  unsigned long cookie;
};

#define ECTYPE_PACKET_RECEIVED 0
#define ECTYPE_TRANSMIT_STATUS 0x10
#define ECTYPE_TRANSMIT_OK 1
#define ECTYPE_TRANSMIT_NOT_LISTENING 2
#define ECTYPE_TRANSMIT_NET_ERROR 3
#define ECTYPE_TRANSMIT_NO_CLOCK 4
#define ECTYPE_TRANSMIT_LINE_JAMMED 5
#define ECTYPE_TRANSMIT_NOT_PRESENT 6

#endif
