#include <netdb.h>

_Static_assert(sizeof(struct netent) == 16, "struct netent size differs from oracle");

_Static_assert(_Alignof(struct netent) == 4, "struct netent alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct netent, n_name) == 0, "struct netent.n_name offset differs from oracle");

typedef char * slate_oracle_struct_netent_n_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct netent *)0)->n_name), slate_oracle_struct_netent_n_name), "struct netent.n_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct netent, n_aliases) == 4, "struct netent.n_aliases offset differs from oracle");

typedef char ** slate_oracle_struct_netent_n_aliases;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct netent *)0)->n_aliases), slate_oracle_struct_netent_n_aliases), "struct netent.n_aliases field type differs from oracle");

_Static_assert(__builtin_offsetof(struct netent, n_addrtype) == 8, "struct netent.n_addrtype offset differs from oracle");

typedef int slate_oracle_struct_netent_n_addrtype;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct netent *)0)->n_addrtype), slate_oracle_struct_netent_n_addrtype), "struct netent.n_addrtype field type differs from oracle");

_Static_assert(__builtin_offsetof(struct netent, n_net) == 12, "struct netent.n_net offset differs from oracle");

typedef unsigned int slate_oracle_struct_netent_n_net;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct netent *)0)->n_net), slate_oracle_struct_netent_n_net), "struct netent.n_net field type differs from oracle");

_Static_assert(sizeof(struct rpcent) == 12, "struct rpcent size differs from oracle");

_Static_assert(_Alignof(struct rpcent) == 4, "struct rpcent alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct rpcent, r_name) == 0, "struct rpcent.r_name offset differs from oracle");

typedef char * slate_oracle_struct_rpcent_r_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rpcent *)0)->r_name), slate_oracle_struct_rpcent_r_name), "struct rpcent.r_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct rpcent, r_aliases) == 4, "struct rpcent.r_aliases offset differs from oracle");

typedef char ** slate_oracle_struct_rpcent_r_aliases;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rpcent *)0)->r_aliases), slate_oracle_struct_rpcent_r_aliases), "struct rpcent.r_aliases field type differs from oracle");

_Static_assert(__builtin_offsetof(struct rpcent, r_number) == 8, "struct rpcent.r_number offset differs from oracle");

typedef int slate_oracle_struct_rpcent_r_number;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rpcent *)0)->r_number), slate_oracle_struct_rpcent_r_number), "struct rpcent.r_number field type differs from oracle");

#ifndef AI_ADDRCONFIG
#error "netdb.h:AI_ADDRCONFIG macro is missing from libc-shim"
#endif

#ifndef AI_ALL
#error "netdb.h:AI_ALL macro is missing from libc-shim"
#endif

#ifndef AI_CANONIDN
#error "netdb.h:AI_CANONIDN macro is missing from libc-shim"
#endif

#ifndef AI_CANONNAME
#error "netdb.h:AI_CANONNAME macro is missing from libc-shim"
#endif

#ifndef AI_IDN
#error "netdb.h:AI_IDN macro is missing from libc-shim"
#endif

#ifndef AI_IDN_ALLOW_UNASSIGNED
#error "netdb.h:AI_IDN_ALLOW_UNASSIGNED macro is missing from libc-shim"
#endif

#ifndef AI_IDN_USE_STD3_ASCII_RULES
#error "netdb.h:AI_IDN_USE_STD3_ASCII_RULES macro is missing from libc-shim"
#endif

#ifndef AI_NUMERICHOST
#error "netdb.h:AI_NUMERICHOST macro is missing from libc-shim"
#endif

#ifndef AI_NUMERICSERV
#error "netdb.h:AI_NUMERICSERV macro is missing from libc-shim"
#endif

#ifndef AI_PASSIVE
#error "netdb.h:AI_PASSIVE macro is missing from libc-shim"
#endif

#ifndef AI_V4MAPPED
#error "netdb.h:AI_V4MAPPED macro is missing from libc-shim"
#endif

#ifndef EAI_ADDRFAMILY
#error "netdb.h:EAI_ADDRFAMILY macro is missing from libc-shim"
#endif

#ifndef EAI_AGAIN
#error "netdb.h:EAI_AGAIN macro is missing from libc-shim"
#endif

#ifndef EAI_ALLDONE
#error "netdb.h:EAI_ALLDONE macro is missing from libc-shim"
#endif

#ifndef EAI_BADFLAGS
#error "netdb.h:EAI_BADFLAGS macro is missing from libc-shim"
#endif

#ifndef EAI_CANCELED
#error "netdb.h:EAI_CANCELED macro is missing from libc-shim"
#endif

