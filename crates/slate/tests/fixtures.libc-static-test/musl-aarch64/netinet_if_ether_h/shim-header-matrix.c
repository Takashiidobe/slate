#include <netinet/if_ether.h>

typedef struct arphdr slate_oracle_struct_ether_arp_ea_hdr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ether_arp *)0)->ea_hdr), slate_oracle_struct_ether_arp_ea_hdr), "struct ether_arp.ea_hdr field type differs from oracle");

_Static_assert(sizeof(struct ethhdr) == 14, "struct ethhdr size differs from oracle");

_Static_assert(_Alignof(struct ethhdr) == 2, "struct ethhdr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct ethhdr, h_dest) == 0, "struct ethhdr.h_dest offset differs from oracle");

_Static_assert(__builtin_offsetof(struct ethhdr, h_source) == 6, "struct ethhdr.h_source offset differs from oracle");

_Static_assert(__builtin_offsetof(struct ethhdr, h_proto) == 12, "struct ethhdr.h_proto offset differs from oracle");

typedef unsigned short slate_oracle_struct_ethhdr_h_proto;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ethhdr *)0)->h_proto), slate_oracle_struct_ethhdr_h_proto), "struct ethhdr.h_proto field type differs from oracle");

#ifndef ETHER_MAP_IP_MULTICAST
#error "netinet/if_ether.h:ETHER_MAP_IP_MULTICAST macro is missing from libc-shim"
#endif

#ifndef ETH_ALEN
#error "netinet/if_ether.h:ETH_ALEN macro is missing from libc-shim"
#endif

#ifndef ETH_DATA_LEN
#error "netinet/if_ether.h:ETH_DATA_LEN macro is missing from libc-shim"
#endif

#ifndef ETH_FCS_LEN
#error "netinet/if_ether.h:ETH_FCS_LEN macro is missing from libc-shim"
#endif

#ifndef ETH_FRAME_LEN
#error "netinet/if_ether.h:ETH_FRAME_LEN macro is missing from libc-shim"
#endif

#ifndef ETH_HLEN
#error "netinet/if_ether.h:ETH_HLEN macro is missing from libc-shim"
#endif

#ifndef ETH_MAX_MTU
#error "netinet/if_ether.h:ETH_MAX_MTU macro is missing from libc-shim"
#endif

#ifndef ETH_MIN_MTU
#error "netinet/if_ether.h:ETH_MIN_MTU macro is missing from libc-shim"
#endif

#ifndef ETH_P_1588
#error "netinet/if_ether.h:ETH_P_1588 macro is missing from libc-shim"
#endif

#ifndef ETH_P_8021AD
#error "netinet/if_ether.h:ETH_P_8021AD macro is missing from libc-shim"
#endif

#ifndef ETH_P_8021AH
#error "netinet/if_ether.h:ETH_P_8021AH macro is missing from libc-shim"
#endif

#ifndef ETH_P_8021Q
#error "netinet/if_ether.h:ETH_P_8021Q macro is missing from libc-shim"
#endif

#ifndef ETH_P_80221
#error "netinet/if_ether.h:ETH_P_80221 macro is missing from libc-shim"
#endif

#ifndef ETH_P_802_2
#error "netinet/if_ether.h:ETH_P_802_2 macro is missing from libc-shim"
#endif

#ifndef ETH_P_802_3
#error "netinet/if_ether.h:ETH_P_802_3 macro is missing from libc-shim"
#endif

#ifndef ETH_P_802_3_MIN
#error "netinet/if_ether.h:ETH_P_802_3_MIN macro is missing from libc-shim"
#endif

#ifndef ETH_P_802_EX1
#error "netinet/if_ether.h:ETH_P_802_EX1 macro is missing from libc-shim"
#endif

#ifndef ETH_P_AARP
#error "netinet/if_ether.h:ETH_P_AARP macro is missing from libc-shim"
#endif

#ifndef ETH_P_AF_IUCV
#error "netinet/if_ether.h:ETH_P_AF_IUCV macro is missing from libc-shim"
#endif

#ifndef ETH_P_ALL
#error "netinet/if_ether.h:ETH_P_ALL macro is missing from libc-shim"
#endif

#ifndef ETH_P_AOE
#error "netinet/if_ether.h:ETH_P_AOE macro is missing from libc-shim"
#endif

