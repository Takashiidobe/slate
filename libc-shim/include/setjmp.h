#ifndef _SLATE_SETJMP_H
#define _SLATE_SETJMP_H

#include <features.h>

#define __STDC_VERSION_SETJMP_H__ 202311L

#if defined(__GNUC__) && !defined(__clang__)
#define __setjmp_attr __attribute__((__returns_twice__))
#elif defined(__clang__)
#define __setjmp_attr __attribute__((returns_twice))
#else
#define __setjmp_attr
#endif

/* Opaque storage sized/aligned to match the real glibc x86-64
   struct __jmp_buf_tag (8 saved registers + mask-saved flag + signal
   mask) since the real setjmp/longjmp symbols read and write it. */
typedef struct {
  long          __regs[8];
  int           __mask_was_saved;
  unsigned long __saved_mask[16];
} __slate_jmp_buf_tag;

typedef __slate_jmp_buf_tag jmp_buf[1];

int            setjmp(jmp_buf env) __setjmp_attr;
_Noreturn void longjmp(jmp_buf env, int val);

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
typedef jmp_buf sigjmp_buf;
int            sigsetjmp(sigjmp_buf env, int savemask) __setjmp_attr;
_Noreturn void siglongjmp(sigjmp_buf env, int val);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int            _setjmp(jmp_buf env) __setjmp_attr;
_Noreturn void _longjmp(jmp_buf env, int val);
#endif

#define setjmp setjmp

#undef __setjmp_attr

#endif
