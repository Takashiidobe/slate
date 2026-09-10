#ifndef _SLATE_NET_IF_SLIP_H
#define _SLATE_NET_IF_SLIP_H

#include <features.h>
#if !defined(__SLATE_LIBC_GLIBC)
#error "<net/if_slip.h> is unavailable for this libc profile."
#endif

#define SL_MODE_SLIP  0
#define SL_MODE_CSLIP 1
#define SL_MODE_KISS  4
#define SL_OPT_SIXBIT 2
#define SL_OPT_ADAPTIVE 8
#define SIOCSKEEPALIVE 0x89F0
#define SIOCGKEEPALIVE 0x89F1
#define SIOCSOUTFILL 0x89F2
#define SIOCGOUTFILL 0x89F3
#define SIOCSLEASE 0x89F4
#define SIOCGLEASE 0x89F5

#endif
