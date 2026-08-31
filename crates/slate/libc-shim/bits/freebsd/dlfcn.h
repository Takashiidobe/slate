#ifndef _SLATE_BITS_FREEBSD_DLFCN_H
#define _SLATE_BITS_FREEBSD_DLFCN_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/dlfcn.h> directly; include a public header instead."
#endif

#define RTLD_LAZY     1
#define RTLD_NOW      2
#define RTLD_MODEMASK 0x3
#define RTLD_GLOBAL   0x100
#define RTLD_LOCAL    0
#define RTLD_TRACE    0x200
#define RTLD_NODELETE 0x01000
#define RTLD_NOLOAD   0x02000
#define RTLD_DEEPBIND 0x04000

#define RTLD_DI_LINKMAP     2
#define RTLD_DI_SERINFO     4
#define RTLD_DI_SERINFOSIZE 5
#define RTLD_DI_ORIGIN      6
#define RTLD_DI_MAX         RTLD_DI_ORIGIN

#define RTLD_NEXT    ((void *)-1)
#define RTLD_DEFAULT ((void *)-2)
#define RTLD_SELF    ((void *)-3)

int   dlclose(void *);
char *dlerror(void);
void *dlopen(const char *, int);
void *dlsym(void *__restrict, const char *__restrict);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
typedef struct {
  const char *dli_fname;
  void       *dli_fbase;
  const char *dli_sname;
  void       *dli_saddr;
} Dl_info;
int dladdr(const void *, Dl_info *);
int dlinfo(void *, int, void *);
#endif

#endif
