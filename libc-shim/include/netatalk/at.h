#ifndef _SLATE_NETATALK_AT_H
#define _SLATE_NETATALK_AT_H

#include <features.h>
#if !defined(__SLATE_LIBC_GLIBC)
#error "<netatalk/at.h> is unavailable for this libc profile."
#endif

#include <asm/types.h>
#include <sys/socket.h>
#include <sys/types.h>

#define ATPORT_FIRST 1
#define ATPORT_RESERVED 128
#define ATPORT_LAST 254
#define ATADDR_ANYNET ((__u16)0)
#define ATADDR_ANYNODE ((__u8)0)
#define ATADDR_ANYPORT ((__u8)0)
#define ATADDR_BCAST ((__u8)255)
#define DDP_MAXSZ 587
#define DDP_MAXHOPS 15
#define SIOCATALKDIFADDR 0x89E0

struct atalk_addr { __u16 s_net; __u8 s_node; };
struct sockaddr_at { unsigned short sat_family; __u8 sat_port; struct atalk_addr sat_addr; char sat_zero[8]; };
struct atalk_netrange { __u8 nr_phase; __u16 nr_firstnet; __u16 nr_lastnet; };

#define SOL_ATALK 258

#endif
