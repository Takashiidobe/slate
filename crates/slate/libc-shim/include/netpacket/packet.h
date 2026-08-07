#ifndef _SLATE_NETPACKET_PACKET_H
#define _SLATE_NETPACKET_PACKET_H

struct sockaddr_ll {
  unsigned short sll_family, sll_protocol;
  int            sll_ifindex;
  unsigned short sll_hatype;
  unsigned char  sll_pkttype, sll_halen;
  unsigned char  sll_addr[8];
};

struct packet_mreq {
  int                mr_ifindex;
  unsigned short int mr_type, mr_alen;
  unsigned char      mr_address[8];
};

enum {
  PACKET_HOST      = 0,
  PACKET_BROADCAST = 1,
  PACKET_MULTICAST = 2,
  PACKET_OTHERHOST = 3,
  PACKET_OUTGOING  = 4,
  PACKET_LOOPBACK  = 5,
  PACKET_FASTROUTE = 6,
};

enum {
  PACKET_ADD_MEMBERSHIP  = 1,
  PACKET_DROP_MEMBERSHIP = 2,
  PACKET_RECV_OUTPUT     = 3,
  PACKET_RX_RING         = 5,
  PACKET_STATISTICS      = 6,
  PACKET_COPY_THRESH     = 7,
  PACKET_AUXDATA         = 8,
  PACKET_ORIGDEV         = 9,
  PACKET_VERSION         = 10,
  PACKET_HDRLEN          = 11,
  PACKET_RESERVE         = 12,
  PACKET_TX_RING         = 13,
  PACKET_LOSS            = 14,
  PACKET_VNET_HDR        = 15,
  PACKET_TX_TIMESTAMP    = 16,
  PACKET_TIMESTAMP       = 17,
  PACKET_FANOUT          = 18,
  PACKET_TX_HAS_OFF      = 19,
  PACKET_QDISC_BYPASS    = 20,
  PACKET_ROLLOVER_STATS  = 21,
  PACKET_FANOUT_DATA     = 22,
  PACKET_IGNORE_OUTGOING = 23,
};

enum {
  PACKET_MR_MULTICAST = 0,
  PACKET_MR_PROMISC   = 1,
  PACKET_MR_ALLMULTI  = 2,
  PACKET_MR_UNICAST   = 3,
};

#endif
