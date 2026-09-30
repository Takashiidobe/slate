#include <netash/ash.h>

_Static_assert(sizeof(struct sockaddr_ash) == 32, "struct sockaddr_ash size differs from oracle");

_Static_assert(_Alignof(struct sockaddr_ash) == 4, "struct sockaddr_ash alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ash, sash_family) == 0, "struct sockaddr_ash.sash_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_ash_sash_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ash *)0)->sash_family), slate_oracle_struct_sockaddr_ash_sash_family), "struct sockaddr_ash.sash_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ash, sash_ifindex) == 4, "struct sockaddr_ash.sash_ifindex offset differs from oracle");

typedef int slate_oracle_struct_sockaddr_ash_sash_ifindex;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ash *)0)->sash_ifindex), slate_oracle_struct_sockaddr_ash_sash_ifindex), "struct sockaddr_ash.sash_ifindex field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ash, sash_channel) == 8, "struct sockaddr_ash.sash_channel offset differs from oracle");

typedef unsigned char slate_oracle_struct_sockaddr_ash_sash_channel;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ash *)0)->sash_channel), slate_oracle_struct_sockaddr_ash_sash_channel), "struct sockaddr_ash.sash_channel field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ash, sash_plen) == 12, "struct sockaddr_ash.sash_plen offset differs from oracle");

typedef unsigned int slate_oracle_struct_sockaddr_ash_sash_plen;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_ash *)0)->sash_plen), slate_oracle_struct_sockaddr_ash_sash_plen), "struct sockaddr_ash.sash_plen field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_ash, sash_prefix) == 16, "struct sockaddr_ash.sash_prefix offset differs from oracle");

#ifndef ASH_CHANNEL_ANY
#error "netash/ash.h:ASH_CHANNEL_ANY macro is missing from libc-shim"
#endif

#ifndef ASH_CHANNEL_CONTROL
#error "netash/ash.h:ASH_CHANNEL_CONTROL macro is missing from libc-shim"
#endif

#ifndef ASH_CHANNEL_REALTIME
#error "netash/ash.h:ASH_CHANNEL_REALTIME macro is missing from libc-shim"
#endif

int main(void) { return 0; }
