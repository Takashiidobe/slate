#include <net/if.h>

_Static_assert(sizeof(struct if_nameindex) == 8, "struct if_nameindex size differs from oracle");

_Static_assert(_Alignof(struct if_nameindex) == 4, "struct if_nameindex alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct if_nameindex, if_index) == 0, "struct if_nameindex.if_index offset differs from oracle");

typedef unsigned int slate_oracle_struct_if_nameindex_if_index;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct if_nameindex *)0)->if_index), slate_oracle_struct_if_nameindex_if_index), "struct if_nameindex.if_index field type differs from oracle");

_Static_assert(__builtin_offsetof(struct if_nameindex, if_name) == 4, "struct if_nameindex.if_name offset differs from oracle");

typedef char * slate_oracle_struct_if_nameindex_if_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct if_nameindex *)0)->if_name), slate_oracle_struct_if_nameindex_if_name), "struct if_nameindex.if_name field type differs from oracle");

#ifndef IFF_ALLMULTI
#error "net/if.h:IFF_ALLMULTI macro is missing from libc-shim"
#endif

#ifndef IFF_AUTOMEDIA
#error "net/if.h:IFF_AUTOMEDIA macro is missing from libc-shim"
#endif

#ifndef IFF_BROADCAST
#error "net/if.h:IFF_BROADCAST macro is missing from libc-shim"
#endif

#ifndef IFF_DEBUG
#error "net/if.h:IFF_DEBUG macro is missing from libc-shim"
#endif

#ifndef IFF_DYNAMIC
#error "net/if.h:IFF_DYNAMIC macro is missing from libc-shim"
#endif

#ifndef IFF_LOOPBACK
#error "net/if.h:IFF_LOOPBACK macro is missing from libc-shim"
#endif

#ifndef IFF_MASTER
#error "net/if.h:IFF_MASTER macro is missing from libc-shim"
#endif

#ifndef IFF_MULTICAST
#error "net/if.h:IFF_MULTICAST macro is missing from libc-shim"
#endif

#ifndef IFF_NOARP
#error "net/if.h:IFF_NOARP macro is missing from libc-shim"
#endif

#ifndef IFF_NOTRAILERS
#error "net/if.h:IFF_NOTRAILERS macro is missing from libc-shim"
#endif

#ifndef IFF_POINTOPOINT
#error "net/if.h:IFF_POINTOPOINT macro is missing from libc-shim"
#endif

#ifndef IFF_PORTSEL
#error "net/if.h:IFF_PORTSEL macro is missing from libc-shim"
#endif

#ifndef IFF_PROMISC
#error "net/if.h:IFF_PROMISC macro is missing from libc-shim"
#endif

#ifndef IFF_RUNNING
#error "net/if.h:IFF_RUNNING macro is missing from libc-shim"
#endif

#ifndef IFF_SLAVE
#error "net/if.h:IFF_SLAVE macro is missing from libc-shim"
#endif

#ifndef IFF_UP
#error "net/if.h:IFF_UP macro is missing from libc-shim"
#endif

#ifndef IFHWADDRLEN
#error "net/if.h:IFHWADDRLEN macro is missing from libc-shim"
#endif

#ifndef IFNAMSIZ
#error "net/if.h:IFNAMSIZ macro is missing from libc-shim"
#endif

#ifndef IF_NAMESIZE
#error "net/if.h:IF_NAMESIZE macro is missing from libc-shim"
#endif

#ifndef ifa_broadaddr
#error "net/if.h:ifa_broadaddr macro is missing from libc-shim"
#endif

#ifndef ifa_dstaddr
#error "net/if.h:ifa_dstaddr macro is missing from libc-shim"
#endif

#ifndef ifc_buf
#error "net/if.h:ifc_buf macro is missing from libc-shim"
#endif

#ifndef ifc_req
#error "net/if.h:ifc_req macro is missing from libc-shim"
#endif

#ifndef ifr_addr
#error "net/if.h:ifr_addr macro is missing from libc-shim"
#endif

#ifndef ifr_bandwidth
#error "net/if.h:ifr_bandwidth macro is missing from libc-shim"
#endif

#ifndef ifr_broadaddr
#error "net/if.h:ifr_broadaddr macro is missing from libc-shim"
#endif

#ifndef ifr_data
#error "net/if.h:ifr_data macro is missing from libc-shim"
#endif

#ifndef ifr_dstaddr
#error "net/if.h:ifr_dstaddr macro is missing from libc-shim"
#endif

#ifndef ifr_flags
#error "net/if.h:ifr_flags macro is missing from libc-shim"
#endif

#ifndef ifr_hwaddr
#error "net/if.h:ifr_hwaddr macro is missing from libc-shim"
#endif

#ifndef ifr_ifindex
#error "net/if.h:ifr_ifindex macro is missing from libc-shim"
#endif

#ifndef ifr_map
#error "net/if.h:ifr_map macro is missing from libc-shim"
#endif

#ifndef ifr_metric
#error "net/if.h:ifr_metric macro is missing from libc-shim"
#endif

#ifndef ifr_mtu
#error "net/if.h:ifr_mtu macro is missing from libc-shim"
#endif

#ifndef ifr_name
#error "net/if.h:ifr_name macro is missing from libc-shim"
#endif

#ifndef ifr_netmask
#error "net/if.h:ifr_netmask macro is missing from libc-shim"
#endif

#ifndef ifr_newname
#error "net/if.h:ifr_newname macro is missing from libc-shim"
#endif

#ifndef ifr_qlen
#error "net/if.h:ifr_qlen macro is missing from libc-shim"
#endif

#ifndef ifr_slave
#error "net/if.h:ifr_slave macro is missing from libc-shim"
#endif

int main(void) { return 0; }