#ifndef ETH_P_ARCNET
#error "netinet/if_ether.h:ETH_P_ARCNET macro is missing from libc-shim"
#endif

#ifndef ETH_P_ARP
#error "netinet/if_ether.h:ETH_P_ARP macro is missing from libc-shim"
#endif

#ifndef ETH_P_ATALK
#error "netinet/if_ether.h:ETH_P_ATALK macro is missing from libc-shim"
#endif

#ifndef ETH_P_ATMFATE
#error "netinet/if_ether.h:ETH_P_ATMFATE macro is missing from libc-shim"
#endif

#ifndef ETH_P_ATMMPOA
#error "netinet/if_ether.h:ETH_P_ATMMPOA macro is missing from libc-shim"
#endif

#ifndef ETH_P_AX25
#error "netinet/if_ether.h:ETH_P_AX25 macro is missing from libc-shim"
#endif

#ifndef ETH_P_BATMAN
#error "netinet/if_ether.h:ETH_P_BATMAN macro is missing from libc-shim"
#endif

#ifndef ETH_P_BPQ
#error "netinet/if_ether.h:ETH_P_BPQ macro is missing from libc-shim"
#endif

#ifndef ETH_P_CAIF
#error "netinet/if_ether.h:ETH_P_CAIF macro is missing from libc-shim"
#endif

#ifndef ETH_P_CAN
#error "netinet/if_ether.h:ETH_P_CAN macro is missing from libc-shim"
#endif

#ifndef ETH_P_CANFD
#error "netinet/if_ether.h:ETH_P_CANFD macro is missing from libc-shim"
#endif

#ifndef ETH_P_CFM
#error "netinet/if_ether.h:ETH_P_CFM macro is missing from libc-shim"
#endif

#ifndef ETH_P_CONTROL
#error "netinet/if_ether.h:ETH_P_CONTROL macro is missing from libc-shim"
#endif

#ifndef ETH_P_CUST
#error "netinet/if_ether.h:ETH_P_CUST macro is missing from libc-shim"
#endif

#ifndef ETH_P_DDCMP
#error "netinet/if_ether.h:ETH_P_DDCMP macro is missing from libc-shim"
#endif

#ifndef ETH_P_DEC
#error "netinet/if_ether.h:ETH_P_DEC macro is missing from libc-shim"
#endif

#ifndef ETH_P_DIAG
#error "netinet/if_ether.h:ETH_P_DIAG macro is missing from libc-shim"
#endif

#ifndef ETH_P_DNA_DL
#error "netinet/if_ether.h:ETH_P_DNA_DL macro is missing from libc-shim"
#endif

#ifndef ETH_P_DNA_RC
#error "netinet/if_ether.h:ETH_P_DNA_RC macro is missing from libc-shim"
#endif

#ifndef ETH_P_DNA_RT
#error "netinet/if_ether.h:ETH_P_DNA_RT macro is missing from libc-shim"
#endif

#ifndef ETH_P_DSA
#error "netinet/if_ether.h:ETH_P_DSA macro is missing from libc-shim"
#endif

#ifndef ETH_P_DSA_8021Q
#error "netinet/if_ether.h:ETH_P_DSA_8021Q macro is missing from libc-shim"
#endif

#ifndef ETH_P_ECONET
#error "netinet/if_ether.h:ETH_P_ECONET macro is missing from libc-shim"
#endif

#ifndef ETH_P_EDSA
#error "netinet/if_ether.h:ETH_P_EDSA macro is missing from libc-shim"
#endif

#ifndef ETH_P_ERSPAN
#error "netinet/if_ether.h:ETH_P_ERSPAN macro is missing from libc-shim"
#endif

#ifndef ETH_P_ERSPAN2
#error "netinet/if_ether.h:ETH_P_ERSPAN2 macro is missing from libc-shim"
#endif

#ifndef ETH_P_FCOE
#error "netinet/if_ether.h:ETH_P_FCOE macro is missing from libc-shim"
#endif

#ifndef ETH_P_FIP
#error "netinet/if_ether.h:ETH_P_FIP macro is missing from libc-shim"
#endif

#ifndef ETH_P_HDLC
#error "netinet/if_ether.h:ETH_P_HDLC macro is missing from libc-shim"
#endif

