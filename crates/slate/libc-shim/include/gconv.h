#ifndef _GCONV_H
#define _GCONV_H 1

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)

#define __NEED_wint_t
#include <bits/types.h>

#define __need_size_t
#define __need_wchar_t
#include <stddef.h>

#define __UNKNOWN_10646_CHAR ((wchar_t)0xfffd)

enum {
  __GCONV_OK = 0,
  __GCONV_NOCONV,
  __GCONV_NODB,
  __GCONV_NOMEM,

  __GCONV_EMPTY_INPUT,
  __GCONV_FULL_OUTPUT,
  __GCONV_ILLEGAL_INPUT,
  __GCONV_INCOMPLETE_INPUT,

  __GCONV_ILLEGAL_DESCRIPTOR,
  __GCONV_INTERNAL_ERROR
};

enum {
  __GCONV_IS_LAST       = 0x0001,
  __GCONV_IGNORE_ERRORS = 0x0002,
  __GCONV_SWAP          = 0x0004,
  __GCONV_TRANSLIT      = 0x0008
};

struct __gconv_step;
struct __gconv_step_data;
struct __gconv_loaded_object;

typedef int (*__gconv_fct)(struct __gconv_step *, struct __gconv_step_data *,
                           const unsigned char **, const unsigned char *,
                           unsigned char **, size_t *, int, int);

typedef wint_t (*__gconv_btowc_fct)(struct __gconv_step *, unsigned char);

typedef int (*__gconv_init_fct)(struct __gconv_step *);
typedef void (*__gconv_end_fct)(struct __gconv_step *);

struct __gconv_step {
  struct __gconv_loaded_object *__shlib_handle;
  const char                   *__modname;

  int __counter;

  char *__from_name;
  char *__to_name;

  __gconv_fct       __fct;
  __gconv_btowc_fct __btowc_fct;
  __gconv_init_fct  __init_fct;
  __gconv_end_fct   __end_fct;

  int __min_needed_from;
  int __max_needed_from;
  int __min_needed_to;
  int __max_needed_to;

  int __stateful;

  void *__data;
};

struct __gconv_step_data {
  unsigned char *__outbuf;
  unsigned char *__outbufend;

  int __flags;

  int __invocation_counter;

  int __internal_use;

  __mbstate_t *__statep;
  __mbstate_t  __state;
};

typedef struct __gconv_info {
  size_t               __nsteps;
  struct __gconv_step *__steps;
  __extension__ struct __gconv_step_data __data[0];
} *__gconv_t;

#endif

#endif
