#ifndef _SLATE_EXEC_INFO_H
#define _SLATE_EXEC_INFO_H

#include <features.h>

int backtrace(void **__array, int __size);

char **backtrace_symbols(void *const *__array, int __size);

void backtrace_symbols_fd(void *const *__array, int __size, int __fd);

#endif
