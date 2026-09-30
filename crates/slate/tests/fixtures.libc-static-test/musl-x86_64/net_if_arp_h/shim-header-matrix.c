#include <net/if_arp.h>

_Static_assert(sizeof(struct arphdr) == 8, "struct arphdr size differs from oracle");

_Static_assert(_Alignof(struct arphdr) == 2, "struct arphdr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct arphdr, ar_hrd) == 0, "struct arphdr.ar_hrd offset differs from oracle");

typedef unsigned short slate_oracle_struct_arphdr_ar_hrd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct arphdr *)0)->ar_hrd), slate_oracle_struct_arphdr_ar_hrd), "struct arphdr.ar_hrd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct arphdr, ar_pro) == 2, "struct arphdr.ar_pro offset differs from oracle");

typedef unsigned short slate_oracle_struct_arphdr_ar_pro;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct arphdr *)0)->ar_pro), slate_oracle_struct_arphdr_ar_pro), "struct arphdr.ar_pro field type differs from oracle");

_Static_assert(__builtin_offsetof(struct arphdr, ar_hln) == 4, "struct arphdr.ar_hln offset differs from oracle");

typedef unsigned char slate_oracle_struct_arphdr_ar_hln;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct arphdr *)0)->ar_hln), slate_oracle_struct_arphdr_ar_hln), "struct arphdr.ar_hln field type differs from oracle");

_Static_assert(__builtin_offsetof(struct arphdr, ar_pln) == 5, "struct arphdr.ar_pln offset differs from oracle");

typedef unsigned char slate_oracle_struct_arphdr_ar_pln;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct arphdr *)0)->ar_pln), slate_oracle_struct_arphdr_ar_pln), "struct arphdr.ar_pln field type differs from oracle");

_Static_assert(__builtin_offsetof(struct arphdr, ar_op) == 6, "struct arphdr.ar_op offset differs from oracle");

typedef unsigned short slate_oracle_struct_arphdr_ar_op;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct arphdr *)0)->ar_op), slate_oracle_struct_arphdr_ar_op), "struct arphdr.ar_op field type differs from oracle");

#ifndef ARPD_FLUSH
#error "net/if_arp.h:ARPD_FLUSH macro is missing from libc-shim"
#endif

#ifndef ARPD_LOOKUP
#error "net/if_arp.h:ARPD_LOOKUP macro is missing from libc-shim"
#endif

#ifndef ARPD_UPDATE
#error "net/if_arp.h:ARPD_UPDATE macro is missing from libc-shim"
#endif

#ifndef ARPHRD_6LOWPAN
#error "net/if_arp.h:ARPHRD_6LOWPAN macro is missing from libc-shim"
#endif

#ifndef ARPHRD_ADAPT
#error "net/if_arp.h:ARPHRD_ADAPT macro is missing from libc-shim"
#endif

#ifndef ARPHRD_APPLETLK
#error "net/if_arp.h:ARPHRD_APPLETLK macro is missing from libc-shim"
#endif

#ifndef ARPHRD_ARCNET
#error "net/if_arp.h:ARPHRD_ARCNET macro is missing from libc-shim"
#endif

#ifndef ARPHRD_ASH
#error "net/if_arp.h:ARPHRD_ASH macro is missing from libc-shim"
#endif

#ifndef ARPHRD_ATM
#error "net/if_arp.h:ARPHRD_ATM macro is missing from libc-shim"
#endif

#ifndef ARPHRD_AX25
#error "net/if_arp.h:ARPHRD_AX25 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_BIF
#error "net/if_arp.h:ARPHRD_BIF macro is missing from libc-shim"
#endif

#ifndef ARPHRD_CAIF
#error "net/if_arp.h:ARPHRD_CAIF macro is missing from libc-shim"
#endif

#ifndef ARPHRD_CAN
#error "net/if_arp.h:ARPHRD_CAN macro is missing from libc-shim"
#endif

#ifndef ARPHRD_CHAOS
#error "net/if_arp.h:ARPHRD_CHAOS macro is missing from libc-shim"
#endif

#ifndef ARPHRD_CISCO
#error "net/if_arp.h:ARPHRD_CISCO macro is missing from libc-shim"
#endif

#ifndef ARPHRD_CSLIP
#error "net/if_arp.h:ARPHRD_CSLIP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_CSLIP6
#error "net/if_arp.h:ARPHRD_CSLIP6 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_DDCMP
#error "net/if_arp.h:ARPHRD_DDCMP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_DLCI
#error "net/if_arp.h:ARPHRD_DLCI macro is missing from libc-shim"
#endif

