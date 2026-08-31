#ifndef _SLATE_BITS_DARWIN_PTHREAD_H
#define _SLATE_BITS_DARWIN_PTHREAD_H

#define __NEED_NULL
#define __NEED_size_t
#define __NEED_sigset_t
#define __NEED_pthread_attr_t
#define __NEED_pthread_t
#define __NEED_pthread_once_t
#define __NEED_pthread_mutex_t
#define __NEED_pthread_cond_t
#define __NEED_pthread_rwlock_t
#define __NEED_pthread_mutexattr_t
#define __NEED_pthread_condattr_t
#define __NEED_pthread_rwlockattr_t
#define __NEED_pthread_key_t
#include <bits/types.h>

#include <sched.h>
#include <time.h>

#define PTHREAD_CREATE_JOINABLE 1
#define PTHREAD_CREATE_DETACHED 2

#define PTHREAD_INHERIT_SCHED  1
#define PTHREAD_EXPLICIT_SCHED 2

#define PTHREAD_CANCEL_ENABLE       0x01
#define PTHREAD_CANCEL_DISABLE      0x00
#define PTHREAD_CANCEL_DEFERRED     0x02
#define PTHREAD_CANCEL_ASYNCHRONOUS 0x00
#define PTHREAD_CANCELED            ((void *)1)

#define PTHREAD_SCOPE_SYSTEM  1
#define PTHREAD_SCOPE_PROCESS 2

#define PTHREAD_PROCESS_SHARED  1
#define PTHREAD_PROCESS_PRIVATE 2

#define PTHREAD_PRIO_NONE    0
#define PTHREAD_PRIO_INHERIT 1
#define PTHREAD_PRIO_PROTECT 2

#define PTHREAD_MUTEX_NORMAL     0
#define PTHREAD_MUTEX_ERRORCHECK 1
#define PTHREAD_MUTEX_RECURSIVE  2
#define PTHREAD_MUTEX_DEFAULT    PTHREAD_MUTEX_NORMAL

#define _PTHREAD_MUTEX_SIG_init  0x32AAABA7
#define _PTHREAD_COND_SIG_init   0x3CB0B1BB
#define _PTHREAD_ONCE_SIG_init   0x30B1BCBA
#define _PTHREAD_RWLOCK_SIG_init 0x2DA8B3B4

#define PTHREAD_MUTEX_INITIALIZER  {_PTHREAD_MUTEX_SIG_init, {0}}
#define PTHREAD_COND_INITIALIZER   {_PTHREAD_COND_SIG_init, {0}}
#define PTHREAD_ONCE_INIT          {_PTHREAD_ONCE_SIG_init, {0}}
#define PTHREAD_RWLOCK_INITIALIZER {_PTHREAD_RWLOCK_SIG_init, {0}}

#define pthread_cleanup_push(routine, argument)                               \
  {                                                                           \
    struct __darwin_pthread_handler_rec __handler;                            \
    pthread_t __self = pthread_self();                                        \
    __handler.__routine = (routine);                                          \
    __handler.__arg = (argument);                                             \
    __handler.__next = __self->__cleanup_stack;                               \
    __self->__cleanup_stack = &__handler;

#define pthread_cleanup_pop(execute)                                          \
    __self->__cleanup_stack = __handler.__next;                               \
    if (execute)                                                              \
      (__handler.__routine)(__handler.__arg);                                 \
  }

int pthread_atfork(void (*)(void), void (*)(void), void (*)(void));
int pthread_attr_destroy(pthread_attr_t *);
int pthread_attr_getdetachstate(const pthread_attr_t *, int *);
int pthread_attr_getguardsize(const pthread_attr_t *__restrict,
                              size_t *__restrict);
int pthread_attr_getinheritsched(const pthread_attr_t *__restrict,
                                 int *__restrict);
int pthread_attr_getschedparam(const pthread_attr_t *__restrict,
                               struct sched_param *__restrict);
int pthread_attr_getschedpolicy(const pthread_attr_t *__restrict,
                                int *__restrict);
int pthread_attr_getscope(const pthread_attr_t *__restrict, int *__restrict);
int pthread_attr_getstack(const pthread_attr_t *__restrict, void **__restrict,
                          size_t *__restrict);
int pthread_attr_getstackaddr(const pthread_attr_t *__restrict,
                              void **__restrict);
int pthread_attr_getstacksize(const pthread_attr_t *__restrict,
                              size_t *__restrict);
int pthread_attr_init(pthread_attr_t *);
int pthread_attr_setdetachstate(pthread_attr_t *, int);
int pthread_attr_setguardsize(pthread_attr_t *, size_t);
int pthread_attr_setinheritsched(pthread_attr_t *, int);
int pthread_attr_setschedparam(pthread_attr_t *__restrict,
                               const struct sched_param *__restrict);
