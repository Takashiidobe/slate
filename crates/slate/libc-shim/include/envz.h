#ifndef _SLATE_ENVZ_H
#define _SLATE_ENVZ_H
#include <features.h>

#include <argz.h>

#define __NEED_size_t
#define __NEED_error_t
#include <bits/types.h>

char *envz_entry(const char *__restrict __envz, size_t __envz_len,
                 const char *__restrict __name);

char *envz_get(const char *__restrict __envz, size_t __envz_len,
               const char *__restrict __name);

error_t envz_add(char **__restrict __envz, size_t *__restrict __envz_len,
                 const char *__restrict __name, const char *__restrict __value);

error_t envz_merge(char **__restrict __envz, size_t *__restrict __envz_len,
                   const char *__restrict __envz2, size_t __envz2_len,
                   int __override);

void envz_remove(char **__restrict __envz, size_t *__restrict __envz_len,
                 const char *__restrict __name);

void envz_strip(char **__restrict __envz, size_t *__restrict __envz_len);
#endif
