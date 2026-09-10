#ifndef _SLATE_SETJMP_H
#define _SLATE_SETJMP_H

#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/setjmp.h>

#else

#define __STDC_VERSION_SETJMP_H__ 202311L

#if defined(__GNUC__) && !defined(__clang__)
#define __setjmp_attr __attribute__((__returns_twice__))
#elif defined(__clang__)
#define __setjmp_attr __attribute__((returns_twice))
#else
#define __setjmp_attr
#endif

#if defined(__SLATE_ARCH_X86_64)
typedef struct {
  unsigned long __storage[25];
} __slate_jmp_buf_tag;
#elif defined(__SLATE_ARCH_X86)
typedef struct {
  unsigned long __storage[39];
} __slate_jmp_buf_tag;
#elif defined(__SLATE_ARCH_ARM)
typedef struct {
  unsigned long long __storage[49];
} __slate_jmp_buf_tag;
#elif defined(__SLATE_ARCH_AARCH64)
typedef struct {
  unsigned long __storage[39];
} __slate_jmp_buf_tag;
#else
typedef struct {
  long long          __regs[8];
  int                __mask_was_saved;
  unsigned long long __saved_mask[16];
} __slate_jmp_buf_tag;
#endif

typedef __slate_jmp_buf_tag jmp_buf[1];

int            setjmp(jmp_buf env) __setjmp_attr;
_Noreturn void longjmp(jmp_buf env, int val);

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
typedef jmp_buf sigjmp_buf;
int             sigsetjmp(sigjmp_buf env, int savemask) __setjmp_attr;
_Noreturn void  siglongjmp(sigjmp_buf env, int val);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int            _setjmp(jmp_buf env) __setjmp_attr;
_Noreturn void _longjmp(jmp_buf env, int val);
#endif

#define setjmp setjmp
#if defined(__SLATE_LIBC_GLIBC)
#define sigsetjmp __sigsetjmp (env, savemask)
#endif

#undef __setjmp_attr

#endif

#endif
