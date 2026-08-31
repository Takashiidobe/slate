#ifndef _SLATE_BITS_FREEBSD_NETDB_H
#define _SLATE_BITS_FREEBSD_NETDB_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/netdb.h> directly; include a public header instead."
#endif

struct addrinfo {
  int               ai_flags;
  int               ai_family;
  int               ai_socktype;
  int               ai_protocol;
  socklen_t         ai_addrlen;
  char             *ai_canonname;
  struct sockaddr  *ai_addr;
  struct addrinfo  *ai_next;
};

#define AI_PASSIVE      0x00000001
#define AI_CANONNAME    0x00000002
#define AI_NUMERICHOST  0x00000004
#define AI_NUMERICSERV  0x00000008
#define AI_ALL          0x00000100
#define AI_V4MAPPED_CFG 0x00000200
#define AI_ADDRCONFIG   0x00000400
#define AI_V4MAPPED     0x00000800

#define NI_MAXHOST 1025
#define NI_MAXSERV 32

#define NI_NOFQDN       0x00000001
#define NI_NUMERICHOST  0x00000002
#define NI_NAMEREQD     0x00000004
#define NI_NUMERICSERV  0x00000008
#define NI_DGRAM        0x00000010
#define NI_NUMERICSCOPE 0x00000020

#define EAI_ADDRFAMILY 1
#define EAI_AGAIN      2
#define EAI_BADFLAGS   3
#define EAI_FAIL       4
#define EAI_FAMILY     5
#define EAI_MEMORY     6
#define EAI_NODATA     7
#define EAI_NONAME     8
#define EAI_SERVICE    9
#define EAI_SOCKTYPE   10
#define EAI_SYSTEM     11
#define EAI_BADHINTS   12
#define EAI_PROTOCOL   13
#define EAI_OVERFLOW   14
#define EAI_MAX        15

int  getaddrinfo(const char *__restrict, const char *__restrict,
                 const struct addrinfo *__restrict,
                 struct addrinfo **__restrict);
void freeaddrinfo(struct addrinfo *);
int  getnameinfo(const struct sockaddr *__restrict, socklen_t, char *__restrict,
                 socklen_t, char *__restrict, socklen_t, int);
const char *gai_strerror(int);

struct netent {
  char    *n_name;
  char   **n_aliases;
  int      n_addrtype;
  uint32_t n_net;
};

struct hostent {
  char  *h_name;
  char **h_aliases;
  int    h_addrtype;
  int    h_length;
  char **h_addr_list;
};
#define h_addr h_addr_list[0]

struct servent {
  char  *s_name;
  char **s_aliases;
  int    s_port;
  char  *s_proto;
};

struct protoent {
  char  *p_name;
  char **p_aliases;
  int    p_proto;
};

void            sethostent(int);
void            endhostent(void);
struct hostent *gethostent(void);

void           setnetent(int);
void           endnetent(void);
struct netent *getnetent(void);
struct netent *getnetbyaddr(uint32_t, int);
struct netent *getnetbyname(const char *);

void            setservent(int);
void            endservent(void);
struct servent *getservent(void);
struct servent *getservbyname(const char *, const char *);
struct servent *getservbyport(int, const char *);

void             setprotoent(int);
void             endprotoent(void);
struct protoent *getprotoent(void);
struct protoent *getprotobyname(const char *);
struct protoent *getprotobynumber(int);

struct hostent *gethostbyname(const char *);
struct hostent *gethostbyaddr(const void *, socklen_t, int);
#ifdef __GNUC__
__attribute__((const))
#endif
int *__h_errno_location(void);
#define h_errno        (*__h_errno_location())
#define HOST_NOT_FOUND 1
#define TRY_AGAIN      2
#define NO_RECOVERY    3
#define NO_DATA        4
#define NO_ADDRESS     NO_DATA

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
void        herror(const char *);
const char *hstrerror(int);
int         gethostbyname_r(const char *, struct hostent *, char *, size_t,
                            struct hostent **, int *);
int gethostbyname2_r(const char *, int, struct hostent *, char *, size_t,
                     struct hostent **, int *);
struct hostent *gethostbyname2(const char *, int);
int gethostbyaddr_r(const void *, socklen_t, int, struct hostent *, char *,
                    size_t, struct hostent **, int *);
int getservbyport_r(int, const char *, struct servent *, char *, size_t,
                    struct servent **);
int getservbyname_r(const char *, const char *, struct servent *, char *,
                    size_t, struct servent **);
#endif

#endif
