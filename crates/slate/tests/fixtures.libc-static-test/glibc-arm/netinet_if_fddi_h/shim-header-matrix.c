#include <netinet/if_fddi.h>

_Static_assert(sizeof(struct fddi_header) == 13, "struct fddi_header size differs from oracle");

_Static_assert(_Alignof(struct fddi_header) == 1, "struct fddi_header alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct fddi_header, fddi_fc) == 0, "struct fddi_header.fddi_fc offset differs from oracle");

typedef unsigned char slate_oracle_struct_fddi_header_fddi_fc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fddi_header *)0)->fddi_fc), slate_oracle_struct_fddi_header_fddi_fc), "struct fddi_header.fddi_fc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fddi_header, fddi_dhost) == 1, "struct fddi_header.fddi_dhost offset differs from oracle");

_Static_assert(__builtin_offsetof(struct fddi_header, fddi_shost) == 7, "struct fddi_header.fddi_shost offset differs from oracle");

int main(void) { return 0; }
