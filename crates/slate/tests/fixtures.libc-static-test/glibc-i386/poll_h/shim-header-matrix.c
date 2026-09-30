#include <poll.h>

typedef unsigned long slate_oracle_typedef_nfds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_nfds_t, nfds_t), "typedef nfds_t differs from oracle");

#ifndef POLLERR
#error "poll.h:POLLERR macro is missing from libc-shim"
#endif

#ifndef POLLHUP
#error "poll.h:POLLHUP macro is missing from libc-shim"
#endif

#ifndef POLLIN
#error "poll.h:POLLIN macro is missing from libc-shim"
#endif

#ifndef POLLMSG
#error "poll.h:POLLMSG macro is missing from libc-shim"
#endif

#ifndef POLLNVAL
#error "poll.h:POLLNVAL macro is missing from libc-shim"
#endif

#ifndef POLLOUT
#error "poll.h:POLLOUT macro is missing from libc-shim"
#endif

#ifndef POLLPRI
#error "poll.h:POLLPRI macro is missing from libc-shim"
#endif

#ifndef POLLRDBAND
#error "poll.h:POLLRDBAND macro is missing from libc-shim"
#endif

#ifndef POLLRDHUP
#error "poll.h:POLLRDHUP macro is missing from libc-shim"
#endif

#ifndef POLLRDNORM
#error "poll.h:POLLRDNORM macro is missing from libc-shim"
#endif

#ifndef POLLREMOVE
#error "poll.h:POLLREMOVE macro is missing from libc-shim"
#endif

#ifndef POLLWRBAND
#error "poll.h:POLLWRBAND macro is missing from libc-shim"
#endif

#ifndef POLLWRNORM
#error "poll.h:POLLWRNORM macro is missing from libc-shim"
#endif

int main(void) { return 0; }
