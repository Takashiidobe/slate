#if !defined(_SLATE_LIBC)
#error "Never include <bits/bionic/stat.h> directly; include a public header instead."
#endif

#if defined(__SLATE_ARCH_X86_64)
struct stat {
  dev_t   st_dev;
  ino_t   st_ino;
  unsigned long st_nlink;
  mode_t  st_mode;
  uid_t   st_uid;
  gid_t   st_gid;
  unsigned int __pad0;
  dev_t   st_rdev;
  off_t   st_size;
  long    st_blksize;
  long    st_blocks;
  struct timespec st_atim;
  struct timespec st_mtim;
  struct timespec st_ctim;
  long    __pad3[3];
};
#else
struct stat {
  dev_t   st_dev;
  ino_t   st_ino;
  mode_t  st_mode;
  nlink_t st_nlink;
  uid_t   st_uid;
  gid_t   st_gid;
  dev_t   st_rdev;
  unsigned long __pad1;
  off_t   st_size;
  int     st_blksize;
  int     __pad2;
  long    st_blocks;
  struct timespec st_atim;
  struct timespec st_mtim;
  struct timespec st_ctim;
  unsigned int __unused4;
  unsigned int __unused5;
};
#endif
