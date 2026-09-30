#include <net/if_packet.h>

_Static_assert(sizeof(struct sockaddr_pkt) == 18, "struct sockaddr_pkt size differs from oracle");

_Static_assert(_Alignof(struct sockaddr_pkt) == 2, "struct sockaddr_pkt alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_pkt, spkt_family) == 0, "struct sockaddr_pkt.spkt_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_pkt_spkt_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_pkt *)0)->spkt_family), slate_oracle_struct_sockaddr_pkt_spkt_family), "struct sockaddr_pkt.spkt_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_pkt, spkt_device) == 2, "struct sockaddr_pkt.spkt_device offset differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr_pkt, spkt_protocol) == 16, "struct sockaddr_pkt.spkt_protocol offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_pkt_spkt_protocol;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr_pkt *)0)->spkt_protocol), slate_oracle_struct_sockaddr_pkt_spkt_protocol), "struct sockaddr_pkt.spkt_protocol field type differs from oracle");

int main(void) { return 0; }