#ifndef ARPHRD_ECONET
#error "net/if_arp.h:ARPHRD_ECONET macro is missing from libc-shim"
#endif

#ifndef ARPHRD_EETHER
#error "net/if_arp.h:ARPHRD_EETHER macro is missing from libc-shim"
#endif

#ifndef ARPHRD_ETHER
#error "net/if_arp.h:ARPHRD_ETHER macro is missing from libc-shim"
#endif

#ifndef ARPHRD_EUI64
#error "net/if_arp.h:ARPHRD_EUI64 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_FCAL
#error "net/if_arp.h:ARPHRD_FCAL macro is missing from libc-shim"
#endif

#ifndef ARPHRD_FCFABRIC
#error "net/if_arp.h:ARPHRD_FCFABRIC macro is missing from libc-shim"
#endif

#ifndef ARPHRD_FCPL
#error "net/if_arp.h:ARPHRD_FCPL macro is missing from libc-shim"
#endif

#ifndef ARPHRD_FCPP
#error "net/if_arp.h:ARPHRD_FCPP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_FDDI
#error "net/if_arp.h:ARPHRD_FDDI macro is missing from libc-shim"
#endif

#ifndef ARPHRD_FRAD
#error "net/if_arp.h:ARPHRD_FRAD macro is missing from libc-shim"
#endif

#ifndef ARPHRD_HDLC
#error "net/if_arp.h:ARPHRD_HDLC macro is missing from libc-shim"
#endif

#ifndef ARPHRD_HIPPI
#error "net/if_arp.h:ARPHRD_HIPPI macro is missing from libc-shim"
#endif

#ifndef ARPHRD_HWX25
#error "net/if_arp.h:ARPHRD_HWX25 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IEEE1394
#error "net/if_arp.h:ARPHRD_IEEE1394 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IEEE802
#error "net/if_arp.h:ARPHRD_IEEE802 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IEEE80211
#error "net/if_arp.h:ARPHRD_IEEE80211 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IEEE80211_PRISM
#error "net/if_arp.h:ARPHRD_IEEE80211_PRISM macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IEEE80211_RADIOTAP
#error "net/if_arp.h:ARPHRD_IEEE80211_RADIOTAP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IEEE802154
#error "net/if_arp.h:ARPHRD_IEEE802154 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IEEE802154_MONITOR
#error "net/if_arp.h:ARPHRD_IEEE802154_MONITOR macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IEEE802_TR
#error "net/if_arp.h:ARPHRD_IEEE802_TR macro is missing from libc-shim"
#endif

#ifndef ARPHRD_INFINIBAND
#error "net/if_arp.h:ARPHRD_INFINIBAND macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IP6GRE
#error "net/if_arp.h:ARPHRD_IP6GRE macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IPDDP
#error "net/if_arp.h:ARPHRD_IPDDP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IPGRE
#error "net/if_arp.h:ARPHRD_IPGRE macro is missing from libc-shim"
#endif

#ifndef ARPHRD_IRDA
#error "net/if_arp.h:ARPHRD_IRDA macro is missing from libc-shim"
#endif

#ifndef ARPHRD_LAPB
#error "net/if_arp.h:ARPHRD_LAPB macro is missing from libc-shim"
#endif

#ifndef ARPHRD_LOCALTLK
#error "net/if_arp.h:ARPHRD_LOCALTLK macro is missing from libc-shim"
#endif

#ifndef ARPHRD_LOOPBACK
#error "net/if_arp.h:ARPHRD_LOOPBACK macro is missing from libc-shim"
#endif

#ifndef ARPHRD_METRICOM
#error "net/if_arp.h:ARPHRD_METRICOM macro is missing from libc-shim"
#endif

#ifndef ARPHRD_NETLINK
#error "net/if_arp.h:ARPHRD_NETLINK macro is missing from libc-shim"
#endif

#ifndef ARPHRD_NETROM
#error "net/if_arp.h:ARPHRD_NETROM macro is missing from libc-shim"
#endif

#ifndef ARPHRD_NONE
#error "net/if_arp.h:ARPHRD_NONE macro is missing from libc-shim"
#endif

#ifndef ARPHRD_PHONET
#error "net/if_arp.h:ARPHRD_PHONET macro is missing from libc-shim"
#endif

