#ifndef _SLATE_PRINTF_H
#define _SLATE_PRINTF_H

#include <features.h>
#include <stddef.h>

size_t parse_printf_format(const char *__restrict, size_t, int *__restrict);

enum {
  PA_INT,
  PA_CHAR,
  PA_WCHAR,
  PA_STRING,
  PA_WSTRING,
  PA_POINTER,
  PA_FLOAT,
  PA_DOUBLE,
  PA_LAST
};

#define PA_FLAG_MASK 0xff00
#define PA_FLAG_LONG_LONG (1 << 8)
#define PA_FLAG_LONG_DOUBLE PA_FLAG_LONG_LONG
#define PA_FLAG_LONG (1 << 9)
#define PA_FLAG_SHORT (1 << 10)
#define PA_FLAG_PTR (1 << 11)

#endif
