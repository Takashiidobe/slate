#include <netinet/ip.h>

_Static_assert(sizeof(struct timestamp) == 40, "struct timestamp size differs from oracle");

_Static_assert(_Alignof(struct timestamp) == 4, "struct timestamp alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timestamp, len) == 0, "struct timestamp.len offset differs from oracle");

typedef unsigned char slate_oracle_struct_timestamp_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timestamp *)0)->len), slate_oracle_struct_timestamp_len), "struct timestamp.len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timestamp, ptr) == 1, "struct timestamp.ptr offset differs from oracle");

typedef unsigned char slate_oracle_struct_timestamp_ptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timestamp *)0)->ptr), slate_oracle_struct_timestamp_ptr), "struct timestamp.ptr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timestamp, data) == 4, "struct timestamp.data offset differs from oracle");

#ifndef IPDEFTTL
#error "netinet/ip.h:IPDEFTTL macro is missing from libc-shim"
#endif

#ifndef IPFRAGTTL
#error "netinet/ip.h:IPFRAGTTL macro is missing from libc-shim"
#endif

#ifndef IPOPT_CLASS
#error "netinet/ip.h:IPOPT_CLASS macro is missing from libc-shim"
#endif

#ifndef IPOPT_CLASS_MASK
#error "netinet/ip.h:IPOPT_CLASS_MASK macro is missing from libc-shim"
#endif

#ifndef IPOPT_CONTROL
#error "netinet/ip.h:IPOPT_CONTROL macro is missing from libc-shim"
#endif

#ifndef IPOPT_COPIED
#error "netinet/ip.h:IPOPT_COPIED macro is missing from libc-shim"
#endif

#ifndef IPOPT_COPY
#error "netinet/ip.h:IPOPT_COPY macro is missing from libc-shim"
#endif

#ifndef IPOPT_DEBMEAS
#error "netinet/ip.h:IPOPT_DEBMEAS macro is missing from libc-shim"
#endif

#ifndef IPOPT_END
#error "netinet/ip.h:IPOPT_END macro is missing from libc-shim"
#endif

#ifndef IPOPT_EOL
#error "netinet/ip.h:IPOPT_EOL macro is missing from libc-shim"
#endif

#ifndef IPOPT_LSRR
#error "netinet/ip.h:IPOPT_LSRR macro is missing from libc-shim"
#endif

#ifndef IPOPT_MEASUREMENT
#error "netinet/ip.h:IPOPT_MEASUREMENT macro is missing from libc-shim"
#endif

#ifndef IPOPT_MINOFF
#error "netinet/ip.h:IPOPT_MINOFF macro is missing from libc-shim"
#endif

#ifndef IPOPT_NOOP
#error "netinet/ip.h:IPOPT_NOOP macro is missing from libc-shim"
#endif

#ifndef IPOPT_NOP
#error "netinet/ip.h:IPOPT_NOP macro is missing from libc-shim"
#endif

#ifndef IPOPT_NUMBER
#error "netinet/ip.h:IPOPT_NUMBER macro is missing from libc-shim"
#endif

#ifndef IPOPT_NUMBER_MASK
#error "netinet/ip.h:IPOPT_NUMBER_MASK macro is missing from libc-shim"
#endif

#ifndef IPOPT_OFFSET
#error "netinet/ip.h:IPOPT_OFFSET macro is missing from libc-shim"
#endif

#ifndef IPOPT_OLEN
#error "netinet/ip.h:IPOPT_OLEN macro is missing from libc-shim"
#endif

#ifndef IPOPT_OPTVAL
#error "netinet/ip.h:IPOPT_OPTVAL macro is missing from libc-shim"
#endif

#ifndef IPOPT_RA
#error "netinet/ip.h:IPOPT_RA macro is missing from libc-shim"
#endif

#ifndef IPOPT_RESERVED1
#error "netinet/ip.h:IPOPT_RESERVED1 macro is missing from libc-shim"
#endif

#ifndef IPOPT_RESERVED2
#error "netinet/ip.h:IPOPT_RESERVED2 macro is missing from libc-shim"
#endif

#ifndef IPOPT_RR
#error "netinet/ip.h:IPOPT_RR macro is missing from libc-shim"
#endif

#ifndef IPOPT_SATID
#error "netinet/ip.h:IPOPT_SATID macro is missing from libc-shim"
#endif

#ifndef IPOPT_SEC
#error "netinet/ip.h:IPOPT_SEC macro is missing from libc-shim"
#endif

