#ifndef _SLATE_BITS_FREEBSD_STATVFS_H
#define _SLATE_BITS_FREEBSD_STATVFS_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/statvfs.h> directly; include a public header instead."
#endif

struct statvfs {
  fsblkcnt_t    f_bavail;
  fsblkcnt_t    f_bfree;
  fsblkcnt_t    f_blocks;
  fsfilcnt_t    f_favail;
  fsfilcnt_t    f_ffree;
  fsfilcnt_t    f_files;
  unsigned long f_bsize;
  unsigned long f_flag;
  unsigned long f_frsize;
  unsigned long f_fsid;
  unsigned long f_namemax;
};

#endif
