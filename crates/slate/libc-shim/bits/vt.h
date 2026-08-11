#ifndef _SLATE_BITS_VT_H
#define _SLATE_BITS_VT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/vt.h> directly; include <sys/vt.h> instead."
#endif

#ifndef _LINUX_VT_H
#define _LINUX_VT_H
#include <sys/ioctl.h>
#define MIN_NR_CONSOLES 1
#define MAX_NR_CONSOLES 63

#define VT_OPENQRY 0x5600

struct vt_mode {
  unsigned char mode;
  unsigned char waitv;
  short         relsig;
  short         acqsig;
  short         frsig;
};
#define VT_GETMODE 0x5601
#define VT_SETMODE 0x5602
#define VT_AUTO    0x00
#define VT_PROCESS 0x01
#define VT_ACKACQ  0x02

struct vt_stat {
  unsigned short v_active;
  unsigned short v_signal;
  unsigned short v_state;
};
#define VT_GETSTATE 0x5603
#define VT_SENDSIG  0x5604

#define VT_RELDISP 0x5605

#define VT_ACTIVATE    0x5606
#define VT_WAITACTIVE  0x5607
#define VT_DISALLOCATE 0x5608

struct vt_sizes {
  unsigned short v_rows;
  unsigned short v_cols;
  unsigned short v_scrollsize;
};
#define VT_RESIZE 0x5609

struct vt_consize {
  unsigned short v_rows;
  unsigned short v_cols;
  unsigned short v_vlin;
  unsigned short v_clin;
  unsigned short v_vcol;
  unsigned short v_ccol;
};
#define VT_RESIZEX       0x560A
#define VT_LOCKSWITCH    0x560B
#define VT_UNLOCKSWITCH  0x560C
#define VT_GETHIFONTMASK 0x560D

struct vt_event {
  unsigned int event;
#define VT_EVENT_SWITCH  0x0001
#define VT_EVENT_BLANK   0x0002
#define VT_EVENT_UNBLANK 0x0004
#define VT_EVENT_RESIZE  0x0008
#define VT_MAX_EVENT     0x000F
  unsigned int oldev;
  unsigned int newev;
  unsigned int pad[4];
};

#define VT_WAITEVENT 0x560E

struct vt_setactivate {
  unsigned int   console;
  struct vt_mode mode;
};

#define VT_SETACTIVATE 0x560F

struct vt_consizecsrpos {
  unsigned short con_rows;
  unsigned short con_cols;
  unsigned short csr_row;
  unsigned short csr_col;
};
#define VT_GETCONSIZECSRPOS _IOR('V', 0x10, struct vt_consizecsrpos)

#endif

#endif
