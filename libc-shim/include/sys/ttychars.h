#ifndef _SLATE_SYS_TTYCHARS_H
#define _SLATE_SYS_TTYCHARS_H

#if defined(__SLATE_LIBC_GLIBC)
struct ttychars {
  char tc_erase;
  char tc_kill;
  char tc_intrc;
  char tc_quitc;
  char tc_startc;
  char tc_stopc;
  char tc_eofc;
  char tc_brkc;
  char tc_suspc;
  char tc_dsuspc;
  char tc_rprntc;
  char tc_flushc;
  char tc_werasc;
  char tc_lnextc;
};
#endif

#endif
