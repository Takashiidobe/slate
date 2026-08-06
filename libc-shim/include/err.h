#ifndef _SLATE_ERR_H
#define _SLATE_ERR_H

#include <features.h>
#include <stdarg.h>

void warn(const char *, ...);
void vwarn(const char *, va_list);
void warnx(const char *, ...);
void vwarnx(const char *, va_list);

_Noreturn void err(int, const char *, ...);
_Noreturn void verr(int, const char *, va_list);
_Noreturn void errx(int, const char *, ...);
_Noreturn void verrx(int, const char *, va_list);

#endif
