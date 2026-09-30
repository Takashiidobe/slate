#include <neteconet/ec.h>

_Static_assert(sizeof(struct ec_addr) == 2, "struct ec_addr size differs from oracle");

_Static_assert(_Alignof(struct ec_addr) == 1, "struct ec_addr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct ec_addr, station) == 0, "struct ec_addr.station offset differs from oracle");

typedef unsigned char slate_oracle_struct_ec_addr_station;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ec_addr *)0)->station), slate_oracle_struct_ec_addr_station), "struct ec_addr.station field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ec_addr, net) == 1, "struct ec_addr.net offset differs from oracle");

typedef unsigned char slate_oracle_struct_ec_addr_net;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ec_addr *)0)->net), slate_oracle_struct_ec_addr_net), "struct ec_addr.net field type differs from oracle");

#ifndef ECTYPE_PACKET_RECEIVED
#error "neteconet/ec.h:ECTYPE_PACKET_RECEIVED macro is missing from libc-shim"
#endif

#ifndef ECTYPE_TRANSMIT_LINE_JAMMED
#error "neteconet/ec.h:ECTYPE_TRANSMIT_LINE_JAMMED macro is missing from libc-shim"
#endif

#ifndef ECTYPE_TRANSMIT_NET_ERROR
#error "neteconet/ec.h:ECTYPE_TRANSMIT_NET_ERROR macro is missing from libc-shim"
#endif

#ifndef ECTYPE_TRANSMIT_NOT_LISTENING
#error "neteconet/ec.h:ECTYPE_TRANSMIT_NOT_LISTENING macro is missing from libc-shim"
#endif

#ifndef ECTYPE_TRANSMIT_NOT_PRESENT
#error "neteconet/ec.h:ECTYPE_TRANSMIT_NOT_PRESENT macro is missing from libc-shim"
#endif

#ifndef ECTYPE_TRANSMIT_NO_CLOCK
#error "neteconet/ec.h:ECTYPE_TRANSMIT_NO_CLOCK macro is missing from libc-shim"
#endif

#ifndef ECTYPE_TRANSMIT_OK
#error "neteconet/ec.h:ECTYPE_TRANSMIT_OK macro is missing from libc-shim"
#endif

#ifndef ECTYPE_TRANSMIT_STATUS
#error "neteconet/ec.h:ECTYPE_TRANSMIT_STATUS macro is missing from libc-shim"
#endif

int main(void) { return 0; }
