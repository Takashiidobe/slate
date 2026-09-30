#include <protocols/timed.h>

_Static_assert(__builtin_offsetof(struct tsp, tsp_type) == 0, "struct tsp.tsp_type offset differs from oracle");

typedef unsigned char slate_oracle_struct_tsp_tsp_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tsp *)0)->tsp_type), slate_oracle_struct_tsp_tsp_type), "struct tsp.tsp_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tsp, tsp_vers) == 1, "struct tsp.tsp_vers offset differs from oracle");

typedef unsigned char slate_oracle_struct_tsp_tsp_vers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tsp *)0)->tsp_vers), slate_oracle_struct_tsp_tsp_vers), "struct tsp.tsp_vers field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tsp, tsp_seq) == 2, "struct tsp.tsp_seq offset differs from oracle");

typedef unsigned short slate_oracle_struct_tsp_tsp_seq;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tsp *)0)->tsp_seq), slate_oracle_struct_tsp_tsp_seq), "struct tsp.tsp_seq field type differs from oracle");

#ifndef ANYADDR
#error "protocols/timed.h:ANYADDR macro is missing from libc-shim"
#endif

#ifndef MAXHOSTNAMELEN
#error "protocols/timed.h:MAXHOSTNAMELEN macro is missing from libc-shim"
#endif

#ifndef TSPTYPENUMBER
#error "protocols/timed.h:TSPTYPENUMBER macro is missing from libc-shim"
#endif

#ifndef TSPVERSION
#error "protocols/timed.h:TSPVERSION macro is missing from libc-shim"
#endif

#ifndef TSP_ACCEPT
#error "protocols/timed.h:TSP_ACCEPT macro is missing from libc-shim"
#endif

#ifndef TSP_ACK
#error "protocols/timed.h:TSP_ACK macro is missing from libc-shim"
#endif

#ifndef TSP_ADJTIME
#error "protocols/timed.h:TSP_ADJTIME macro is missing from libc-shim"
#endif

#ifndef TSP_ANY
#error "protocols/timed.h:TSP_ANY macro is missing from libc-shim"
#endif

#ifndef TSP_CONFLICT
#error "protocols/timed.h:TSP_CONFLICT macro is missing from libc-shim"
#endif

#ifndef TSP_DATE
#error "protocols/timed.h:TSP_DATE macro is missing from libc-shim"
#endif

#ifndef TSP_DATEACK
#error "protocols/timed.h:TSP_DATEACK macro is missing from libc-shim"
#endif

#ifndef TSP_DATEREQ
#error "protocols/timed.h:TSP_DATEREQ macro is missing from libc-shim"
#endif

#ifndef TSP_ELECTION
#error "protocols/timed.h:TSP_ELECTION macro is missing from libc-shim"
#endif

#ifndef TSP_LOOP
#error "protocols/timed.h:TSP_LOOP macro is missing from libc-shim"
#endif

#ifndef TSP_MASTERACK
#error "protocols/timed.h:TSP_MASTERACK macro is missing from libc-shim"
#endif

#ifndef TSP_MASTERREQ
#error "protocols/timed.h:TSP_MASTERREQ macro is missing from libc-shim"
#endif

#ifndef TSP_MASTERUP
#error "protocols/timed.h:TSP_MASTERUP macro is missing from libc-shim"
#endif

#ifndef TSP_MSITE
#error "protocols/timed.h:TSP_MSITE macro is missing from libc-shim"
#endif

#ifndef TSP_MSITEREQ
#error "protocols/timed.h:TSP_MSITEREQ macro is missing from libc-shim"
#endif

#ifndef TSP_QUIT
#error "protocols/timed.h:TSP_QUIT macro is missing from libc-shim"
#endif

#ifndef TSP_REFUSE
#error "protocols/timed.h:TSP_REFUSE macro is missing from libc-shim"
#endif

#ifndef TSP_RESOLVE
#error "protocols/timed.h:TSP_RESOLVE macro is missing from libc-shim"
#endif

#ifndef TSP_SETDATE
#error "protocols/timed.h:TSP_SETDATE macro is missing from libc-shim"
#endif

#ifndef TSP_SETDATEREQ
#error "protocols/timed.h:TSP_SETDATEREQ macro is missing from libc-shim"
#endif

#ifndef TSP_SETTIME
#error "protocols/timed.h:TSP_SETTIME macro is missing from libc-shim"
#endif

#ifndef TSP_SLAVEUP
#error "protocols/timed.h:TSP_SLAVEUP macro is missing from libc-shim"
#endif

#ifndef TSP_TEST
#error "protocols/timed.h:TSP_TEST macro is missing from libc-shim"
#endif

#ifndef TSP_TRACEOFF
#error "protocols/timed.h:TSP_TRACEOFF macro is missing from libc-shim"
#endif

#ifndef TSP_TRACEON
#error "protocols/timed.h:TSP_TRACEON macro is missing from libc-shim"
#endif

#ifndef tsp_hopcnt
#error "protocols/timed.h:tsp_hopcnt macro is missing from libc-shim"
#endif

#ifndef tsp_time
#error "protocols/timed.h:tsp_time macro is missing from libc-shim"
#endif

int main(void) { return 0; }
