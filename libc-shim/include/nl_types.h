#ifndef _SLATE_NL_TYPES_H
#define _SLATE_NL_TYPES_H

#define NL_SETD       1
#define NL_CAT_LOCALE 1

typedef int   nl_item;
typedef void *nl_catd;

nl_catd catopen(const char *, int);
char   *catgets(nl_catd, int, int, const char *);
int     catclose(nl_catd);

#endif
