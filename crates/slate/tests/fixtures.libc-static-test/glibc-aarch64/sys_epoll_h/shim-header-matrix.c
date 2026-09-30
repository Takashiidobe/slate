#include <sys/epoll.h>

_Static_assert(EPOLLIN == (1), "enum EPOLLIN value differs from oracle");

_Static_assert(EPOLLPRI == (2), "enum EPOLLPRI value differs from oracle");

_Static_assert(EPOLLOUT == (4), "enum EPOLLOUT value differs from oracle");

_Static_assert(EPOLLRDNORM == (64), "enum EPOLLRDNORM value differs from oracle");

_Static_assert(EPOLLRDBAND == (128), "enum EPOLLRDBAND value differs from oracle");

_Static_assert(EPOLLWRNORM == (256), "enum EPOLLWRNORM value differs from oracle");

_Static_assert(EPOLLWRBAND == (512), "enum EPOLLWRBAND value differs from oracle");

_Static_assert(EPOLLMSG == (1024), "enum EPOLLMSG value differs from oracle");

_Static_assert(EPOLLERR == (8), "enum EPOLLERR value differs from oracle");

_Static_assert(EPOLLHUP == (16), "enum EPOLLHUP value differs from oracle");

_Static_assert(EPOLLRDHUP == (8192), "enum EPOLLRDHUP value differs from oracle");

_Static_assert(EPOLLEXCLUSIVE == (268435456), "enum EPOLLEXCLUSIVE value differs from oracle");

_Static_assert(EPOLLWAKEUP == (536870912), "enum EPOLLWAKEUP value differs from oracle");

_Static_assert(EPOLLONESHOT == (1073741824), "enum EPOLLONESHOT value differs from oracle");

_Static_assert(EPOLLET == (2147483648), "enum EPOLLET value differs from oracle");

#ifndef EPIOCGPARAMS
#error "sys/epoll.h:EPIOCGPARAMS macro is missing from libc-shim"
#endif

#ifndef EPIOCSPARAMS
#error "sys/epoll.h:EPIOCSPARAMS macro is missing from libc-shim"
#endif

#ifndef EPOLLERR
#error "sys/epoll.h:EPOLLERR macro is missing from libc-shim"
#endif

#ifndef EPOLLET
#error "sys/epoll.h:EPOLLET macro is missing from libc-shim"
#endif

#ifndef EPOLLEXCLUSIVE
#error "sys/epoll.h:EPOLLEXCLUSIVE macro is missing from libc-shim"
#endif

#ifndef EPOLLHUP
#error "sys/epoll.h:EPOLLHUP macro is missing from libc-shim"
#endif

#ifndef EPOLLIN
#error "sys/epoll.h:EPOLLIN macro is missing from libc-shim"
#endif

#ifndef EPOLLMSG
#error "sys/epoll.h:EPOLLMSG macro is missing from libc-shim"
#endif

#ifndef EPOLLONESHOT
#error "sys/epoll.h:EPOLLONESHOT macro is missing from libc-shim"
#endif

#ifndef EPOLLOUT
#error "sys/epoll.h:EPOLLOUT macro is missing from libc-shim"
#endif

#ifndef EPOLLPRI
#error "sys/epoll.h:EPOLLPRI macro is missing from libc-shim"
#endif

#ifndef EPOLLRDBAND
#error "sys/epoll.h:EPOLLRDBAND macro is missing from libc-shim"
#endif

#ifndef EPOLLRDHUP
#error "sys/epoll.h:EPOLLRDHUP macro is missing from libc-shim"
#endif

#ifndef EPOLLRDNORM
#error "sys/epoll.h:EPOLLRDNORM macro is missing from libc-shim"
#endif

#ifndef EPOLLWAKEUP
#error "sys/epoll.h:EPOLLWAKEUP macro is missing from libc-shim"
#endif

#ifndef EPOLLWRBAND
#error "sys/epoll.h:EPOLLWRBAND macro is missing from libc-shim"
#endif

#ifndef EPOLLWRNORM
#error "sys/epoll.h:EPOLLWRNORM macro is missing from libc-shim"
#endif

#ifndef EPOLL_CLOEXEC
#error "sys/epoll.h:EPOLL_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef EPOLL_CTL_ADD
#error "sys/epoll.h:EPOLL_CTL_ADD macro is missing from libc-shim"
#endif

#ifndef EPOLL_CTL_DEL
#error "sys/epoll.h:EPOLL_CTL_DEL macro is missing from libc-shim"
#endif

#ifndef EPOLL_CTL_MOD
#error "sys/epoll.h:EPOLL_CTL_MOD macro is missing from libc-shim"
#endif

#ifndef EPOLL_IOC_TYPE
#error "sys/epoll.h:EPOLL_IOC_TYPE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
