#ifndef _SLATE_BITS_DARWIN_STATVFS_H
#define _SLATE_BITS_DARWIN_STATVFS_H

struct statvfs {
  unsigned long f_bsize;
  unsigned long f_frsize;
  fsblkcnt_t    f_blocks;
  fsblkcnt_t    f_bfree;
  fsblkcnt_t    f_bavail;
  fsfilcnt_t    f_files;
  fsfilcnt_t    f_ffree;
  fsfilcnt_t    f_favail;
  unsigned long f_fsid;
  unsigned long f_flag;
  unsigned long f_namemax;
};

#endif
