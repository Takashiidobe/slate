#ifndef _SLATE_PRINTF_H
#define _SLATE_PRINTF_H

#if defined(__SLATE_LIBC_GLIBC)
#include <features.h>
#include <stdarg.h>
#include <stddef.h>
#define __NEED_wchar_t
#define __NEED_FILE
#define __NEED_size_t
#define __NEED_va_list
#include <bits/types.h>

struct printf_info {
  int prec;
  int width;
  wchar_t spec;
  unsigned int is_long_double : 1;
  unsigned int is_short : 1;
  unsigned int is_long : 1;
  unsigned int alt : 1;
  unsigned int space : 1;
  unsigned int left : 1;
  unsigned int showsign : 1;
  unsigned int group : 1;
  unsigned int extra : 1;
  unsigned int is_char : 1;
  unsigned int wide : 1;
  unsigned int i18n : 1;
  unsigned int is_binary128 : 1;
  unsigned int __pad : 3;
  unsigned short user;
  wchar_t pad;
};

typedef int printf_function(FILE *, const struct printf_info *, const void *const *);
typedef int printf_arginfo_size_function(const struct printf_info *, size_t, int *, int *);
typedef int printf_arginfo_function(const struct printf_info *, size_t, int *);
typedef void printf_va_arg_function(void *, va_list *);

int register_printf_specifier(int, printf_function, printf_arginfo_size_function);
int register_printf_function(int, printf_function, printf_arginfo_function);
int register_printf_modifier(const wchar_t *);
int register_printf_type(printf_va_arg_function);
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

int printf_size(FILE *__restrict, const struct printf_info *, const void *const *__restrict);
int printf_size_info(const struct printf_info *__restrict, size_t, int *__restrict);
#endif

#endif
