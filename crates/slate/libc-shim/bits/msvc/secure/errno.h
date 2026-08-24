#ifndef _SLATE_BITS_MSVC_SECURE_ERRNO_H
#define _SLATE_BITS_MSVC_SECURE_ERRNO_H

#include <bits/msvc/types.h>

errno_t       _set_errno(int);
errno_t       _get_errno(int *);
unsigned long *__doserrno(void);
errno_t       _set_doserrno(unsigned long);
errno_t       _get_doserrno(unsigned long *);

#endif
