#ifndef _SLATE_BITS_FREEBSD_SOCKET_H
#define _SLATE_BITS_FREEBSD_SOCKET_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/socket.h> directly; include a public header instead."
#endif

struct sockaddr {
  unsigned char sa_len;
  sa_family_t   sa_family;
  char          sa_data[14];
};

#define _SS_MAXSIZE   128
#define _SS_ALIGNSIZE (sizeof(long long))
#define _SS_PAD1SIZE  (_SS_ALIGNSIZE - sizeof(unsigned char) - sizeof(sa_family_t))
#define _SS_PAD2SIZE                                                          \
  (_SS_MAXSIZE - sizeof(unsigned char) - sizeof(sa_family_t) - _SS_PAD1SIZE - \
   _SS_ALIGNSIZE)

struct sockaddr_storage {
  unsigned char ss_len;
  sa_family_t   ss_family;
  char          __ss_pad1[_SS_PAD1SIZE];
  long long     __ss_align;
  char          __ss_pad2[_SS_PAD2SIZE];
};

struct msghdr {
  void         *msg_name;
  socklen_t     msg_namelen;
  struct iovec *msg_iov;
  int           msg_iovlen;
  void         *msg_control;
  socklen_t     msg_controllen;
  int           msg_flags;
};

struct cmsghdr {
  socklen_t cmsg_len;
  int       cmsg_level;
  int       cmsg_type;
};

struct linger {
  int l_onoff;
  int l_linger;
};

#define SOCK_STREAM    1
#define SOCK_DGRAM     2
#define SOCK_RAW       3
#define SOCK_RDM       4
#define SOCK_SEQPACKET 5
#define SOCK_CLOEXEC   0x10000000
#define SOCK_NONBLOCK  0x20000000

#define PF_UNSPEC    0
#define PF_LOCAL     1
#define PF_UNIX      PF_LOCAL
#define PF_INET      2
#define PF_ROUTE     17
#define PF_LINK      18
#define PF_INET6     28
#define PF_MAX       42

#define AF_UNSPEC PF_UNSPEC
#define AF_LOCAL  PF_LOCAL
#define AF_UNIX   AF_LOCAL
#define AF_INET   PF_INET
#define AF_ROUTE  PF_ROUTE
#define AF_LINK   PF_LINK
#define AF_INET6  PF_INET6
#define AF_MAX    PF_MAX

#define SO_DEBUG       0x00000001
#define SO_ACCEPTCONN  0x00000002
#define SO_REUSEADDR   0x00000004
#define SO_KEEPALIVE   0x00000008
#define SO_DONTROUTE   0x00000010
#define SO_BROADCAST   0x00000020
#define SO_USELOOPBACK 0x00000040
#define SO_LINGER      0x00000080
#define SO_OOBINLINE   0x00000100
#define SO_REUSEPORT   0x00000200
#define SO_TIMESTAMP   0x00000400
#define SO_NOSIGPIPE   0x00000800
#define SO_ACCEPTFILTER 0x00001000

#define SO_SNDBUF   0x1001
#define SO_RCVBUF   0x1002
#define SO_SNDLOWAT 0x1003
#define SO_RCVLOWAT 0x1004
#define SO_SNDTIMEO 0x1005
#define SO_RCVTIMEO 0x1006
#define SO_ERROR    0x1007
#define SO_TYPE     0x1008
#define SO_PROTOCOL 0x1016
#define SO_PROTOTYPE SO_PROTOCOL
#define SO_DOMAIN   0x1019

#define SOL_SOCKET 0xffff

#define SOMAXCONN 128

#define MSG_OOB          0x00000001
#define MSG_PEEK         0x00000002
#define MSG_DONTROUTE    0x00000004
#define MSG_EOR          0x00000008
#define MSG_TRUNC        0x00000010
#define MSG_CTRUNC       0x00000020
#define MSG_WAITALL      0x00000040
#define MSG_DONTWAIT     0x00000080
#define MSG_NOSIGNAL     0x00020000
#define MSG_CMSG_CLOEXEC 0x00040000

#define SCM_RIGHTS 0x01

#define SHUT_RD   0
#define SHUT_WR   1
#define SHUT_RDWR 2

#define _ALIGNBYTES  (sizeof(long) - 1)
#define _ALIGN(p)    (((unsigned long)(p) + _ALIGNBYTES) & ~_ALIGNBYTES)

#define CMSG_DATA(cmsg) ((unsigned char *)(cmsg) + _ALIGN(sizeof(struct cmsghdr)))
#define CMSG_FIRSTHDR(mhdr)                                                    \
  ((size_t)(mhdr)->msg_controllen >= sizeof(struct cmsghdr)                    \
       ? (struct cmsghdr *)(mhdr)->msg_control                                 \
       : (struct cmsghdr *)0)
#define CMSG_NXTHDR(mhdr, cmsg)                                                \
  ((char *)(cmsg) == (char *)0 ? CMSG_FIRSTHDR(mhdr)                           \
                                : (((char *)(cmsg) +                           \
                                    _ALIGN(((struct cmsghdr *)(cmsg))->cmsg_len) + \
                                    _ALIGN(sizeof(struct cmsghdr)) >            \
                                    (char *)(mhdr)->msg_control +               \
                                        (mhdr)->msg_controllen)                 \
                                       ? (struct cmsghdr *)0                    \
                                       : (struct cmsghdr *)(void *)((char *)(cmsg) + \
                                             _ALIGN(((struct cmsghdr *)(cmsg))->cmsg_len))))
#define CMSG_SPACE(l) (_ALIGN(sizeof(struct cmsghdr)) + _ALIGN(l))
#define CMSG_LEN(l)   (_ALIGN(sizeof(struct cmsghdr)) + (l))

int socket(int, int, int);
int socketpair(int, int, int, int[2]);

int shutdown(int, int);

int bind(int, const struct sockaddr *, socklen_t);
int connect(int, const struct sockaddr *, socklen_t);
int listen(int, int);
int accept(int, struct sockaddr *__restrict, socklen_t *__restrict);
int accept4(int, struct sockaddr *__restrict, socklen_t *__restrict, int);

int getsockname(int, struct sockaddr *__restrict, socklen_t *__restrict);
int getpeername(int, struct sockaddr *__restrict, socklen_t *__restrict);

ssize_t send(int, const void *, size_t, int);
ssize_t recv(int, void *, size_t, int);
ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *,
               socklen_t);
ssize_t recvfrom(int, void *__restrict, size_t, int,
                 struct sockaddr *__restrict, socklen_t *__restrict);
ssize_t sendmsg(int, const struct msghdr *, int);
ssize_t recvmsg(int, struct msghdr *, int);

int getsockopt(int, int, int, void *__restrict, socklen_t *__restrict);
int setsockopt(int, int, int, const void *, socklen_t);

int sockatmark(int);

#endif
