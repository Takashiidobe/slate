#include <protocols/routed.h>

typedef struct sockaddr slate_oracle_struct_netinfo_rip_dst;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct netinfo *)0)->rip_dst), slate_oracle_struct_netinfo_rip_dst), "struct netinfo.rip_dst field type differs from oracle");

typedef int slate_oracle_struct_netinfo_rip_metric;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct netinfo *)0)->rip_metric), slate_oracle_struct_netinfo_rip_metric), "struct netinfo.rip_metric field type differs from oracle");

#ifndef EXPIRE_TIME
#error "protocols/routed.h:EXPIRE_TIME macro is missing from libc-shim"
#endif

#ifndef GARBAGE_TIME
#error "protocols/routed.h:GARBAGE_TIME macro is missing from libc-shim"
#endif

#ifndef HOPCNT_INFINITY
#error "protocols/routed.h:HOPCNT_INFINITY macro is missing from libc-shim"
#endif

#ifndef MAXPACKETSIZE
#error "protocols/routed.h:MAXPACKETSIZE macro is missing from libc-shim"
#endif

#ifndef MAX_WAITTIME
#error "protocols/routed.h:MAX_WAITTIME macro is missing from libc-shim"
#endif

#ifndef MIN_WAITTIME
#error "protocols/routed.h:MIN_WAITTIME macro is missing from libc-shim"
#endif

#ifndef RIPCMD_MAX
#error "protocols/routed.h:RIPCMD_MAX macro is missing from libc-shim"
#endif

#ifndef RIPCMD_REQUEST
#error "protocols/routed.h:RIPCMD_REQUEST macro is missing from libc-shim"
#endif

#ifndef RIPCMD_RESPONSE
#error "protocols/routed.h:RIPCMD_RESPONSE macro is missing from libc-shim"
#endif

#ifndef RIPCMD_TRACEOFF
#error "protocols/routed.h:RIPCMD_TRACEOFF macro is missing from libc-shim"
#endif

#ifndef RIPCMD_TRACEON
#error "protocols/routed.h:RIPCMD_TRACEON macro is missing from libc-shim"
#endif

#ifndef RIPVERSION
#error "protocols/routed.h:RIPVERSION macro is missing from libc-shim"
#endif

#ifndef SUPPLY_INTERVAL
#error "protocols/routed.h:SUPPLY_INTERVAL macro is missing from libc-shim"
#endif

#ifndef TIMER_RATE
#error "protocols/routed.h:TIMER_RATE macro is missing from libc-shim"
#endif

#ifndef rip_nets
#error "protocols/routed.h:rip_nets macro is missing from libc-shim"
#endif

#ifndef rip_tracefile
#error "protocols/routed.h:rip_tracefile macro is missing from libc-shim"
#endif

int main(void) { return 0; }