#ifndef ARPHRD_PHONET_PIPE
#error "net/if_arp.h:ARPHRD_PHONET_PIPE macro is missing from libc-shim"
#endif

#ifndef ARPHRD_PIMREG
#error "net/if_arp.h:ARPHRD_PIMREG macro is missing from libc-shim"
#endif

#ifndef ARPHRD_PPP
#error "net/if_arp.h:ARPHRD_PPP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_PRONET
#error "net/if_arp.h:ARPHRD_PRONET macro is missing from libc-shim"
#endif

#ifndef ARPHRD_RAWHDLC
#error "net/if_arp.h:ARPHRD_RAWHDLC macro is missing from libc-shim"
#endif

#ifndef ARPHRD_RAWIP
#error "net/if_arp.h:ARPHRD_RAWIP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_ROSE
#error "net/if_arp.h:ARPHRD_ROSE macro is missing from libc-shim"
#endif

#ifndef ARPHRD_RSRVD
#error "net/if_arp.h:ARPHRD_RSRVD macro is missing from libc-shim"
#endif

#ifndef ARPHRD_SIT
#error "net/if_arp.h:ARPHRD_SIT macro is missing from libc-shim"
#endif

#ifndef ARPHRD_SKIP
#error "net/if_arp.h:ARPHRD_SKIP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_SLIP
#error "net/if_arp.h:ARPHRD_SLIP macro is missing from libc-shim"
#endif

#ifndef ARPHRD_SLIP6
#error "net/if_arp.h:ARPHRD_SLIP6 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_TUNNEL
#error "net/if_arp.h:ARPHRD_TUNNEL macro is missing from libc-shim"
#endif

#ifndef ARPHRD_TUNNEL6
#error "net/if_arp.h:ARPHRD_TUNNEL6 macro is missing from libc-shim"
#endif

#ifndef ARPHRD_VOID
#error "net/if_arp.h:ARPHRD_VOID macro is missing from libc-shim"
#endif

#ifndef ARPHRD_VSOCKMON
#error "net/if_arp.h:ARPHRD_VSOCKMON macro is missing from libc-shim"
#endif

#ifndef ARPHRD_X25
#error "net/if_arp.h:ARPHRD_X25 macro is missing from libc-shim"
#endif

#ifndef ARPOP_InREPLY
#error "net/if_arp.h:ARPOP_InREPLY macro is missing from libc-shim"
#endif

#ifndef ARPOP_InREQUEST
#error "net/if_arp.h:ARPOP_InREQUEST macro is missing from libc-shim"
#endif

#ifndef ARPOP_NAK
#error "net/if_arp.h:ARPOP_NAK macro is missing from libc-shim"
#endif

#ifndef ARPOP_REPLY
#error "net/if_arp.h:ARPOP_REPLY macro is missing from libc-shim"
#endif

#ifndef ARPOP_REQUEST
#error "net/if_arp.h:ARPOP_REQUEST macro is missing from libc-shim"
#endif

#ifndef ARPOP_RREPLY
#error "net/if_arp.h:ARPOP_RREPLY macro is missing from libc-shim"
#endif

#ifndef ARPOP_RREQUEST
#error "net/if_arp.h:ARPOP_RREQUEST macro is missing from libc-shim"
#endif

#ifndef ATF_COM
#error "net/if_arp.h:ATF_COM macro is missing from libc-shim"
#endif

#ifndef ATF_DONTPUB
#error "net/if_arp.h:ATF_DONTPUB macro is missing from libc-shim"
#endif

#ifndef ATF_MAGIC
#error "net/if_arp.h:ATF_MAGIC macro is missing from libc-shim"
#endif

#ifndef ATF_NETMASK
#error "net/if_arp.h:ATF_NETMASK macro is missing from libc-shim"
#endif

#ifndef ATF_PERM
#error "net/if_arp.h:ATF_PERM macro is missing from libc-shim"
#endif

#ifndef ATF_PUBL
#error "net/if_arp.h:ATF_PUBL macro is missing from libc-shim"
#endif

#ifndef ATF_USETRAILERS
#error "net/if_arp.h:ATF_USETRAILERS macro is missing from libc-shim"
#endif

#ifndef MAX_ADDR_LEN
#error "net/if_arp.h:MAX_ADDR_LEN macro is missing from libc-shim"
#endif

int main(void) { return 0; }
