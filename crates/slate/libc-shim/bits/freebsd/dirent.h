#ifndef _SLATE_BITS_FREEBSD_DIRENT_H
#define _SLATE_BITS_FREEBSD_DIRENT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/dirent.h> directly; include a public header instead."
#endif

struct dirent {
  ino_t          d_fileno;
  off_t          d_off;
  unsigned short d_reclen;
  unsigned char  d_type;
  unsigned char  d_pad0;
  unsigned short d_namlen;
  unsigned short d_pad1;
  char           d_name[256];
};

#endif
