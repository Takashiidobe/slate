#ifndef _SLATE_SYS_USER_H
#define _SLATE_SYS_USER_H

#include <limits.h>
#include <stdint.h>
#include <unistd.h>

#include <bits/types.h>

#undef __WORDSIZE
#if __LONG_MAX == 0x7fffffffL
#define __WORDSIZE 32
#else
#define __WORDSIZE 64
#endif

#include <bits/user.h>

#endif
