#include <netinet/udp.h>

_Static_assert(sizeof(struct udphdr) == 8, "struct udphdr size differs from oracle");

_Static_assert(_Alignof(struct udphdr) == 2, "struct udphdr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct udphdr, source) == 0, "struct udphdr.source offset differs from oracle");

typedef unsigned short slate_oracle_struct_udphdr_source;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct udphdr *)0)->source), slate_oracle_struct_udphdr_source), "struct udphdr.source field type differs from oracle");

_Static_assert(__builtin_offsetof(struct udphdr, dest) == 2, "struct udphdr.dest offset differs from oracle");

typedef unsigned short slate_oracle_struct_udphdr_dest;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct udphdr *)0)->dest), slate_oracle_struct_udphdr_dest), "struct udphdr.dest field type differs from oracle");

_Static_assert(__builtin_offsetof(struct udphdr, len) == 4, "struct udphdr.len offset differs from oracle");

typedef unsigned short slate_oracle_struct_udphdr_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct udphdr *)0)->len), slate_oracle_struct_udphdr_len), "struct udphdr.len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct udphdr, check) == 6, "struct udphdr.check offset differs from oracle");

typedef unsigned short slate_oracle_struct_udphdr_check;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct udphdr *)0)->check), slate_oracle_struct_udphdr_check), "struct udphdr.check field type differs from oracle");

#ifndef SOL_UDP
#error "netinet/udp.h:SOL_UDP macro is missing from libc-shim"
#endif

#ifndef TCP_ENCAP_ESPINTCP
#error "netinet/udp.h:TCP_ENCAP_ESPINTCP macro is missing from libc-shim"
#endif

#ifndef UDP_CORK
#error "netinet/udp.h:UDP_CORK macro is missing from libc-shim"
#endif

#ifndef UDP_ENCAP
#error "netinet/udp.h:UDP_ENCAP macro is missing from libc-shim"
#endif

#ifndef UDP_ENCAP_ESPINUDP
#error "netinet/udp.h:UDP_ENCAP_ESPINUDP macro is missing from libc-shim"
#endif

#ifndef UDP_ENCAP_ESPINUDP_NON_IKE
#error "netinet/udp.h:UDP_ENCAP_ESPINUDP_NON_IKE macro is missing from libc-shim"
#endif

#ifndef UDP_ENCAP_GTP0
#error "netinet/udp.h:UDP_ENCAP_GTP0 macro is missing from libc-shim"
#endif

#ifndef UDP_ENCAP_GTP1U
#error "netinet/udp.h:UDP_ENCAP_GTP1U macro is missing from libc-shim"
#endif

#ifndef UDP_ENCAP_L2TPINUDP
#error "netinet/udp.h:UDP_ENCAP_L2TPINUDP macro is missing from libc-shim"
#endif

#ifndef UDP_ENCAP_RXRPC
#error "netinet/udp.h:UDP_ENCAP_RXRPC macro is missing from libc-shim"
#endif

#ifndef UDP_GRO
#error "netinet/udp.h:UDP_GRO macro is missing from libc-shim"
#endif

#ifndef UDP_NO_CHECK6_RX
#error "netinet/udp.h:UDP_NO_CHECK6_RX macro is missing from libc-shim"
#endif

#ifndef UDP_NO_CHECK6_TX
#error "netinet/udp.h:UDP_NO_CHECK6_TX macro is missing from libc-shim"
#endif

#ifndef UDP_SEGMENT
#error "netinet/udp.h:UDP_SEGMENT macro is missing from libc-shim"
#endif

#ifndef uh_dport
#error "netinet/udp.h:uh_dport macro is missing from libc-shim"
#endif

#ifndef uh_sport
#error "netinet/udp.h:uh_sport macro is missing from libc-shim"
#endif

#ifndef uh_sum
#error "netinet/udp.h:uh_sum macro is missing from libc-shim"
#endif

#ifndef uh_ulen
#error "netinet/udp.h:uh_ulen macro is missing from libc-shim"
#endif

int main(void) { return 0; }
