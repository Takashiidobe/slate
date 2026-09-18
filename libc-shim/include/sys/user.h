#ifndef _SLATE_SYS_USER_H
#define _SLATE_SYS_USER_H

#include <limits.h>
#include <stdint.h>

#if defined(__SLATE_LIBC_MUSL)
#include <unistd.h>
#endif

#include <bits/types.h>

#undef __WORDSIZE
#if __LONG_MAX == 0x7fffffffL
#define __WORDSIZE 32
#else
#define __WORDSIZE 64
#endif

#include <bits/user.h>

#if (defined(__SLATE_LIBC_GLIBC) && defined(__SLATE_ARCH_X86_64))
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE-1))
#define NBPG PAGE_SIZE
#define UPAGES 1
#define HOST_TEXT_START_ADDR (u.start_code)
#define HOST_STACK_END_ADDR (u.start_stack + u.u_ssize * NBPG)
#endif
#if defined(__SLATE_LIBC_MUSL) && defined(__SLATE_ARCH_X86_64)
#define NBPG PAGESIZE
#define PAGE_MASK (~(PAGESIZE - 1))
#define UPAGES 1
#define HOST_TEXT_START_ADDR (u.start_code)
#define HOST_STACK_END_ADDR (u.start_stack + u.u_ssize * NBPG)
#endif
#if defined(__SLATE_LIBC_GLIBC) && (defined(__SLATE_ARCH_X86) || defined(__SLATE_ARCH_X86_64))
#define PAGE_SHIFT 12
#endif

#endif