#ifndef EAI_FAIL
#error "netdb.h:EAI_FAIL macro is missing from libc-shim"
#endif

#ifndef EAI_FAMILY
#error "netdb.h:EAI_FAMILY macro is missing from libc-shim"
#endif

#ifndef EAI_IDN_ENCODE
#error "netdb.h:EAI_IDN_ENCODE macro is missing from libc-shim"
#endif

#ifndef EAI_INPROGRESS
#error "netdb.h:EAI_INPROGRESS macro is missing from libc-shim"
#endif

#ifndef EAI_INTR
#error "netdb.h:EAI_INTR macro is missing from libc-shim"
#endif

#ifndef EAI_MEMORY
#error "netdb.h:EAI_MEMORY macro is missing from libc-shim"
#endif

#ifndef EAI_NODATA
#error "netdb.h:EAI_NODATA macro is missing from libc-shim"
#endif

#ifndef EAI_NONAME
#error "netdb.h:EAI_NONAME macro is missing from libc-shim"
#endif

#ifndef EAI_NOTCANCELED
#error "netdb.h:EAI_NOTCANCELED macro is missing from libc-shim"
#endif

#ifndef EAI_OVERFLOW
#error "netdb.h:EAI_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef EAI_SERVICE
#error "netdb.h:EAI_SERVICE macro is missing from libc-shim"
#endif

#ifndef EAI_SOCKTYPE
#error "netdb.h:EAI_SOCKTYPE macro is missing from libc-shim"
#endif

#ifndef EAI_SYSTEM
#error "netdb.h:EAI_SYSTEM macro is missing from libc-shim"
#endif

#ifndef GAI_NOWAIT
#error "netdb.h:GAI_NOWAIT macro is missing from libc-shim"
#endif

#ifndef GAI_WAIT
#error "netdb.h:GAI_WAIT macro is missing from libc-shim"
#endif

#ifndef HOST_NOT_FOUND
#error "netdb.h:HOST_NOT_FOUND macro is missing from libc-shim"
#endif

#ifndef IPPORT_RESERVED
#error "netdb.h:IPPORT_RESERVED macro is missing from libc-shim"
#endif

#ifndef NETDB_INTERNAL
#error "netdb.h:NETDB_INTERNAL macro is missing from libc-shim"
#endif

#ifndef NETDB_SUCCESS
#error "netdb.h:NETDB_SUCCESS macro is missing from libc-shim"
#endif

#ifndef NI_DGRAM
#error "netdb.h:NI_DGRAM macro is missing from libc-shim"
#endif

#ifndef NI_IDN
#error "netdb.h:NI_IDN macro is missing from libc-shim"
#endif

#ifndef NI_IDN_ALLOW_UNASSIGNED
#error "netdb.h:NI_IDN_ALLOW_UNASSIGNED macro is missing from libc-shim"
#endif

#ifndef NI_IDN_USE_STD3_ASCII_RULES
#error "netdb.h:NI_IDN_USE_STD3_ASCII_RULES macro is missing from libc-shim"
#endif

#ifndef NI_MAXHOST
#error "netdb.h:NI_MAXHOST macro is missing from libc-shim"
#endif

#ifndef NI_MAXSERV
#error "netdb.h:NI_MAXSERV macro is missing from libc-shim"
#endif

#ifndef NI_NAMEREQD
#error "netdb.h:NI_NAMEREQD macro is missing from libc-shim"
#endif

#ifndef NI_NOFQDN
#error "netdb.h:NI_NOFQDN macro is missing from libc-shim"
#endif

#ifndef NI_NUMERICHOST
#error "netdb.h:NI_NUMERICHOST macro is missing from libc-shim"
#endif

#ifndef NI_NUMERICSERV
#error "netdb.h:NI_NUMERICSERV macro is missing from libc-shim"
#endif

#ifndef NO_ADDRESS
#error "netdb.h:NO_ADDRESS macro is missing from libc-shim"
#endif

#ifndef NO_DATA
#error "netdb.h:NO_DATA macro is missing from libc-shim"
#endif

#ifndef NO_RECOVERY
#error "netdb.h:NO_RECOVERY macro is missing from libc-shim"
#endif

#ifndef SCOPE_DELIMITER
#error "netdb.h:SCOPE_DELIMITER macro is missing from libc-shim"
#endif

#ifndef TRY_AGAIN
#error "netdb.h:TRY_AGAIN macro is missing from libc-shim"
#endif

#ifndef h_addr
#error "netdb.h:h_addr macro is missing from libc-shim"
#endif

#ifndef h_errno
#error "netdb.h:h_errno macro is missing from libc-shim"
#endif

int main(void) { return 0; }
