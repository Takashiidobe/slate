#ifndef _SLATE_ERROR_H
#define _SLATE_ERROR_H

#include <features.h>

#if !defined(__SLATE_LIBC_GLIBC)
#error "<error.h> is only available with glibc"
#endif

void error(int status, int errnum, const char *format, ...);
void error_at_line(int status, int errnum, const char *filename,
                    unsigned int lineno, const char *format, ...);

extern unsigned int error_message_count;
extern int          error_one_per_line;
extern void (*error_print_progname)(void);

#endif
