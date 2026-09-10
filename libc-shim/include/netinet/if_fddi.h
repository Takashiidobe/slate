#ifndef _SLATE_NETINET_IF_FDDI_H
#define _SLATE_NETINET_IF_FDDI_H

#include <features.h>
#include <sys/types.h>
#include <stdint.h>

#if !defined(__SLATE_LIBC_GLIBC)
#error "<netinet/if_fddi.h> is unavailable for this libc profile."
#endif

#define FDDI_K_ALEN 6
#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
struct fddi_header { uint8_t fddi_fc; uint8_t fddi_dhost[FDDI_K_ALEN]; uint8_t fddi_shost[FDDI_K_ALEN]; };
#endif

#endif
