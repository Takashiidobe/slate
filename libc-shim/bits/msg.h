#ifndef _SLATE_BITS_MSG_H
#define _SLATE_BITS_MSG_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msg.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_ARM)
#include <bits/arm/msg.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/msg.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/msg.h>
#else
struct msqid_ds {
  struct ipc_perm msg_perm;
  time_t          msg_stime;
  time_t          msg_rtime;
  time_t          msg_ctime;
  unsigned long   msg_cbytes;
  msgqnum_t       msg_qnum;
  msglen_t        msg_qbytes;
  pid_t           msg_lspid;
  pid_t           msg_lrpid;
  unsigned long   __unused[2];
};
#endif

#endif /* _SLATE_BITS_MSG_H */
