#include <pthread.h>

extern int slate_oracle_pthread_create(struct __pthread * *restrict, const pthread_attr_t *restrict, void *(*)(void *), void *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pthread_create), __typeof__(pthread_create)),
    "pthread.h:pthread_create declaration differs from oracle");

static __typeof__(pthread_create) *const slate_reference_pthread_create = &pthread_create;

#ifndef PTHREAD_BARRIER_SERIAL_THREAD
#error "pthread.h:PTHREAD_BARRIER_SERIAL_THREAD macro is missing from libc-shim"
#endif

#ifndef PTHREAD_CANCELED
#error "pthread.h:PTHREAD_CANCELED macro is missing from libc-shim"
#endif

#ifndef PTHREAD_CANCEL_ASYNCHRONOUS
#error "pthread.h:PTHREAD_CANCEL_ASYNCHRONOUS macro is missing from libc-shim"
#endif

#ifndef PTHREAD_CANCEL_DEFERRED
#error "pthread.h:PTHREAD_CANCEL_DEFERRED macro is missing from libc-shim"
#endif

#ifndef PTHREAD_CANCEL_DISABLE
#error "pthread.h:PTHREAD_CANCEL_DISABLE macro is missing from libc-shim"
#endif

#ifndef PTHREAD_CANCEL_ENABLE
#error "pthread.h:PTHREAD_CANCEL_ENABLE macro is missing from libc-shim"
#endif

#ifndef PTHREAD_CANCEL_MASKED
#error "pthread.h:PTHREAD_CANCEL_MASKED macro is missing from libc-shim"
#endif

#ifndef PTHREAD_COND_INITIALIZER
#error "pthread.h:PTHREAD_COND_INITIALIZER macro is missing from libc-shim"
#endif

#ifndef PTHREAD_CREATE_DETACHED
#error "pthread.h:PTHREAD_CREATE_DETACHED macro is missing from libc-shim"
#endif

#ifndef PTHREAD_CREATE_JOINABLE
#error "pthread.h:PTHREAD_CREATE_JOINABLE macro is missing from libc-shim"
#endif

#ifndef PTHREAD_EXPLICIT_SCHED
#error "pthread.h:PTHREAD_EXPLICIT_SCHED macro is missing from libc-shim"
#endif

#ifndef PTHREAD_INHERIT_SCHED
#error "pthread.h:PTHREAD_INHERIT_SCHED macro is missing from libc-shim"
#endif

#ifndef PTHREAD_MUTEX_DEFAULT
#error "pthread.h:PTHREAD_MUTEX_DEFAULT macro is missing from libc-shim"
#endif

#ifndef PTHREAD_MUTEX_ERRORCHECK
#error "pthread.h:PTHREAD_MUTEX_ERRORCHECK macro is missing from libc-shim"
#endif

#ifndef PTHREAD_MUTEX_INITIALIZER
#error "pthread.h:PTHREAD_MUTEX_INITIALIZER macro is missing from libc-shim"
#endif

#ifndef PTHREAD_MUTEX_NORMAL
#error "pthread.h:PTHREAD_MUTEX_NORMAL macro is missing from libc-shim"
#endif

#ifndef PTHREAD_MUTEX_RECURSIVE
#error "pthread.h:PTHREAD_MUTEX_RECURSIVE macro is missing from libc-shim"
#endif

#ifndef PTHREAD_MUTEX_ROBUST
#error "pthread.h:PTHREAD_MUTEX_ROBUST macro is missing from libc-shim"
#endif

#ifndef PTHREAD_MUTEX_STALLED
#error "pthread.h:PTHREAD_MUTEX_STALLED macro is missing from libc-shim"
#endif

#ifndef PTHREAD_NULL
#error "pthread.h:PTHREAD_NULL macro is missing from libc-shim"
#endif

#ifndef PTHREAD_ONCE_INIT
#error "pthread.h:PTHREAD_ONCE_INIT macro is missing from libc-shim"
#endif

#ifndef PTHREAD_PRIO_INHERIT
#error "pthread.h:PTHREAD_PRIO_INHERIT macro is missing from libc-shim"
#endif

#ifndef PTHREAD_PRIO_NONE
#error "pthread.h:PTHREAD_PRIO_NONE macro is missing from libc-shim"
#endif

#ifndef PTHREAD_PRIO_PROTECT
#error "pthread.h:PTHREAD_PRIO_PROTECT macro is missing from libc-shim"
#endif

#ifndef PTHREAD_PROCESS_PRIVATE
#error "pthread.h:PTHREAD_PROCESS_PRIVATE macro is missing from libc-shim"
#endif

#ifndef PTHREAD_PROCESS_SHARED
#error "pthread.h:PTHREAD_PROCESS_SHARED macro is missing from libc-shim"
#endif

#ifndef PTHREAD_RWLOCK_INITIALIZER
#error "pthread.h:PTHREAD_RWLOCK_INITIALIZER macro is missing from libc-shim"
#endif

#ifndef PTHREAD_SCOPE_PROCESS
#error "pthread.h:PTHREAD_SCOPE_PROCESS macro is missing from libc-shim"
#endif

#ifndef PTHREAD_SCOPE_SYSTEM
#error "pthread.h:PTHREAD_SCOPE_SYSTEM macro is missing from libc-shim"
#endif

#ifndef pthread_cleanup_pop
#error "pthread.h:pthread_cleanup_pop macro is missing from libc-shim"
#endif

#ifndef pthread_cleanup_push
#error "pthread.h:pthread_cleanup_push macro is missing from libc-shim"
#endif

#ifndef pthread_equal
#error "pthread.h:pthread_equal macro is missing from libc-shim"
#endif

int main(void) { return 0; }
