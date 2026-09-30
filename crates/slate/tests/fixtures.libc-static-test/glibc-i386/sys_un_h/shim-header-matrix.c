#include <sys/un.h>

_Static_assert(sizeof(struct sockaddr_un) == 110, "struct sockaddr_un size differs from oracle");

_Static_assert(_Alignof(struct sockaddr_un) == 2, "struct sockaddr_un alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_un, sun_family) == 0, "struct sockaddr_un.sun_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_un_sun_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_un *)0)->sun_family), slate_oracle_struct_sockaddr_un_sun_family), "struct sockaddr_un.sun_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_un, sun_path) == 2, "struct sockaddr_un.sun_path offset differs from oracle");

#ifndef SUN_LEN
#error "sys/un.h:SUN_LEN macro is missing from libc-shim"
#endif

int main(void) { return 0; }
