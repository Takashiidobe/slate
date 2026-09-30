#include <net/if_ppp.h>

_Static_assert(sizeof(struct npioctl) == 8, "struct npioctl size differs from oracle");

_Static_assert(_Alignof(struct npioctl) == 4, "struct npioctl alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct npioctl, protocol) == 0, "struct npioctl.protocol offset differs from oracle");

typedef int slate_oracle_struct_npioctl_protocol;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct npioctl *)0)->protocol), slate_oracle_struct_npioctl_protocol), "struct npioctl.protocol field type differs from oracle");

_Static_assert(__builtin_offsetof(struct npioctl, mode) == 4, "struct npioctl.mode offset differs from oracle");

typedef enum NPmode slate_oracle_struct_npioctl_mode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct npioctl *)0)->mode), slate_oracle_struct_npioctl_mode), "struct npioctl.mode field type differs from oracle");

#ifndef PPPIOCGASYNCMAP
#error "net/if_ppp.h:PPPIOCGASYNCMAP macro is missing from libc-shim"
#endif

#ifndef PPPIOCGDEBUG
#error "net/if_ppp.h:PPPIOCGDEBUG macro is missing from libc-shim"
#endif

#ifndef PPPIOCGFLAGS
#error "net/if_ppp.h:PPPIOCGFLAGS macro is missing from libc-shim"
#endif

#ifndef PPPIOCGIDLE
#error "net/if_ppp.h:PPPIOCGIDLE macro is missing from libc-shim"
#endif

#ifndef PPPIOCGMRU
#error "net/if_ppp.h:PPPIOCGMRU macro is missing from libc-shim"
#endif

#ifndef PPPIOCGNPMODE
#error "net/if_ppp.h:PPPIOCGNPMODE macro is missing from libc-shim"
#endif

#ifndef PPPIOCGRASYNCMAP
#error "net/if_ppp.h:PPPIOCGRASYNCMAP macro is missing from libc-shim"
#endif

#ifndef PPPIOCGUNIT
#error "net/if_ppp.h:PPPIOCGUNIT macro is missing from libc-shim"
#endif

#ifndef PPPIOCGXASYNCMAP
#error "net/if_ppp.h:PPPIOCGXASYNCMAP macro is missing from libc-shim"
#endif

#ifndef PPPIOCSASYNCMAP
#error "net/if_ppp.h:PPPIOCSASYNCMAP macro is missing from libc-shim"
#endif

#ifndef PPPIOCSCOMPRESS
#error "net/if_ppp.h:PPPIOCSCOMPRESS macro is missing from libc-shim"
#endif

#ifndef PPPIOCSDEBUG
#error "net/if_ppp.h:PPPIOCSDEBUG macro is missing from libc-shim"
#endif

#ifndef PPPIOCSFLAGS
#error "net/if_ppp.h:PPPIOCSFLAGS macro is missing from libc-shim"
#endif

#ifndef PPPIOCSMAXCID
#error "net/if_ppp.h:PPPIOCSMAXCID macro is missing from libc-shim"
#endif

#ifndef PPPIOCSMRU
#error "net/if_ppp.h:PPPIOCSMRU macro is missing from libc-shim"
#endif

#ifndef PPPIOCSNPMODE
#error "net/if_ppp.h:PPPIOCSNPMODE macro is missing from libc-shim"
#endif

#ifndef PPPIOCSRASYNCMAP
#error "net/if_ppp.h:PPPIOCSRASYNCMAP macro is missing from libc-shim"
#endif

#ifndef PPPIOCSXASYNCMAP
#error "net/if_ppp.h:PPPIOCSXASYNCMAP macro is missing from libc-shim"
#endif

#ifndef PPPIOCXFERUNIT
#error "net/if_ppp.h:PPPIOCXFERUNIT macro is missing from libc-shim"
#endif

#ifndef PPP_MAGIC
#error "net/if_ppp.h:PPP_MAGIC macro is missing from libc-shim"
#endif

#ifndef PPP_MAXMRU
#error "net/if_ppp.h:PPP_MAXMRU macro is missing from libc-shim"
#endif

#ifndef PPP_MTU
#error "net/if_ppp.h:PPP_MTU macro is missing from libc-shim"
#endif

#ifndef PPP_VERSION
#error "net/if_ppp.h:PPP_VERSION macro is missing from libc-shim"
#endif

#ifndef PROTO_DNA_RT
#error "net/if_ppp.h:PROTO_DNA_RT macro is missing from libc-shim"
#endif

#ifndef PROTO_IPX
#error "net/if_ppp.h:PROTO_IPX macro is missing from libc-shim"
#endif

