#ifndef _SLATE_PWD_H
#define _SLATE_PWD_H

#include <features.h>

#define __NEED_size_t
#define __NEED_uid_t
#define __NEED_gid_t
#ifdef _GNU_SOURCE
#define __NEED_FILE
#endif
#if defined(__SLATE_LIBC_DARWIN) || defined(__SLATE_LIBC_FREEBSD)
#define __NEED_time_t
#endif
#include <bits/types.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/pwd.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/pwd.h>

#else

struct passwd {
  char *pw_name;
  char *pw_passwd;
  uid_t pw_uid;
  gid_t pw_gid;
  char *pw_gecos;
  char *pw_dir;
  char *pw_shell;
};

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
void           setpwent(void);
void           endpwent(void);
struct passwd *getpwent(void);
#endif

struct passwd *getpwuid(uid_t);
struct passwd *getpwnam(const char *);
int getpwuid_r(uid_t, struct passwd *, char *, size_t, struct passwd **);
int getpwnam_r(const char *, struct passwd *, char *, size_t, struct passwd **);

#ifdef _GNU_SOURCE
struct passwd *fgetpwent(FILE *);
int            putpwent(const struct passwd *, FILE *);
#endif

#endif 

#endif
