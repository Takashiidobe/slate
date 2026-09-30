#include <netrom/netrom.h>

_Static_assert(sizeof(struct nr_route_struct) == 112, "struct nr_route_struct size differs from oracle");

_Static_assert(_Alignof(struct nr_route_struct) == 4, "struct nr_route_struct alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, type) == 0, "struct nr_route_struct.type offset differs from oracle");

typedef int slate_oracle_struct_nr_route_struct_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct nr_route_struct *)0)->type), slate_oracle_struct_nr_route_struct_type), "struct nr_route_struct.type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, callsign) == 4, "struct nr_route_struct.callsign offset differs from oracle");

typedef struct ax25_address slate_oracle_struct_nr_route_struct_callsign;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct nr_route_struct *)0)->callsign), slate_oracle_struct_nr_route_struct_callsign), "struct nr_route_struct.callsign field type differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, device) == 11, "struct nr_route_struct.device offset differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, quality) == 28, "struct nr_route_struct.quality offset differs from oracle");

typedef unsigned int slate_oracle_struct_nr_route_struct_quality;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct nr_route_struct *)0)->quality), slate_oracle_struct_nr_route_struct_quality), "struct nr_route_struct.quality field type differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, mnemonic) == 32, "struct nr_route_struct.mnemonic offset differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, neighbour) == 39, "struct nr_route_struct.neighbour offset differs from oracle");

typedef struct ax25_address slate_oracle_struct_nr_route_struct_neighbour;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct nr_route_struct *)0)->neighbour), slate_oracle_struct_nr_route_struct_neighbour), "struct nr_route_struct.neighbour field type differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, obs_count) == 48, "struct nr_route_struct.obs_count offset differs from oracle");

typedef unsigned int slate_oracle_struct_nr_route_struct_obs_count;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct nr_route_struct *)0)->obs_count), slate_oracle_struct_nr_route_struct_obs_count), "struct nr_route_struct.obs_count field type differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, ndigis) == 52, "struct nr_route_struct.ndigis offset differs from oracle");

typedef unsigned int slate_oracle_struct_nr_route_struct_ndigis;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct nr_route_struct *)0)->ndigis), slate_oracle_struct_nr_route_struct_ndigis), "struct nr_route_struct.ndigis field type differs from oracle");

_Static_assert(__builtin_offsetof(struct nr_route_struct, digipeaters) == 56, "struct nr_route_struct.digipeaters offset differs from oracle");

#ifndef NETROM_IDLE
#error "netrom/netrom.h:NETROM_IDLE macro is missing from libc-shim"
#endif

#ifndef NETROM_KILL
#error "netrom/netrom.h:NETROM_KILL macro is missing from libc-shim"
#endif

#ifndef NETROM_N2
#error "netrom/netrom.h:NETROM_N2 macro is missing from libc-shim"
#endif

#ifndef NETROM_NEIGH
#error "netrom/netrom.h:NETROM_NEIGH macro is missing from libc-shim"
#endif

#ifndef NETROM_NODE
#error "netrom/netrom.h:NETROM_NODE macro is missing from libc-shim"
#endif

#ifndef NETROM_PACLEN
#error "netrom/netrom.h:NETROM_PACLEN macro is missing from libc-shim"
#endif

#ifndef NETROM_T1
#error "netrom/netrom.h:NETROM_T1 macro is missing from libc-shim"
#endif

#ifndef NETROM_T2
#error "netrom/netrom.h:NETROM_T2 macro is missing from libc-shim"
#endif

#ifndef NETROM_T4
#error "netrom/netrom.h:NETROM_T4 macro is missing from libc-shim"
#endif

#ifndef SIOCNRCTLCON
#error "netrom/netrom.h:SIOCNRCTLCON macro is missing from libc-shim"
#endif

#ifndef SIOCNRDECOBS
#error "netrom/netrom.h:SIOCNRDECOBS macro is missing from libc-shim"
#endif

#ifndef SIOCNRGETPARMS
#error "netrom/netrom.h:SIOCNRGETPARMS macro is missing from libc-shim"
#endif

#ifndef SIOCNRRTCTL
#error "netrom/netrom.h:SIOCNRRTCTL macro is missing from libc-shim"
#endif

#ifndef SIOCNRSETPARMS
#error "netrom/netrom.h:SIOCNRSETPARMS macro is missing from libc-shim"
#endif

#ifndef SOL_NETROM
#error "netrom/netrom.h:SOL_NETROM macro is missing from libc-shim"
#endif

int main(void) { return 0; }
