#ifndef _SLATE_RPC_NETDB_H
#define _SLATE_RPC_NETDB_H

#include <features.h>
#include <stddef.h>

struct rpcent { char *r_name; char **r_aliases; int r_number; };
void setrpcent(int);
void endrpcent(void);
struct rpcent *getrpcbyname(const char *);
struct rpcent *getrpcbynumber(int);
struct rpcent *getrpcent(void);
#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
int getrpcbyname_r(const char *, struct rpcent *, char *, size_t, struct rpcent **);
int getrpcbynumber_r(int, struct rpcent *, char *, size_t, struct rpcent **);
int getrpcent_r(struct rpcent *, char *, size_t, struct rpcent **);
#endif

#endif
