#ifndef _SLATE_FPU_CONTROL_H
#define _SLATE_FPU_CONTROL_H

#include <features.h>

#if !defined(__SLATE_LIBC_GLIBC)
#error "<fpu_control.h> is only available with glibc"
#endif

#if defined(__SLATE_ARCH_AARCH64)
#include <sys/types.h>
#endif

#if defined(__SLATE_ARCH_X86) || defined(__SLATE_ARCH_X86_64)
typedef unsigned short fpu_control_t;
#else
typedef unsigned int fpu_control_t;
#endif

extern fpu_control_t __fpu_control;

#endif
