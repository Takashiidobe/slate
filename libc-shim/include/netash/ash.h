#ifndef _SLATE_NETASH_ASH_H
#define _SLATE_NETASH_ASH_H

#include <features.h>
#if !defined(__SLATE_LIBC_GLIBC)
#error "<netash/ash.h> is unavailable for this libc profile."
#endif

#include <bits/sockaddr.h>

struct __attribute__((__may_alias__)) sockaddr_ash {
  __SOCKADDR_COMMON(sash_);
  int sash_ifindex;
  unsigned char sash_channel;
  unsigned int sash_plen;
  unsigned char sash_prefix[16];
};

#define ASH_CHANNEL_ANY 0
#define ASH_CHANNEL_CONTROL 1
#define ASH_CHANNEL_REALTIME 2

#endif