#ifndef SC_CCP_OPEN
#error "net/if_ppp.h:SC_CCP_OPEN macro is missing from libc-shim"
#endif

#ifndef SC_CCP_UP
#error "net/if_ppp.h:SC_CCP_UP macro is missing from libc-shim"
#endif

#ifndef SC_COMP_AC
#error "net/if_ppp.h:SC_COMP_AC macro is missing from libc-shim"
#endif

#ifndef SC_COMP_PROT
#error "net/if_ppp.h:SC_COMP_PROT macro is missing from libc-shim"
#endif

#ifndef SC_COMP_RUN
#error "net/if_ppp.h:SC_COMP_RUN macro is missing from libc-shim"
#endif

#ifndef SC_COMP_TCP
#error "net/if_ppp.h:SC_COMP_TCP macro is missing from libc-shim"
#endif

#ifndef SC_DC_ERROR
#error "net/if_ppp.h:SC_DC_ERROR macro is missing from libc-shim"
#endif

#ifndef SC_DC_FERROR
#error "net/if_ppp.h:SC_DC_FERROR macro is missing from libc-shim"
#endif

#ifndef SC_DEBUG
#error "net/if_ppp.h:SC_DEBUG macro is missing from libc-shim"
#endif

#ifndef SC_DECOMP_RUN
#error "net/if_ppp.h:SC_DECOMP_RUN macro is missing from libc-shim"
#endif

#ifndef SC_ENABLE_IP
#error "net/if_ppp.h:SC_ENABLE_IP macro is missing from libc-shim"
#endif

#ifndef SC_ESCAPED
#error "net/if_ppp.h:SC_ESCAPED macro is missing from libc-shim"
#endif

#ifndef SC_FLUSH
#error "net/if_ppp.h:SC_FLUSH macro is missing from libc-shim"
#endif

#ifndef SC_LOG_FLUSH
#error "net/if_ppp.h:SC_LOG_FLUSH macro is missing from libc-shim"
#endif

#ifndef SC_LOG_INPKT
#error "net/if_ppp.h:SC_LOG_INPKT macro is missing from libc-shim"
#endif

#ifndef SC_LOG_OUTPKT
#error "net/if_ppp.h:SC_LOG_OUTPKT macro is missing from libc-shim"
#endif

#ifndef SC_LOG_RAWIN
#error "net/if_ppp.h:SC_LOG_RAWIN macro is missing from libc-shim"
#endif

#ifndef SC_MASK
#error "net/if_ppp.h:SC_MASK macro is missing from libc-shim"
#endif

#ifndef SC_NO_TCP_CCID
#error "net/if_ppp.h:SC_NO_TCP_CCID macro is missing from libc-shim"
#endif

#ifndef SC_RCV_B7_0
#error "net/if_ppp.h:SC_RCV_B7_0 macro is missing from libc-shim"
#endif

#ifndef SC_RCV_B7_1
#error "net/if_ppp.h:SC_RCV_B7_1 macro is missing from libc-shim"
#endif

#ifndef SC_RCV_EVNP
#error "net/if_ppp.h:SC_RCV_EVNP macro is missing from libc-shim"
#endif

#ifndef SC_RCV_ODDP
#error "net/if_ppp.h:SC_RCV_ODDP macro is missing from libc-shim"
#endif

#ifndef SC_REJ_COMP_AC
#error "net/if_ppp.h:SC_REJ_COMP_AC macro is missing from libc-shim"
#endif

#ifndef SC_REJ_COMP_TCP
#error "net/if_ppp.h:SC_REJ_COMP_TCP macro is missing from libc-shim"
#endif

#ifndef SC_VJ_RESET
#error "net/if_ppp.h:SC_VJ_RESET macro is missing from libc-shim"
#endif

#ifndef SC_XMIT_BUSY
#error "net/if_ppp.h:SC_XMIT_BUSY macro is missing from libc-shim"
#endif

#ifndef SIOCGPPPCSTATS
#error "net/if_ppp.h:SIOCGPPPCSTATS macro is missing from libc-shim"
#endif

#ifndef SIOCGPPPSTATS
#error "net/if_ppp.h:SIOCGPPPSTATS macro is missing from libc-shim"
#endif

#ifndef SIOCGPPPVER
#error "net/if_ppp.h:SIOCGPPPVER macro is missing from libc-shim"
#endif

#ifndef ifr__name
#error "net/if_ppp.h:ifr__name macro is missing from libc-shim"
#endif

#ifndef stats_ptr
#error "net/if_ppp.h:stats_ptr macro is missing from libc-shim"
#endif

int main(void) { return 0; }
