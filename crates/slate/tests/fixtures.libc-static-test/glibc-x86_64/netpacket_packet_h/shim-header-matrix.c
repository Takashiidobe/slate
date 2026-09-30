#include <netpacket/packet.h>

_Static_assert(sizeof(struct sockaddr_ll) == 20, "struct sockaddr_ll size differs from oracle");

_Static_assert(_Alignof(struct sockaddr_ll) == 4, "struct sockaddr_ll alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ll, sll_family) == 0, "struct sockaddr_ll.sll_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_ll_sll_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ll *)0)->sll_family), slate_oracle_struct_sockaddr_ll_sll_family), "struct sockaddr_ll.sll_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ll, sll_protocol) == 2, "struct sockaddr_ll.sll_protocol offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_ll_sll_protocol;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ll *)0)->sll_protocol), slate_oracle_struct_sockaddr_ll_sll_protocol), "struct sockaddr_ll.sll_protocol field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ll, sll_ifindex) == 4, "struct sockaddr_ll.sll_ifindex offset differs from oracle");

typedef int slate_oracle_struct_sockaddr_ll_sll_ifindex;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ll *)0)->sll_ifindex), slate_oracle_struct_sockaddr_ll_sll_ifindex), "struct sockaddr_ll.sll_ifindex field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ll, sll_hatype) == 8, "struct sockaddr_ll.sll_hatype offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_ll_sll_hatype;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ll *)0)->sll_hatype), slate_oracle_struct_sockaddr_ll_sll_hatype), "struct sockaddr_ll.sll_hatype field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ll, sll_pkttype) == 10, "struct sockaddr_ll.sll_pkttype offset differs from oracle");

typedef unsigned char slate_oracle_struct_sockaddr_ll_sll_pkttype;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ll *)0)->sll_pkttype), slate_oracle_struct_sockaddr_ll_sll_pkttype), "struct sockaddr_ll.sll_pkttype field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ll, sll_halen) == 11, "struct sockaddr_ll.sll_halen offset differs from oracle");

typedef unsigned char slate_oracle_struct_sockaddr_ll_sll_halen;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ll *)0)->sll_halen), slate_oracle_struct_sockaddr_ll_sll_halen), "struct sockaddr_ll.sll_halen field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ll, sll_addr) == 12, "struct sockaddr_ll.sll_addr offset differs from oracle");

#ifndef PACKET_ADD_MEMBERSHIP
#error "netpacket/packet.h:PACKET_ADD_MEMBERSHIP macro is missing from libc-shim"
#endif

#ifndef PACKET_AUXDATA
#error "netpacket/packet.h:PACKET_AUXDATA macro is missing from libc-shim"
#endif

#ifndef PACKET_BROADCAST
#error "netpacket/packet.h:PACKET_BROADCAST macro is missing from libc-shim"
#endif

#ifndef PACKET_COPY_THRESH
#error "netpacket/packet.h:PACKET_COPY_THRESH macro is missing from libc-shim"
#endif

#ifndef PACKET_DROP_MEMBERSHIP
#error "netpacket/packet.h:PACKET_DROP_MEMBERSHIP macro is missing from libc-shim"
#endif

#ifndef PACKET_FANOUT
#error "netpacket/packet.h:PACKET_FANOUT macro is missing from libc-shim"
#endif

#ifndef PACKET_FANOUT_DATA
#error "netpacket/packet.h:PACKET_FANOUT_DATA macro is missing from libc-shim"
#endif

#ifndef PACKET_FASTROUTE
#error "netpacket/packet.h:PACKET_FASTROUTE macro is missing from libc-shim"
#endif

#ifndef PACKET_HDRLEN
#error "netpacket/packet.h:PACKET_HDRLEN macro is missing from libc-shim"
#endif

#ifndef PACKET_HOST
#error "netpacket/packet.h:PACKET_HOST macro is missing from libc-shim"
#endif

#ifndef PACKET_IGNORE_OUTGOING
#error "netpacket/packet.h:PACKET_IGNORE_OUTGOING macro is missing from libc-shim"
#endif

#ifndef PACKET_LOOPBACK
#error "netpacket/packet.h:PACKET_LOOPBACK macro is missing from libc-shim"
#endif

#ifndef PACKET_LOSS
#error "netpacket/packet.h:PACKET_LOSS macro is missing from libc-shim"
#endif

#ifndef PACKET_MR_ALLMULTI
#error "netpacket/packet.h:PACKET_MR_ALLMULTI macro is missing from libc-shim"
#endif

#ifndef PACKET_MR_MULTICAST
#error "netpacket/packet.h:PACKET_MR_MULTICAST macro is missing from libc-shim"
#endif

#ifndef PACKET_MR_PROMISC
#error "netpacket/packet.h:PACKET_MR_PROMISC macro is missing from libc-shim"
#endif

#ifndef PACKET_MR_UNICAST
#error "netpacket/packet.h:PACKET_MR_UNICAST macro is missing from libc-shim"
#endif

#ifndef PACKET_MULTICAST
#error "netpacket/packet.h:PACKET_MULTICAST macro is missing from libc-shim"
#endif

#ifndef PACKET_ORIGDEV
#error "netpacket/packet.h:PACKET_ORIGDEV macro is missing from libc-shim"
#endif

#ifndef PACKET_OTHERHOST
#error "netpacket/packet.h:PACKET_OTHERHOST macro is missing from libc-shim"
#endif

#ifndef PACKET_OUTGOING
#error "netpacket/packet.h:PACKET_OUTGOING macro is missing from libc-shim"
#endif

#ifndef PACKET_QDISC_BYPASS
#error "netpacket/packet.h:PACKET_QDISC_BYPASS macro is missing from libc-shim"
#endif

#ifndef PACKET_RECV_OUTPUT
#error "netpacket/packet.h:PACKET_RECV_OUTPUT macro is missing from libc-shim"
#endif

#ifndef PACKET_RESERVE
#error "netpacket/packet.h:PACKET_RESERVE macro is missing from libc-shim"
#endif

#ifndef PACKET_ROLLOVER_STATS
#error "netpacket/packet.h:PACKET_ROLLOVER_STATS macro is missing from libc-shim"
#endif

#ifndef PACKET_RX_RING
#error "netpacket/packet.h:PACKET_RX_RING macro is missing from libc-shim"
#endif

#ifndef PACKET_STATISTICS
#error "netpacket/packet.h:PACKET_STATISTICS macro is missing from libc-shim"
#endif

#ifndef PACKET_TIMESTAMP
#error "netpacket/packet.h:PACKET_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef PACKET_TX_HAS_OFF
#error "netpacket/packet.h:PACKET_TX_HAS_OFF macro is missing from libc-shim"
#endif

#ifndef PACKET_TX_RING
#error "netpacket/packet.h:PACKET_TX_RING macro is missing from libc-shim"
#endif

#ifndef PACKET_TX_TIMESTAMP
#error "netpacket/packet.h:PACKET_TX_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef PACKET_VERSION
#error "netpacket/packet.h:PACKET_VERSION macro is missing from libc-shim"
#endif

#ifndef PACKET_VNET_HDR
#error "netpacket/packet.h:PACKET_VNET_HDR macro is missing from libc-shim"
#endif

#ifndef PACKET_VNET_HDR_SZ
#error "netpacket/packet.h:PACKET_VNET_HDR_SZ macro is missing from libc-shim"
#endif

int main(void) { return 0; }
