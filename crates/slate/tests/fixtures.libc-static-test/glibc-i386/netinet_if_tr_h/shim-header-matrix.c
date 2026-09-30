#include <netinet/if_tr.h>

_Static_assert(sizeof(struct trh_hdr) == 32, "struct trh_hdr size differs from oracle");

_Static_assert(_Alignof(struct trh_hdr) == 2, "struct trh_hdr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct trh_hdr, ac) == 0, "struct trh_hdr.ac offset differs from oracle");

typedef unsigned char slate_oracle_struct_trh_hdr_ac;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct trh_hdr *)0)->ac), slate_oracle_struct_trh_hdr_ac), "struct trh_hdr.ac field type differs from oracle");

_Static_assert(__builtin_offsetof(struct trh_hdr, fc) == 1, "struct trh_hdr.fc offset differs from oracle");

typedef unsigned char slate_oracle_struct_trh_hdr_fc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct trh_hdr *)0)->fc), slate_oracle_struct_trh_hdr_fc), "struct trh_hdr.fc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct trh_hdr, daddr) == 2, "struct trh_hdr.daddr offset differs from oracle");

_Static_assert(__builtin_offsetof(struct trh_hdr, saddr) == 8, "struct trh_hdr.saddr offset differs from oracle");

_Static_assert(__builtin_offsetof(struct trh_hdr, rcf) == 14, "struct trh_hdr.rcf offset differs from oracle");

typedef unsigned short slate_oracle_struct_trh_hdr_rcf;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct trh_hdr *)0)->rcf), slate_oracle_struct_trh_hdr_rcf), "struct trh_hdr.rcf field type differs from oracle");

_Static_assert(__builtin_offsetof(struct trh_hdr, rseg) == 16, "struct trh_hdr.rseg offset differs from oracle");

#ifndef AC
#error "netinet/if_tr.h:AC macro is missing from libc-shim"
#endif

#ifndef EXTENDED_SAP
#error "netinet/if_tr.h:EXTENDED_SAP macro is missing from libc-shim"
#endif

#ifndef LLC_FRAME
#error "netinet/if_tr.h:LLC_FRAME macro is missing from libc-shim"
#endif

#ifndef TR_ALEN
#error "netinet/if_tr.h:TR_ALEN macro is missing from libc-shim"
#endif

#ifndef TR_HLEN
#error "netinet/if_tr.h:TR_HLEN macro is missing from libc-shim"
#endif

#ifndef TR_MAXRIFLEN
#error "netinet/if_tr.h:TR_MAXRIFLEN macro is missing from libc-shim"
#endif

#ifndef TR_RCF_BROADCAST
#error "netinet/if_tr.h:TR_RCF_BROADCAST macro is missing from libc-shim"
#endif

#ifndef TR_RCF_BROADCAST_MASK
#error "netinet/if_tr.h:TR_RCF_BROADCAST_MASK macro is missing from libc-shim"
#endif

#ifndef TR_RCF_DIR_BIT
#error "netinet/if_tr.h:TR_RCF_DIR_BIT macro is missing from libc-shim"
#endif

#ifndef TR_RCF_FRAME2K
#error "netinet/if_tr.h:TR_RCF_FRAME2K macro is missing from libc-shim"
#endif

#ifndef TR_RCF_LEN_MASK
#error "netinet/if_tr.h:TR_RCF_LEN_MASK macro is missing from libc-shim"
#endif

#ifndef TR_RCF_LIMITED_BROADCAST
#error "netinet/if_tr.h:TR_RCF_LIMITED_BROADCAST macro is missing from libc-shim"
#endif

#ifndef TR_RII
#error "netinet/if_tr.h:TR_RII macro is missing from libc-shim"
#endif

#ifndef UI_CMD
#error "netinet/if_tr.h:UI_CMD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
