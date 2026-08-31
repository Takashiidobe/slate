#ifndef _SLATE_BITS_DARWIN_UCONTEXT_H
#define _SLATE_BITS_DARWIN_UCONTEXT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/ucontext.h> directly; include a public header instead."
#endif

#include <signal.h>

struct __darwin_mcontext64;

typedef struct __darwin_ucontext {
  int                       uc_onstack;
  sigset_t                  uc_sigmask;
  stack_t                   uc_stack;
  struct __darwin_ucontext *uc_link;
  size_t                    uc_mcsize;
  struct __darwin_mcontext64 *uc_mcontext;
} ucontext_t;

#endif
