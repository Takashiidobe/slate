#include <net/ethernet.h>

_Static_assert(sizeof(struct ether_addr) == 6, "struct ether_addr size differs from oracle");

_Static_assert(_Alignof(struct ether_addr) == 1, "struct ether_addr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct ether_addr, ether_addr_octet) == 0, "struct ether_addr.ether_addr_octet offset differs from oracle");

#ifndef ETHERMIN
#error "net/ethernet.h:ETHERMIN macro is missing from libc-shim"
#endif

#ifndef ETHERMTU
#error "net/ethernet.h:ETHERMTU macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_AARP
#error "net/ethernet.h:ETHERTYPE_AARP macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_ARP
#error "net/ethernet.h:ETHERTYPE_ARP macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_AT
#error "net/ethernet.h:ETHERTYPE_AT macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_IP
#error "net/ethernet.h:ETHERTYPE_IP macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_IPV6
#error "net/ethernet.h:ETHERTYPE_IPV6 macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_IPX
#error "net/ethernet.h:ETHERTYPE_IPX macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_LOOPBACK
#error "net/ethernet.h:ETHERTYPE_LOOPBACK macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_NTRAILER
#error "net/ethernet.h:ETHERTYPE_NTRAILER macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_PUP
#error "net/ethernet.h:ETHERTYPE_PUP macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_REVARP
#error "net/ethernet.h:ETHERTYPE_REVARP macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_SPRITE
#error "net/ethernet.h:ETHERTYPE_SPRITE macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_TRAIL
#error "net/ethernet.h:ETHERTYPE_TRAIL macro is missing from libc-shim"
#endif

#ifndef ETHERTYPE_VLAN
#error "net/ethernet.h:ETHERTYPE_VLAN macro is missing from libc-shim"
#endif

#ifndef ETHER_ADDR_LEN
#error "net/ethernet.h:ETHER_ADDR_LEN macro is missing from libc-shim"
#endif

#ifndef ETHER_CRC_LEN
#error "net/ethernet.h:ETHER_CRC_LEN macro is missing from libc-shim"
#endif

#ifndef ETHER_HDR_LEN
#error "net/ethernet.h:ETHER_HDR_LEN macro is missing from libc-shim"
#endif

#ifndef ETHER_IS_VALID_LEN
#error "net/ethernet.h:ETHER_IS_VALID_LEN macro is missing from libc-shim"
#endif

#ifndef ETHER_MAX_LEN
#error "net/ethernet.h:ETHER_MAX_LEN macro is missing from libc-shim"
#endif

#ifndef ETHER_MIN_LEN
#error "net/ethernet.h:ETHER_MIN_LEN macro is missing from libc-shim"
#endif

#ifndef ETHER_TYPE_LEN
#error "net/ethernet.h:ETHER_TYPE_LEN macro is missing from libc-shim"
#endif

int main(void) { return 0; }
