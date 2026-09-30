#include <netdb.h>

_Static_assert(__builtin_offsetof(struct addrinfo, ai_flags) == 0, "struct addrinfo.ai_flags offset differs from oracle");

typedef int slate_oracle_struct_addrinfo_ai_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct addrinfo *)0)->ai_flags), slate_oracle_struct_addrinfo_ai_flags), "struct addrinfo.ai_flags field type differs from oracle");

_Static_assert(__builtin_offsetof(struct addrinfo, ai_family) == 4, "struct addrinfo.ai_family offset differs from oracle");

typedef int slate_oracle_struct_addrinfo_ai_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct addrinfo *)0)->ai_family), slate_oracle_struct_addrinfo_ai_family), "struct addrinfo.ai_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct addrinfo, ai_socktype) == 8, "struct addrinfo.ai_socktype offset differs from oracle");

typedef int slate_oracle_struct_addrinfo_ai_socktype;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct addrinfo *)0)->ai_socktype), slate_oracle_struct_addrinfo_ai_socktype), "struct addrinfo.ai_socktype field type differs from oracle");

_Static_assert(__builtin_offsetof(struct addrinfo, ai_protocol) == 12, "struct addrinfo.ai_protocol offset differs from oracle");

typedef int slate_oracle_struct_addrinfo_ai_protocol;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct addrinfo *)0)->ai_protocol), slate_oracle_struct_addrinfo_ai_protocol), "struct addrinfo.ai_protocol field type differs from oracle");

_Static_assert(__builtin_offsetof(struct addrinfo, ai_addrlen) == 16, "struct addrinfo.ai_addrlen offset differs from oracle");

typedef unsigned int slate_oracle_struct_addrinfo_ai_addrlen;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct addrinfo *)0)->ai_addrlen), slate_oracle_struct_addrinfo_ai_addrlen), "struct addrinfo.ai_addrlen field type differs from oracle");

typedef struct sockaddr * slate_oracle_struct_addrinfo_ai_addr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct addrinfo *)0)->ai_addr), slate_oracle_struct_addrinfo_ai_addr), "struct addrinfo.ai_addr field type differs from oracle");

typedef char * slate_oracle_struct_addrinfo_ai_canonname;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct addrinfo *)0)->ai_canonname), slate_oracle_struct_addrinfo_ai_canonname), "struct addrinfo.ai_canonname field type differs from oracle");

typedef struct addrinfo * slate_oracle_struct_addrinfo_ai_next;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct addrinfo *)0)->ai_next), slate_oracle_struct_addrinfo_ai_next), "struct addrinfo.ai_next field type differs from oracle");

#ifndef AI_ADDRCONFIG
#error "netdb.h:AI_ADDRCONFIG macro is missing from libc-shim"
#endif

#ifndef AI_ALL
#error "netdb.h:AI_ALL macro is missing from libc-shim"
#endif

#ifndef AI_CANONNAME
#error "netdb.h:AI_CANONNAME macro is missing from libc-shim"
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

#ifndef HOST_NOT_FOUND
#error "netdb.h:HOST_NOT_FOUND macro is missing from libc-shim"
#endif

#ifndef NI_DGRAM
#error "netdb.h:NI_DGRAM macro is missing from libc-shim"
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

#ifndef NI_NUMERICSCOPE
#error "netdb.h:NI_NUMERICSCOPE macro is missing from libc-shim"
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