#ifndef IPOPT_SECURITY
#error "netinet/ip.h:IPOPT_SECURITY macro is missing from libc-shim"
#endif

#ifndef IPOPT_SECUR_CONFID
#error "netinet/ip.h:IPOPT_SECUR_CONFID macro is missing from libc-shim"
#endif

#ifndef IPOPT_SECUR_EFTO
#error "netinet/ip.h:IPOPT_SECUR_EFTO macro is missing from libc-shim"
#endif

#ifndef IPOPT_SECUR_MMMM
#error "netinet/ip.h:IPOPT_SECUR_MMMM macro is missing from libc-shim"
#endif

#ifndef IPOPT_SECUR_RESTR
#error "netinet/ip.h:IPOPT_SECUR_RESTR macro is missing from libc-shim"
#endif

#ifndef IPOPT_SECUR_SECRET
#error "netinet/ip.h:IPOPT_SECUR_SECRET macro is missing from libc-shim"
#endif

#ifndef IPOPT_SECUR_TOPSECRET
#error "netinet/ip.h:IPOPT_SECUR_TOPSECRET macro is missing from libc-shim"
#endif

#ifndef IPOPT_SECUR_UNCLASS
#error "netinet/ip.h:IPOPT_SECUR_UNCLASS macro is missing from libc-shim"
#endif

#ifndef IPOPT_SID
#error "netinet/ip.h:IPOPT_SID macro is missing from libc-shim"
#endif

#ifndef IPOPT_SSRR
#error "netinet/ip.h:IPOPT_SSRR macro is missing from libc-shim"
#endif

#ifndef IPOPT_TIMESTAMP
#error "netinet/ip.h:IPOPT_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef IPOPT_TS
#error "netinet/ip.h:IPOPT_TS macro is missing from libc-shim"
#endif

#ifndef IPOPT_TS_PRESPEC
#error "netinet/ip.h:IPOPT_TS_PRESPEC macro is missing from libc-shim"
#endif

#ifndef IPOPT_TS_TSANDADDR
#error "netinet/ip.h:IPOPT_TS_TSANDADDR macro is missing from libc-shim"
#endif

#ifndef IPOPT_TS_TSONLY
#error "netinet/ip.h:IPOPT_TS_TSONLY macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS
#error "netinet/ip.h:IPTOS_CLASS macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_CS0
#error "netinet/ip.h:IPTOS_CLASS_CS0 macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_CS1
#error "netinet/ip.h:IPTOS_CLASS_CS1 macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_CS2
#error "netinet/ip.h:IPTOS_CLASS_CS2 macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_CS3
#error "netinet/ip.h:IPTOS_CLASS_CS3 macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_CS4
#error "netinet/ip.h:IPTOS_CLASS_CS4 macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_CS5
#error "netinet/ip.h:IPTOS_CLASS_CS5 macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_CS6
#error "netinet/ip.h:IPTOS_CLASS_CS6 macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_CS7
#error "netinet/ip.h:IPTOS_CLASS_CS7 macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_DEFAULT
#error "netinet/ip.h:IPTOS_CLASS_DEFAULT macro is missing from libc-shim"
#endif

#ifndef IPTOS_CLASS_MASK
#error "netinet/ip.h:IPTOS_CLASS_MASK macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP
#error "netinet/ip.h:IPTOS_DSCP macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF11
#error "netinet/ip.h:IPTOS_DSCP_AF11 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF12
#error "netinet/ip.h:IPTOS_DSCP_AF12 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF13
#error "netinet/ip.h:IPTOS_DSCP_AF13 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF21
#error "netinet/ip.h:IPTOS_DSCP_AF21 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF22
#error "netinet/ip.h:IPTOS_DSCP_AF22 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF23
#error "netinet/ip.h:IPTOS_DSCP_AF23 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF31
#error "netinet/ip.h:IPTOS_DSCP_AF31 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF32
#error "netinet/ip.h:IPTOS_DSCP_AF32 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF33
#error "netinet/ip.h:IPTOS_DSCP_AF33 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF41
#error "netinet/ip.h:IPTOS_DSCP_AF41 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF42
#error "netinet/ip.h:IPTOS_DSCP_AF42 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_AF43
#error "netinet/ip.h:IPTOS_DSCP_AF43 macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_EF
#error "netinet/ip.h:IPTOS_DSCP_EF macro is missing from libc-shim"
#endif