int pthread_attr_setschedpolicy(pthread_attr_t *, int);
int pthread_attr_setscope(pthread_attr_t *, int);
int pthread_attr_setstack(pthread_attr_t *, void *, size_t);
int pthread_attr_setstackaddr(pthread_attr_t *, void *);
int pthread_attr_setstacksize(pthread_attr_t *, size_t);

int pthread_cancel(pthread_t);
int pthread_create(pthread_t *__restrict, const pthread_attr_t *__restrict,
                   void *(*)(void *), void *__restrict);
int pthread_detach(pthread_t);
int pthread_equal(pthread_t, pthread_t);
_Noreturn void pthread_exit(void *);
int            pthread_getconcurrency(void);
int pthread_getschedparam(pthread_t, int *__restrict,
                          struct sched_param *__restrict);
void *pthread_getspecific(pthread_key_t);
int   pthread_join(pthread_t, void **);
int   pthread_key_create(pthread_key_t *, void (*)(void *));
int   pthread_key_delete(pthread_key_t);
int   pthread_once(pthread_once_t *, void (*)(void));
pthread_t pthread_self(void);
int pthread_setcancelstate(int, int *);
int pthread_setcanceltype(int, int *);
int pthread_setconcurrency(int);
int pthread_setschedparam(pthread_t, int, const struct sched_param *);
int pthread_setspecific(pthread_key_t, const void *);
void pthread_testcancel(void);

int pthread_cond_broadcast(pthread_cond_t *);
int pthread_cond_destroy(pthread_cond_t *);
int pthread_cond_init(pthread_cond_t *__restrict,
                      const pthread_condattr_t *__restrict);
int pthread_cond_signal(pthread_cond_t *);
int pthread_cond_timedwait(pthread_cond_t *__restrict,
                           pthread_mutex_t *__restrict,
                           const struct timespec *__restrict);
int pthread_cond_wait(pthread_cond_t *__restrict, pthread_mutex_t *__restrict);
int pthread_condattr_destroy(pthread_condattr_t *);
int pthread_condattr_init(pthread_condattr_t *);
int pthread_condattr_getpshared(const pthread_condattr_t *__restrict,
                                int *__restrict);
int pthread_condattr_setpshared(pthread_condattr_t *, int);

int pthread_mutex_destroy(pthread_mutex_t *);
int pthread_mutex_getprioceiling(const pthread_mutex_t *__restrict,
                                 int *__restrict);
int pthread_mutex_init(pthread_mutex_t *__restrict,
                       const pthread_mutexattr_t *__restrict);
int pthread_mutex_lock(pthread_mutex_t *);
int pthread_mutex_setprioceiling(pthread_mutex_t *__restrict, int,
                                 int *__restrict);
int pthread_mutex_trylock(pthread_mutex_t *);
int pthread_mutex_unlock(pthread_mutex_t *);
int pthread_mutexattr_destroy(pthread_mutexattr_t *);
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *__restrict,
                                     int *__restrict);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *__restrict,
                                  int *__restrict);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *__restrict,
                                 int *__restrict);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *__restrict,
                              int *__restrict);
int pthread_mutexattr_init(pthread_mutexattr_t *);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
int pthread_mutexattr_settype(pthread_mutexattr_t *, int);

int pthread_rwlock_destroy(pthread_rwlock_t *);
int pthread_rwlock_init(pthread_rwlock_t *__restrict,
                        const pthread_rwlockattr_t *__restrict);
int pthread_rwlock_rdlock(pthread_rwlock_t *);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int pthread_rwlock_trywrlock(pthread_rwlock_t *);
int pthread_rwlock_wrlock(pthread_rwlock_t *);
int pthread_rwlock_unlock(pthread_rwlock_t *);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *__restrict,
                                  int *__restrict);
int pthread_rwlockattr_init(pthread_rwlockattr_t *);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);

int pthread_is_threaded_np(void);
int pthread_getname_np(pthread_t, char *, size_t);
int pthread_setname_np(const char *);
int pthread_main_np(void);
size_t pthread_get_stacksize_np(pthread_t);
void  *pthread_get_stackaddr_np(pthread_t);
int pthread_cond_signal_thread_np(pthread_cond_t *, pthread_t);
int pthread_cond_timedwait_relative_np(pthread_cond_t *, pthread_mutex_t *,
                                       const struct timespec *);
int pthread_kill(pthread_t, int);
int pthread_sigmask(int, const sigset_t *, sigset_t *);
void pthread_yield_np(void);

#endif
