#ifndef _SLATE_BITS_FREEBSD_PWD_H
#define _SLATE_BITS_FREEBSD_PWD_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/pwd.h> directly; include a public header instead."
#endif

struct passwd {
  char  *pw_name;
  char  *pw_passwd;
  uid_t  pw_uid;
  gid_t  pw_gid;
  time_t pw_change;
  char  *pw_class;
  char  *pw_gecos;
  char  *pw_dir;
  char  *pw_shell;
  time_t pw_expire;
  int    pw_fields;
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
