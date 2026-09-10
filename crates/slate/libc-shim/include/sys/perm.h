#ifndef _SYS_PERM_H
#define _SYS_PERM_H
#if defined(__SLATE_LIBC_GLIBC)
int ioperm(unsigned long, unsigned long, int);
int iopl(int);
#endif
#endif
