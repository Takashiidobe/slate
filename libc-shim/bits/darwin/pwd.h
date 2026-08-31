#ifndef _SLATE_BITS_DARWIN_PWD_H
#define _SLATE_BITS_DARWIN_PWD_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/pwd.h> directly; include a public header instead."
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
};

void           setpwent(void);
void           endpwent(void);
struct passwd *getpwent(void);
struct passwd *getpwuid(uid_t);
struct passwd *getpwnam(const char *);
int getpwuid_r(uid_t, struct passwd *, char *, size_t, struct passwd **);
int getpwnam_r(const char *, struct passwd *, char *, size_t, struct passwd **);

#endif
