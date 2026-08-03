#ifndef _SLATE_SETJMP_H
#define _SLATE_SETJMP_H

#define __STDC_VERSION_SETJMP_H__ 202311L

/* Opaque storage sized/aligned to match the real glibc x86-64
   struct __jmp_buf_tag (8 saved registers + mask-saved flag + signal
   mask) since the real setjmp/longjmp symbols read and write it. */
typedef struct {
  long __regs[8];
  int __mask_was_saved;
  unsigned long __saved_mask[16];
} __slate_jmp_buf_tag;

typedef __slate_jmp_buf_tag jmp_buf[1];

extern int setjmp(jmp_buf env);
extern _Noreturn void longjmp(jmp_buf env, int val);

#endif
