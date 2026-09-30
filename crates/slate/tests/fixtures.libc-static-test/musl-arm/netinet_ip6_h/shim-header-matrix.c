#include <netinet/ip6.h>

typedef struct in6_addr slate_oracle_struct_ip6_hdr_ip6_src;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ip6_hdr *)0)->ip6_src), slate_oracle_struct_ip6_hdr_ip6_src), "struct ip6_hdr.ip6_src field type differs from oracle");

typedef struct in6_addr slate_oracle_struct_ip6_hdr_ip6_dst;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ip6_hdr *)0)->ip6_dst), slate_oracle_struct_ip6_hdr_ip6_dst), "struct ip6_hdr.ip6_dst field type differs from oracle");

#ifndef IP6F_MORE_FRAG
#error "netinet/ip6.h:IP6F_MORE_FRAG macro is missing from libc-shim"
#endif

#ifndef IP6F_OFF_MASK
#error "netinet/ip6.h:IP6F_OFF_MASK macro is missing from libc-shim"
#endif

#ifndef IP6F_RESERVED_MASK
#error "netinet/ip6.h:IP6F_RESERVED_MASK macro is missing from libc-shim"
#endif

#ifndef IP6OPT_JUMBO
#error "netinet/ip6.h:IP6OPT_JUMBO macro is missing from libc-shim"
#endif

#ifndef IP6OPT_JUMBO_LEN
#error "netinet/ip6.h:IP6OPT_JUMBO_LEN macro is missing from libc-shim"
#endif

#ifndef IP6OPT_NSAP_ADDR
#error "netinet/ip6.h:IP6OPT_NSAP_ADDR macro is missing from libc-shim"
#endif

#ifndef IP6OPT_PAD1
#error "netinet/ip6.h:IP6OPT_PAD1 macro is missing from libc-shim"
#endif

#ifndef IP6OPT_PADN
#error "netinet/ip6.h:IP6OPT_PADN macro is missing from libc-shim"
#endif

#ifndef IP6OPT_ROUTER_ALERT
#error "netinet/ip6.h:IP6OPT_ROUTER_ALERT macro is missing from libc-shim"
#endif

#ifndef IP6OPT_TUNNEL_LIMIT
#error "netinet/ip6.h:IP6OPT_TUNNEL_LIMIT macro is missing from libc-shim"
#endif

#ifndef IP6OPT_TYPE
#error "netinet/ip6.h:IP6OPT_TYPE macro is missing from libc-shim"
#endif

#ifndef IP6OPT_TYPE_DISCARD
#error "netinet/ip6.h:IP6OPT_TYPE_DISCARD macro is missing from libc-shim"
#endif

#ifndef IP6OPT_TYPE_FORCEICMP
#error "netinet/ip6.h:IP6OPT_TYPE_FORCEICMP macro is missing from libc-shim"
#endif

#ifndef IP6OPT_TYPE_ICMP
#error "netinet/ip6.h:IP6OPT_TYPE_ICMP macro is missing from libc-shim"
#endif

#ifndef IP6OPT_TYPE_MUTABLE
#error "netinet/ip6.h:IP6OPT_TYPE_MUTABLE macro is missing from libc-shim"
#endif

#ifndef IP6OPT_TYPE_SKIP
#error "netinet/ip6.h:IP6OPT_TYPE_SKIP macro is missing from libc-shim"
#endif

#ifndef IP6_ALERT_AN
#error "netinet/ip6.h:IP6_ALERT_AN macro is missing from libc-shim"
#endif

#ifndef IP6_ALERT_MLD
#error "netinet/ip6.h:IP6_ALERT_MLD macro is missing from libc-shim"
#endif

#ifndef IP6_ALERT_RSVP
#error "netinet/ip6.h:IP6_ALERT_RSVP macro is missing from libc-shim"
#endif

#ifndef ip6_flow
#error "netinet/ip6.h:ip6_flow macro is missing from libc-shim"
#endif

#ifndef ip6_hlim
#error "netinet/ip6.h:ip6_hlim macro is missing from libc-shim"
#endif

#ifndef ip6_hops
#error "netinet/ip6.h:ip6_hops macro is missing from libc-shim"
#endif

#ifndef ip6_nxt
#error "netinet/ip6.h:ip6_nxt macro is missing from libc-shim"
#endif

#ifndef ip6_plen
#error "netinet/ip6.h:ip6_plen macro is missing from libc-shim"
#endif

#ifndef ip6_vfc
#error "netinet/ip6.h:ip6_vfc macro is missing from libc-shim"
#endif

int main(void) { return 0; }
