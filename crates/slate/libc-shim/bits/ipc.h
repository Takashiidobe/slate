#ifndef _SLATE_BITS_IPC_H
#define _SLATE_BITS_IPC_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/ipc.h> directly; include <sys/ipc.h> instead."
#endif

struct ipc_perm {
#if defined(__SLATE_LIBC_GLIBC)
  key_t          __key;
  uid_t          uid;
  gid_t          gid;
  uid_t          cuid;
  gid_t          cgid;
  mode_t         mode;
  unsigned short __seq;
  unsigned short __pad2;
  unsigned long  __glibc_reserved1;
  unsigned long  __glibc_reserved2;
#else
  key_t  __ipc_perm_key;
  uid_t  uid;
  gid_t  gid;
  uid_t  cuid;
  gid_t  cgid;
  mode_t mode;
  int    __ipc_perm_seq;
  long   __pad1;
  long   __pad2;
#endif
};

#endif
