#include <sys/pidfd.h>

extern int slate_oracle_pidfd_open(int, unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pidfd_open), __typeof__(pidfd_open)),
    "sys/pidfd.h:pidfd_open declaration differs from oracle");

static __typeof__(pidfd_open) *const slate_reference_pidfd_open = &pidfd_open;

#ifndef PIDFD_GET_CGROUP_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_CGROUP_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_IPC_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_IPC_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_MNT_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_MNT_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_NET_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_NET_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_PID_FOR_CHILDREN_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_PID_FOR_CHILDREN_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_PID_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_PID_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_TIME_FOR_CHILDREN_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_TIME_FOR_CHILDREN_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_TIME_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_TIME_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_USER_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_USER_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_GET_UTS_NAMESPACE
#error "sys/pidfd.h:PIDFD_GET_UTS_NAMESPACE macro is missing from libc-shim"
#endif

#ifndef PIDFD_NONBLOCK
#error "sys/pidfd.h:PIDFD_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef PIDFD_SIGNAL_PROCESS_GROUP
#error "sys/pidfd.h:PIDFD_SIGNAL_PROCESS_GROUP macro is missing from libc-shim"
#endif

#ifndef PIDFD_SIGNAL_THREAD
#error "sys/pidfd.h:PIDFD_SIGNAL_THREAD macro is missing from libc-shim"
#endif

#ifndef PIDFD_SIGNAL_THREAD_GROUP
#error "sys/pidfd.h:PIDFD_SIGNAL_THREAD_GROUP macro is missing from libc-shim"
#endif

#ifndef PIDFD_THREAD
#error "sys/pidfd.h:PIDFD_THREAD macro is missing from libc-shim"
#endif

#ifndef PIDFS_IOCTL_MAGIC
#error "sys/pidfd.h:PIDFS_IOCTL_MAGIC macro is missing from libc-shim"
#endif

int main(void) { return 0; }
