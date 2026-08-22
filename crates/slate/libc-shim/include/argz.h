#ifndef _SLATE_ARGZ_H
#define _SLATE_ARGZ_H

#include <features.h>
#include <stddef.h>

#ifndef __error_t_defined
#define __error_t_defined 1
typedef int error_t;
#endif

error_t argz_create(char *const argv[], char **__restrict argz,
                    size_t *__restrict len);
error_t argz_create_sep(const char *__restrict string, int sep,
                        char **__restrict argz, size_t *__restrict len);
size_t  argz_count(const char *argz, size_t len);
void    argz_extract(const char *__restrict argz, size_t len,
                     char **__restrict argv);
void    argz_stringify(char *argz, size_t len, int sep);
error_t argz_append(char **__restrict argz, size_t *__restrict argz_len,
                    const char *__restrict buf, size_t buf_len);
error_t argz_add(char **__restrict argz, size_t *__restrict argz_len,
                 const char *__restrict str);
error_t argz_add_sep(char **__restrict argz, size_t *__restrict argz_len,
                     const char *__restrict string, int delim);
void    argz_delete(char **__restrict argz, size_t *__restrict argz_len,
                    char *__restrict entry);
error_t argz_insert(char **__restrict argz, size_t *__restrict argz_len,
                    char *__restrict before, const char *__restrict entry);
error_t argz_replace(char **__restrict argz, size_t *__restrict argz_len,
                     const char *__restrict str, const char *__restrict with,
                     unsigned int *__restrict replace_count);
char   *argz_next(const char *__restrict argz, size_t argz_len,
                  const char *__restrict entry);

#endif /* _SLATE_ARGZ_H */