#ifndef IPTOS_DSCP_MASK
#error "netinet/ip.h:IPTOS_DSCP_MASK macro is missing from libc-shim"
#endif

#ifndef IPTOS_ECN
#error "netinet/ip.h:IPTOS_ECN macro is missing from libc-shim"
#endif

#ifndef IPTOS_ECN_CE
#error "netinet/ip.h:IPTOS_ECN_CE macro is missing from libc-shim"
#endif

#ifndef IPTOS_ECN_ECT0
#error "netinet/ip.h:IPTOS_ECN_ECT0 macro is missing from libc-shim"
#endif

#ifndef IPTOS_ECN_ECT1
#error "netinet/ip.h:IPTOS_ECN_ECT1 macro is missing from libc-shim"
#endif

#ifndef IPTOS_ECN_MASK
#error "netinet/ip.h:IPTOS_ECN_MASK macro is missing from libc-shim"
#endif

#ifndef IPTOS_ECN_NOT_ECT
#error "netinet/ip.h:IPTOS_ECN_NOT_ECT macro is missing from libc-shim"
#endif

#ifndef IPTOS_LOWCOST
#error "netinet/ip.h:IPTOS_LOWCOST macro is missing from libc-shim"
#endif

#ifndef IPTOS_LOWDELAY
#error "netinet/ip.h:IPTOS_LOWDELAY macro is missing from libc-shim"
#endif

#ifndef IPTOS_MINCOST
#error "netinet/ip.h:IPTOS_MINCOST macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC
#error "netinet/ip.h:IPTOS_PREC macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_CRITIC_ECP
#error "netinet/ip.h:IPTOS_PREC_CRITIC_ECP macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_FLASH
#error "netinet/ip.h:IPTOS_PREC_FLASH macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_FLASHOVERRIDE
#error "netinet/ip.h:IPTOS_PREC_FLASHOVERRIDE macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_IMMEDIATE
#error "netinet/ip.h:IPTOS_PREC_IMMEDIATE macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_INTERNETCONTROL
#error "netinet/ip.h:IPTOS_PREC_INTERNETCONTROL macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_MASK
#error "netinet/ip.h:IPTOS_PREC_MASK macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_NETCONTROL
#error "netinet/ip.h:IPTOS_PREC_NETCONTROL macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_PRIORITY
#error "netinet/ip.h:IPTOS_PREC_PRIORITY macro is missing from libc-shim"
#endif

#ifndef IPTOS_PREC_ROUTINE
#error "netinet/ip.h:IPTOS_PREC_ROUTINE macro is missing from libc-shim"
#endif

#ifndef IPTOS_RELIABILITY
#error "netinet/ip.h:IPTOS_RELIABILITY macro is missing from libc-shim"
#endif

#ifndef IPTOS_THROUGHPUT
#error "netinet/ip.h:IPTOS_THROUGHPUT macro is missing from libc-shim"
#endif

#ifndef IPTOS_TOS
#error "netinet/ip.h:IPTOS_TOS macro is missing from libc-shim"
#endif

#ifndef IPTOS_TOS_MASK
#error "netinet/ip.h:IPTOS_TOS_MASK macro is missing from libc-shim"
#endif

#ifndef IPTTLDEC
#error "netinet/ip.h:IPTTLDEC macro is missing from libc-shim"
#endif

#ifndef IPVERSION
#error "netinet/ip.h:IPVERSION macro is missing from libc-shim"
#endif

#ifndef IP_DF
#error "netinet/ip.h:IP_DF macro is missing from libc-shim"
#endif

#ifndef IP_MAXPACKET
#error "netinet/ip.h:IP_MAXPACKET macro is missing from libc-shim"
#endif

#ifndef IP_MF
#error "netinet/ip.h:IP_MF macro is missing from libc-shim"
#endif

#ifndef IP_MSS
#error "netinet/ip.h:IP_MSS macro is missing from libc-shim"
#endif

#ifndef IP_OFFMASK
#error "netinet/ip.h:IP_OFFMASK macro is missing from libc-shim"
#endif

#ifndef IP_RF
#error "netinet/ip.h:IP_RF macro is missing from libc-shim"
#endif

#ifndef MAXTTL
#error "netinet/ip.h:MAXTTL macro is missing from libc-shim"
#endif

#ifndef MAX_IPOPTLEN
#error "netinet/ip.h:MAX_IPOPTLEN macro is missing from libc-shim"
#endif

int main(void) { return 0; }
