#ifndef _SLATE_DLFCN_H
#define _SLATE_DLFCN_H
#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/dlfcn.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/dlfcn.h>

#else

#define RTLD_LAZY     1
#define RTLD_NOW      2
#define RTLD_NOLOAD   4
#define RTLD_NODELETE 4096
#define RTLD_GLOBAL   256
#define RTLD_LOCAL    0

#define RTLD_NEXT    ((void *)-1)
#define RTLD_DEFAULT ((void *)0)

#define RTLD_DI_LINKMAP 2

#if defined(__SLATE_LIBC_GLIBC)
typedef long Lmid_t;

#define LM_ID_BASE         0
#define LM_ID_NEWLM        (-1L)
#define RTLD_BINDING_MASK  0x3
#define RTLD_DEEPBIND      0x8
#define DL_CALL_FCT(fctp, args) (fctp) args
#define DLFO_FLAG_SFRAME   (1ULL << 0)
#endif

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

#if _REDIR_TIME64
__REDIR(dlsym, __dlsym_time64);
#endif

#endif 

#endif
