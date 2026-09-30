#include <netinet/if_ether.h>

typedef struct arphdr slate_oracle_struct_ether_arp_ea_hdr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ether_arp *)0)->ea_hdr), slate_oracle_struct_ether_arp_ea_hdr), "struct ether_arp.ea_hdr field type differs from oracle");

#ifndef ETHER_MAP_IP_MULTICAST
#error "netinet/if_ether.h:ETHER_MAP_IP_MULTICAST macro is missing from libc-shim"
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
