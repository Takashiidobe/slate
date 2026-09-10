#ifndef _SLATE_NETIUCV_IUCV_H
#define _SLATE_NETIUCV_IUCV_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)
#include <bits/sockaddr.h>

struct __attribute_struct_may_alias__ sockaddr_iucv {
  __SOCKADDR_COMMON(siucv_);
  unsigned short siucv_port;
  unsigned int siucv_addr;
  char siucv_nodeid[8];
  char siucv_user_id[8];
  char siucv_name[8];
};

#define SOL_IUCV 277
#define SO_IPRMDATA_MSG 0x0080
#define SO_MSGLIMIT 0x1000
#define SO_MSGSIZE 0x0800
#define SCM_IUCV_TRGCLS 0x0001
#endif

#endif
