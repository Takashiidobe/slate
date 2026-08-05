#ifndef _SLATE_SYS_SOCKET_H
#define _SLATE_SYS_SOCKET_H

#include <sys/types.h>

typedef unsigned int socklen_t;
typedef unsigned char sa_family_t;

struct sockaddr {
  sa_family_t sa_family;
  char sa_data[14];
};

struct sockaddr_storage {
  sa_family_t ss_family;
  unsigned long __ss_align;
  char __padding[128 - 2 * sizeof(unsigned long)];
};

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