#ifndef ETH_P_HSR
#error "netinet/if_ether.h:ETH_P_HSR macro is missing from libc-shim"
#endif

#ifndef ETH_P_IBOE
#error "netinet/if_ether.h:ETH_P_IBOE macro is missing from libc-shim"
#endif

#ifndef ETH_P_IEEE802154
#error "netinet/if_ether.h:ETH_P_IEEE802154 macro is missing from libc-shim"
#endif

#ifndef ETH_P_IEEEPUP
#error "netinet/if_ether.h:ETH_P_IEEEPUP macro is missing from libc-shim"
#endif

#ifndef ETH_P_IEEEPUPAT
#error "netinet/if_ether.h:ETH_P_IEEEPUPAT macro is missing from libc-shim"
#endif

#ifndef ETH_P_IFE
#error "netinet/if_ether.h:ETH_P_IFE macro is missing from libc-shim"
#endif

#ifndef ETH_P_IP
#error "netinet/if_ether.h:ETH_P_IP macro is missing from libc-shim"
#endif

#ifndef ETH_P_IPV6
#error "netinet/if_ether.h:ETH_P_IPV6 macro is missing from libc-shim"
#endif

#ifndef ETH_P_IPX
#error "netinet/if_ether.h:ETH_P_IPX macro is missing from libc-shim"
#endif

#ifndef ETH_P_IRDA
#error "netinet/if_ether.h:ETH_P_IRDA macro is missing from libc-shim"
#endif

#ifndef ETH_P_LAT
#error "netinet/if_ether.h:ETH_P_LAT macro is missing from libc-shim"
#endif

#ifndef ETH_P_LINK_CTL
#error "netinet/if_ether.h:ETH_P_LINK_CTL macro is missing from libc-shim"
#endif

#ifndef ETH_P_LLDP
#error "netinet/if_ether.h:ETH_P_LLDP macro is missing from libc-shim"
#endif

#ifndef ETH_P_LOCALTALK
#error "netinet/if_ether.h:ETH_P_LOCALTALK macro is missing from libc-shim"
#endif

#ifndef ETH_P_LOOP
#error "netinet/if_ether.h:ETH_P_LOOP macro is missing from libc-shim"
#endif

#ifndef ETH_P_LOOPBACK
#error "netinet/if_ether.h:ETH_P_LOOPBACK macro is missing from libc-shim"
#endif

#ifndef ETH_P_MACSEC
#error "netinet/if_ether.h:ETH_P_MACSEC macro is missing from libc-shim"
#endif

#ifndef ETH_P_MAP
#error "netinet/if_ether.h:ETH_P_MAP macro is missing from libc-shim"
#endif

#ifndef ETH_P_MOBITEX
#error "netinet/if_ether.h:ETH_P_MOBITEX macro is missing from libc-shim"
#endif

#ifndef ETH_P_MPLS_MC
#error "netinet/if_ether.h:ETH_P_MPLS_MC macro is missing from libc-shim"
#endif

#ifndef ETH_P_MPLS_UC
#error "netinet/if_ether.h:ETH_P_MPLS_UC macro is missing from libc-shim"
#endif

#ifndef ETH_P_MRP
#error "netinet/if_ether.h:ETH_P_MRP macro is missing from libc-shim"
#endif

#ifndef ETH_P_MVRP
#error "netinet/if_ether.h:ETH_P_MVRP macro is missing from libc-shim"
#endif

#ifndef ETH_P_NCSI
#error "netinet/if_ether.h:ETH_P_NCSI macro is missing from libc-shim"
#endif

#ifndef ETH_P_NSH
#error "netinet/if_ether.h:ETH_P_NSH macro is missing from libc-shim"
#endif

#ifndef ETH_P_PAE
#error "netinet/if_ether.h:ETH_P_PAE macro is missing from libc-shim"
#endif

#ifndef ETH_P_PAUSE
#error "netinet/if_ether.h:ETH_P_PAUSE macro is missing from libc-shim"
#endif

#ifndef ETH_P_PHONET
#error "netinet/if_ether.h:ETH_P_PHONET macro is missing from libc-shim"
#endif

#ifndef ETH_P_PPPTALK
#error "netinet/if_ether.h:ETH_P_PPPTALK macro is missing from libc-shim"
#endif

#ifndef ETH_P_PPP_DISC
#error "netinet/if_ether.h:ETH_P_PPP_DISC macro is missing from libc-shim"
#endif

