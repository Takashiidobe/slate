#ifndef _SLATE_BITS_IPC_H
#define _SLATE_BITS_IPC_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/ipc.h> directly; include <sys/ipc.h> instead."
#endif

struct ipc_perm {
  key_t  __ipc_perm_key;
  uid_t  uid;
  gid_t  gid;
  uid_t  cuid;
  gid_t  cgid;
  mode_t mode;
  int    __ipc_perm_seq;
  long   __pad1;
  long   __pad2;
};

#endif
