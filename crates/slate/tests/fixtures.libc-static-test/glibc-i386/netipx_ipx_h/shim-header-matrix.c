#include <netipx/ipx.h>

_Static_assert(sizeof(struct sockaddr_ipx) == 16, "struct sockaddr_ipx size differs from oracle");

_Static_assert(_Alignof(struct sockaddr_ipx) == 4, "struct sockaddr_ipx alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ipx, sipx_family) == 0, "struct sockaddr_ipx.sipx_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_ipx_sipx_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ipx *)0)->sipx_family), slate_oracle_struct_sockaddr_ipx_sipx_family), "struct sockaddr_ipx.sipx_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ipx, sipx_port) == 2, "struct sockaddr_ipx.sipx_port offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_ipx_sipx_port;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ipx *)0)->sipx_port), slate_oracle_struct_sockaddr_ipx_sipx_port), "struct sockaddr_ipx.sipx_port field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ipx, sipx_network) == 4, "struct sockaddr_ipx.sipx_network offset differs from oracle");

typedef unsigned int slate_oracle_struct_sockaddr_ipx_sipx_network;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ipx *)0)->sipx_network), slate_oracle_struct_sockaddr_ipx_sipx_network), "struct sockaddr_ipx.sipx_network field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ipx, sipx_node) == 8, "struct sockaddr_ipx.sipx_node offset differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ipx, sipx_type) == 14, "struct sockaddr_ipx.sipx_type offset differs from oracle");

typedef unsigned char slate_oracle_struct_sockaddr_ipx_sipx_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ipx *)0)->sipx_type), slate_oracle_struct_sockaddr_ipx_sipx_type), "struct sockaddr_ipx.sipx_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ipx, sipx_zero) == 15, "struct sockaddr_ipx.sipx_zero offset differs from oracle");

typedef unsigned char slate_oracle_struct_sockaddr_ipx_sipx_zero;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ipx *)0)->sipx_zero), slate_oracle_struct_sockaddr_ipx_sipx_zero), "struct sockaddr_ipx.sipx_zero field type differs from oracle");

#ifndef IPX_CRTITF
#error "netipx/ipx.h:IPX_CRTITF macro is missing from libc-shim"
#endif

#ifndef IPX_DLTITF
#error "netipx/ipx.h:IPX_DLTITF macro is missing from libc-shim"
#endif

#ifndef IPX_FRAME_8022
#error "netipx/ipx.h:IPX_FRAME_8022 macro is missing from libc-shim"
#endif

#ifndef IPX_FRAME_8023
#error "netipx/ipx.h:IPX_FRAME_8023 macro is missing from libc-shim"
#endif

#ifndef IPX_FRAME_ETHERII
#error "netipx/ipx.h:IPX_FRAME_ETHERII macro is missing from libc-shim"
#endif

#ifndef IPX_FRAME_NONE
#error "netipx/ipx.h:IPX_FRAME_NONE macro is missing from libc-shim"
#endif

#ifndef IPX_FRAME_SNAP
#error "netipx/ipx.h:IPX_FRAME_SNAP macro is missing from libc-shim"
#endif

#ifndef IPX_FRAME_TR_8022
#error "netipx/ipx.h:IPX_FRAME_TR_8022 macro is missing from libc-shim"
#endif

#ifndef IPX_INTERNAL
#error "netipx/ipx.h:IPX_INTERNAL macro is missing from libc-shim"
#endif

#ifndef IPX_MTU
#error "netipx/ipx.h:IPX_MTU macro is missing from libc-shim"
#endif

#ifndef IPX_NODE_LEN
#error "netipx/ipx.h:IPX_NODE_LEN macro is missing from libc-shim"
#endif

#ifndef IPX_PRIMARY
#error "netipx/ipx.h:IPX_PRIMARY macro is missing from libc-shim"
#endif

#ifndef IPX_ROUTE_NO_ROUTER
#error "netipx/ipx.h:IPX_ROUTE_NO_ROUTER macro is missing from libc-shim"
#endif

#ifndef IPX_RT_8022
#error "netipx/ipx.h:IPX_RT_8022 macro is missing from libc-shim"
#endif

#ifndef IPX_RT_BLUEBOOK
#error "netipx/ipx.h:IPX_RT_BLUEBOOK macro is missing from libc-shim"
#endif

#ifndef IPX_RT_ROUTED
#error "netipx/ipx.h:IPX_RT_ROUTED macro is missing from libc-shim"
#endif

#ifndef IPX_RT_SNAP
#error "netipx/ipx.h:IPX_RT_SNAP macro is missing from libc-shim"
#endif

#ifndef IPX_SPECIAL_NONE
#error "netipx/ipx.h:IPX_SPECIAL_NONE macro is missing from libc-shim"
#endif

#ifndef IPX_TYPE
#error "netipx/ipx.h:IPX_TYPE macro is missing from libc-shim"
#endif

#ifndef SIOCAIPXITFCRT
#error "netipx/ipx.h:SIOCAIPXITFCRT macro is missing from libc-shim"
#endif

#ifndef SIOCAIPXPRISLT
#error "netipx/ipx.h:SIOCAIPXPRISLT macro is missing from libc-shim"
#endif

#ifndef SIOCIPXCFGDATA
#error "netipx/ipx.h:SIOCIPXCFGDATA macro is missing from libc-shim"
#endif

#ifndef SIOCIPXNCPCONN
#error "netipx/ipx.h:SIOCIPXNCPCONN macro is missing from libc-shim"
#endif

#ifndef SOL_IPX
#error "netipx/ipx.h:SOL_IPX macro is missing from libc-shim"
#endif

#ifndef sipx_action
#error "netipx/ipx.h:sipx_action macro is missing from libc-shim"
#endif

#ifndef sipx_special
#error "netipx/ipx.h:sipx_special macro is missing from libc-shim"
#endif

int main(void) { return 0; }
