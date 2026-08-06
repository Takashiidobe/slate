#ifndef _SLATE_ALIASES_H
#define _SLATE_ALIASES_H

#include <features.h>

#include <sys/types.h>

struct aliasent {
  char  *alias_name;
  size_t alias_members_len;
  char **alias_members;
  int    alias_local;
};

void setaliasent(void);

void endaliasent(void);

struct aliasent *getaliasent(void);

int getaliasent_r(struct aliasent *__restrict __result_buf,
                  char *__restrict __buffer, size_t __buflen,
                  struct aliasent **__restrict __result);

struct aliasent *getaliasbyname(const char *__name);

int getaliasbyname_r(const char *__restrict __name,
                     struct aliasent *__restrict __result_buf,
                     char *__restrict __buffer, size_t __buflen,
                     struct aliasent **__restrict __result);

#endif /* aliases.h */
