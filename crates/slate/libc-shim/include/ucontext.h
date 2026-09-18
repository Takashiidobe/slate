#ifndef _SLATE_UCONTEXT_H
#define _SLATE_UCONTEXT_H

#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)
#error "ucontext.h process-context interfaces are unavailable for the Darwin libc profile; use sys/ucontext.h for ucontext_t"
#endif

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/ucontext.h>
#else
#include <signal.h>
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define NGREG (sizeof(gregset_t) / sizeof(greg_t))
#endif

struct __ucontext;

int  getcontext(ucontext_t *);
void makecontext(ucontext_t *, void (*)(void), int, ...);
int  setcontext(const ucontext_t *);
int  swapcontext(ucontext_t *, const ucontext_t *);

#endif
