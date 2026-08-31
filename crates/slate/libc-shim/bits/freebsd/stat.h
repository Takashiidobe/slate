#ifndef _SLATE_BITS_FREEBSD_STAT_H
#define _SLATE_BITS_FREEBSD_STAT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/stat.h> directly; include a public header instead."
#endif

struct stat {
  dev_t           st_dev;
  ino_t           st_ino;
  nlink_t         st_nlink;
  mode_t          st_mode;
  short           st_bsdflags;
  uid_t           st_uid;
  gid_t           st_gid;
  int             st_padding1;
  dev_t           st_rdev;
  struct timespec st_atim;
  struct timespec st_mtim;
  struct timespec st_ctim;
  struct timespec st_birthtim;
  off_t           st_size;
  blkcnt_t        st_blocks;
  blksize_t       st_blksize;
  unsigned int    st_flags;
  unsigned long long st_gen;
  unsigned long long st_filerev;
  unsigned long long st_spare[9];
};

#define st_atime         st_atim.tv_sec
#define st_mtime         st_mtim.tv_sec
#define st_ctime         st_ctim.tv_sec
#define st_birthtime     st_birthtim.tv_sec
#define st_atimensec     st_atim.tv_nsec
#define st_mtimensec     st_mtim.tv_nsec
#define st_ctimensec     st_ctim.tv_nsec
#define st_birthtimensec st_birthtim.tv_nsec

#define st_atimespec     st_atim
#define st_mtimespec     st_mtim
#define st_ctimespec     st_ctim
#define st_birthtimespec st_birthtim

#endif
