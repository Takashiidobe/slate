#ifndef _SLATE_BITS_DARWIN_STAT_H
#define _SLATE_BITS_DARWIN_STAT_H

struct stat {
  dev_t           st_dev;
  mode_t          st_mode;
  nlink_t         st_nlink;
  ino_t           st_ino;
  uid_t           st_uid;
  gid_t           st_gid;
  dev_t           st_rdev;
  struct timespec st_atimespec;
  struct timespec st_mtimespec;
  struct timespec st_ctimespec;
  struct timespec st_birthtimespec;
  off_t           st_size;
  blkcnt_t        st_blocks;
  blksize_t       st_blksize;
  unsigned int    st_flags;
  unsigned int    st_gen;
  int             st_lspare;
  long long       st_qspare[2];
};

#define st_atime st_atimespec.tv_sec
#define st_mtime st_mtimespec.tv_sec
#define st_ctime st_ctimespec.tv_sec

#endif
