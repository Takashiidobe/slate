#ifndef _SLATE_SYS_VM86_H
#define _SLATE_SYS_VM86_H

#if defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_ARCH_X86)
#include <bits/x86/vm86.h>
int vm86(unsigned long, struct vm86plus_struct *);
#endif

#endif
