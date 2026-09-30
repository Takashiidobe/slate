#include <sys/poll.h>

typedef unsigned long slate_oracle_typedef_nfds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_nfds_t, nfds_t), "typedef nfds_t differs from oracle");

#ifndef POLLERR
#error "sys/poll.h:POLLERR macro is missing from libc-shim"
#endif

#ifndef POLLHUP
#error "sys/poll.h:POLLHUP macro is missing from libc-shim"
#endif

#ifndef POLLIN
#error "sys/poll.h:POLLIN macro is missing from libc-shim"
#endif

#ifndef POLLMSG
#error "sys/poll.h:POLLMSG macro is missing from libc-shim"
#endif

#ifndef POLLNVAL
#error "sys/poll.h:POLLNVAL macro is missing from libc-shim"
#endif

#ifndef POLLOUT
#error "sys/poll.h:POLLOUT macro is missing from libc-shim"
#endif

#ifndef POLLPRI
#error "sys/poll.h:POLLPRI macro is missing from libc-shim"
#endif

#ifndef POLLRDBAND
#error "sys/poll.h:POLLRDBAND macro is missing from libc-shim"
#endif

#ifndef POLLRDHUP
#error "sys/poll.h:POLLRDHUP macro is missing from libc-shim"
#endif

#ifndef POLLRDNORM
#error "sys/poll.h:POLLRDNORM macro is missing from libc-shim"
#endif

#ifndef POLLREMOVE
#error "sys/poll.h:POLLREMOVE macro is missing from libc-shim"
#endif

#ifndef POLLWRBAND
#error "sys/poll.h:POLLWRBAND macro is missing from libc-shim"
#endif

#ifndef POLLWRNORM
#error "sys/poll.h:POLLWRNORM macro is missing from libc-shim"
#endif

int main(void) { return 0; }
