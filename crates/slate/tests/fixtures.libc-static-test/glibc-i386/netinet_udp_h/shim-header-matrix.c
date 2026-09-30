#include <netinet/udp.h>

#ifndef SOL_UDP
#error "netinet/udp.h:SOL_UDP macro is missing from libc-shim"
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

int main(void) { return 0; }
