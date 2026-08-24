#ifndef _SLATE_BITS_MSVC_SYS_STAT_H
#define _SLATE_BITS_MSVC_SYS_STAT_H

#include <bits/msvc/types.h>

struct _stat32 {
  _dev_t         st_dev;
  _ino_t         st_ino;
  unsigned short st_mode;
  short          st_nlink;
  short          st_uid;
  short          st_gid;
  _dev_t         st_rdev;
  _off_t         st_size;
  __time32_t     st_atime;
  __time32_t     st_mtime;
  __time32_t     st_ctime;
};

struct _stat32i64 {
  _dev_t         st_dev;
  _ino_t         st_ino;
  unsigned short st_mode;
  short          st_nlink;
  short          st_uid;
  short          st_gid;
  _dev_t         st_rdev;
  long long      st_size;
  __time32_t     st_atime;
  __time32_t     st_mtime;
  __time32_t     st_ctime;
};

struct _stat64i32 {
  _dev_t         st_dev;
  _ino_t         st_ino;
  unsigned short st_mode;
  short          st_nlink;
  short          st_uid;
  short          st_gid;
  _dev_t         st_rdev;
  _off_t         st_size;
  __time64_t     st_atime;
  __time64_t     st_mtime;
  __time64_t     st_ctime;
};

struct _stat64 {
  _dev_t         st_dev;
  _ino_t         st_ino;
  unsigned short st_mode;
  short          st_nlink;
  short          st_uid;
  short          st_gid;
  _dev_t         st_rdev;
  long long      st_size;
  __time64_t     st_atime;
  __time64_t     st_mtime;
  __time64_t     st_ctime;
};

#define _S_IFMT   0xF000
#define _S_IFDIR  0x4000
#define _S_IFCHR  0x2000
#define _S_IFIFO  0x1000
#define _S_IFREG  0x8000
#define _S_IREAD  0x0100
#define _S_IWRITE 0x0080
#define _S_IEXEC  0x0040

int _fstat32(int, struct _stat32 *);
int _fstat32i64(int, struct _stat32i64 *);
int _fstat64i32(int, struct _stat64i32 *);
int _fstat64(int, struct _stat64 *);
int _stat32(const char *, struct _stat32 *);
int _stat32i64(const char *, struct _stat32i64 *);
int _stat64i32(const char *, struct _stat64i32 *);
int _stat64(const char *, struct _stat64 *);
int _wstat32(const wchar_t *, struct _stat32 *);
int _wstat32i64(const wchar_t *, struct _stat32i64 *);
int _wstat64i32(const wchar_t *, struct _stat64i32 *);
int _wstat64(const wchar_t *, struct _stat64 *);

#endif
