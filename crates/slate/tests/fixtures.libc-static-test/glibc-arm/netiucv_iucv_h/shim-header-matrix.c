#include <netiucv/iucv.h>

_Static_assert(sizeof(struct sockaddr_iucv) == 32, "struct sockaddr_iucv size differs from oracle");

_Static_assert(_Alignof(struct sockaddr_iucv) == 4, "struct sockaddr_iucv alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_iucv, siucv_family) == 0, "struct sockaddr_iucv.siucv_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_iucv_siucv_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_iucv *)0)->siucv_family), slate_oracle_struct_sockaddr_iucv_siucv_family), "struct sockaddr_iucv.siucv_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_iucv, siucv_port) == 2, "struct sockaddr_iucv.siucv_port offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_iucv_siucv_port;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_iucv *)0)->siucv_port), slate_oracle_struct_sockaddr_iucv_siucv_port), "struct sockaddr_iucv.siucv_port field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_iucv, siucv_addr) == 4, "struct sockaddr_iucv.siucv_addr offset differs from oracle");

typedef unsigned int slate_oracle_struct_sockaddr_iucv_siucv_addr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_iucv *)0)->siucv_addr), slate_oracle_struct_sockaddr_iucv_siucv_addr), "struct sockaddr_iucv.siucv_addr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_iucv, siucv_nodeid) == 8, "struct sockaddr_iucv.siucv_nodeid offset differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_iucv, siucv_user_id) == 16, "struct sockaddr_iucv.siucv_user_id offset differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_iucv, siucv_name) == 24, "struct sockaddr_iucv.siucv_name offset differs from oracle");

#ifndef SCM_IUCV_TRGCLS
#error "netiucv/iucv.h:SCM_IUCV_TRGCLS macro is missing from libc-shim"
#endif

#ifndef SOL_IUCV
#error "netiucv/iucv.h:SOL_IUCV macro is missing from libc-shim"
#endif

#ifndef SO_IPRMDATA_MSG
#error "netiucv/iucv.h:SO_IPRMDATA_MSG macro is missing from libc-shim"
#endif

#ifndef SO_MSGLIMIT
#error "netiucv/iucv.h:SO_MSGLIMIT macro is missing from libc-shim"
#endif

#ifndef SO_MSGSIZE
#error "netiucv/iucv.h:SO_MSGSIZE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