#ifndef ETH_P_PPP_MP
#error "netinet/if_ether.h:ETH_P_PPP_MP macro is missing from libc-shim"
#endif

#ifndef ETH_P_PPP_SES
#error "netinet/if_ether.h:ETH_P_PPP_SES macro is missing from libc-shim"
#endif

#ifndef ETH_P_PREAUTH
#error "netinet/if_ether.h:ETH_P_PREAUTH macro is missing from libc-shim"
#endif

#ifndef ETH_P_PRP
#error "netinet/if_ether.h:ETH_P_PRP macro is missing from libc-shim"
#endif

#ifndef ETH_P_PUP
#error "netinet/if_ether.h:ETH_P_PUP macro is missing from libc-shim"
#endif

#ifndef ETH_P_PUPAT
#error "netinet/if_ether.h:ETH_P_PUPAT macro is missing from libc-shim"
#endif

#ifndef ETH_P_QINQ1
#error "netinet/if_ether.h:ETH_P_QINQ1 macro is missing from libc-shim"
#endif

#ifndef ETH_P_QINQ2
#error "netinet/if_ether.h:ETH_P_QINQ2 macro is missing from libc-shim"
#endif

#ifndef ETH_P_QINQ3
#error "netinet/if_ether.h:ETH_P_QINQ3 macro is missing from libc-shim"
#endif

#ifndef ETH_P_RARP
#error "netinet/if_ether.h:ETH_P_RARP macro is missing from libc-shim"
#endif

#ifndef ETH_P_SCA
#error "netinet/if_ether.h:ETH_P_SCA macro is missing from libc-shim"
#endif

#ifndef ETH_P_SLOW
#error "netinet/if_ether.h:ETH_P_SLOW macro is missing from libc-shim"
#endif

#ifndef ETH_P_SNAP
#error "netinet/if_ether.h:ETH_P_SNAP macro is missing from libc-shim"
#endif

#ifndef ETH_P_TDLS
#error "netinet/if_ether.h:ETH_P_TDLS macro is missing from libc-shim"
#endif

#ifndef ETH_P_TEB
#error "netinet/if_ether.h:ETH_P_TEB macro is missing from libc-shim"
#endif

#ifndef ETH_P_TIPC
#error "netinet/if_ether.h:ETH_P_TIPC macro is missing from libc-shim"
#endif

#ifndef ETH_P_TRAILER
#error "netinet/if_ether.h:ETH_P_TRAILER macro is missing from libc-shim"
#endif

#ifndef ETH_P_TR_802_2
#error "netinet/if_ether.h:ETH_P_TR_802_2 macro is missing from libc-shim"
#endif

#ifndef ETH_P_TSN
#error "netinet/if_ether.h:ETH_P_TSN macro is missing from libc-shim"
#endif

#ifndef ETH_P_WAN_PPP
#error "netinet/if_ether.h:ETH_P_WAN_PPP macro is missing from libc-shim"
#endif

#ifndef ETH_P_WCCP
#error "netinet/if_ether.h:ETH_P_WCCP macro is missing from libc-shim"
#endif

#ifndef ETH_P_X25
#error "netinet/if_ether.h:ETH_P_X25 macro is missing from libc-shim"
#endif

#ifndef ETH_P_XDSA
#error "netinet/if_ether.h:ETH_P_XDSA macro is missing from libc-shim"
#endif

#ifndef ETH_TLEN
#error "netinet/if_ether.h:ETH_TLEN macro is missing from libc-shim"
#endif

#ifndef ETH_ZLEN
#error "netinet/if_ether.h:ETH_ZLEN macro is missing from libc-shim"
#endif

#ifndef arp_hln
#error "netinet/if_ether.h:arp_hln macro is missing from libc-shim"
#endif

#ifndef arp_hrd
#error "netinet/if_ether.h:arp_hrd macro is missing from libc-shim"
#endif

#ifndef arp_op
#error "netinet/if_ether.h:arp_op macro is missing from libc-shim"
#endif

#ifndef arp_pln
#error "netinet/if_ether.h:arp_pln macro is missing from libc-shim"
#endif

#ifndef arp_pro
#error "netinet/if_ether.h:arp_pro macro is missing from libc-shim"
#endif

int main(void) { return 0; }
