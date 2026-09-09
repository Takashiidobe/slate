#ifndef _SLATE_FSTAB_H
#define _SLATE_FSTAB_H

#include <features.h>

#if !defined(__SLATE_LIBC_GLIBC)
#error "<fstab.h> is only available with glibc"
#endif

#define FSTAB    "/etc/fstab"
#define FSTAB_RW "rw"
#define FSTAB_RQ "rq"
#define FSTAB_RO "ro"
#define FSTAB_SW "sw"
#define FSTAB_XX "xx"

struct fstab {
  char       *fs_spec;
  char       *fs_file;
  char       *fs_vfstype;
  char       *fs_mntops;
  const char *fs_type;
  int         fs_freq;
  int         fs_passno;
};

struct fstab *getfsent(void);
struct fstab *getfsspec(const char *);
struct fstab *getfsfile(const char *);
int           setfsent(void);
void          endfsent(void);

#endif
