#ifndef _SLATE_ARGZ_H
#define _SLATE_ARGZ_H

#include <features.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char  *argz_create_sep(const char *string, int sep, char **argz, size_t *len);
size_t argz_count(const char *argz, size_t len);
void   argz_add(char **argz, size_t *len, const char *str);
void   argz_add_sep(char **argz, size_t *len, const char *str, int sep);
void   argz_append(char **argz, size_t *len, const char *buf, size_t buflen);
int argz_replace(char **argz, size_t *len, const char *str, const char *with);

#ifdef __cplusplus
}
#endif

#endif /* _SLATE_ARGZ_H */
