#ifndef _SLATE_BITS_DARWIN_DIRENT_H
#define _SLATE_BITS_DARWIN_DIRENT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/dirent.h> directly; include a public header instead."
#endif

struct dirent {
  ino_t              d_ino;
  unsigned long long d_seekoff;
  unsigned short     d_reclen;
  unsigned short     d_namlen;
  unsigned char      d_type;
  char               d_name[1024];
};

#endif
