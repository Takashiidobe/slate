#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stddef.h>
#include <time.h>

_Static_assert(sizeof(pthread_t) == 8, "pthread_t");
_Static_assert(sizeof(pthread_key_t) == 8, "pthread_key_t");
_Static_assert(sizeof(pthread_attr_t) == 64, "pthread_attr_t");
_Static_assert(sizeof(pthread_mutex_t) == 64, "pthread_mutex_t");
_Static_assert(sizeof(pthread_mutexattr_t) == 16, "pthread_mutexattr_t");
_Static_assert(sizeof(pthread_cond_t) == 48, "pthread_cond_t");
_Static_assert(sizeof(pthread_condattr_t) == 16, "pthread_condattr_t");
_Static_assert(sizeof(pthread_once_t) == 16, "pthread_once_t");
_Static_assert(sizeof(pthread_rwlock_t) == 200, "pthread_rwlock_t");
_Static_assert(sizeof(pthread_rwlockattr_t) == 24, "pthread_rwlockattr_t");
_Static_assert(_Alignof(pthread_attr_t) == 8, "pthread attr alignment");
_Static_assert(_Alignof(pthread_mutex_t) == 8, "pthread mutex alignment");
_Static_assert(_Alignof(pthread_cond_t) == 8, "pthread cond alignment");
_Static_assert(_Alignof(pthread_once_t) == 8, "pthread once alignment");
_Static_assert(_Alignof(pthread_rwlock_t) == 8, "pthread rwlock alignment");
_Static_assert(offsetof(pthread_mutex_t, __opaque) == 8,
               "pthread mutex opaque");
_Static_assert(offsetof(pthread_rwlock_t, __opaque) == 8,
               "pthread rwlock opaque");
_Static_assert(sizeof(sem_t) == 4, "sem_t");
_Static_assert(sizeof(struct sched_param) == 8, "sched_param");

_Static_assert(PTHREAD_CREATE_JOINABLE == 1, "joinable");
_Static_assert(PTHREAD_CREATE_DETACHED == 2, "detached");
_Static_assert(PTHREAD_CANCEL_ENABLE == 1, "cancel enable");
_Static_assert(PTHREAD_CANCEL_DISABLE == 0, "cancel disable");
_Static_assert(PTHREAD_CANCEL_DEFERRED == 2, "cancel deferred");
_Static_assert(PTHREAD_CANCEL_ASYNCHRONOUS == 0, "cancel async");
_Static_assert(PTHREAD_PROCESS_SHARED == 1, "process shared");
_Static_assert(PTHREAD_PROCESS_PRIVATE == 2, "process private");
_Static_assert(PTHREAD_MUTEX_ERRORCHECK == 1, "errorcheck mutex");
_Static_assert(PTHREAD_MUTEX_RECURSIVE == 2, "recursive mutex");
_Static_assert(_PTHREAD_MUTEX_SIG_init == 0x32AAABA7,
               "mutex initializer signature");
_Static_assert(_PTHREAD_COND_SIG_init == 0x3CB0B1BB,
               "condition initializer signature");
_Static_assert(_PTHREAD_ONCE_SIG_init == 0x30B1BCBA,
               "once initializer signature");
_Static_assert(_PTHREAD_RWLOCK_SIG_init == 0x2DA8B3B4,
               "rwlock initializer signature");
_Static_assert(SCHED_OTHER == 1, "SCHED_OTHER");
_Static_assert(SCHED_RR == 2, "SCHED_RR");
_Static_assert(SCHED_FIFO == 4, "SCHED_FIFO");
_Static_assert(SEM_VALUE_MAX == 32767, "SEM_VALUE_MAX");

_Static_assert(__builtin_types_compatible_p(pthread_t,
                                            struct _opaque_pthread_t *),
               "pthread_t type");
_Static_assert(__builtin_types_compatible_p(pthread_key_t, unsigned long),
               "pthread_key_t type");
_Static_assert(
    __builtin_types_compatible_p(__typeof__(&pthread_create),
                                 int (*)(pthread_t *__restrict,
                                         const pthread_attr_t *__restrict,
                                         void *(*)(void *), void *__restrict)),
    "pthread_create signature");
_Static_assert(
    __builtin_types_compatible_p(__typeof__(&pthread_cond_timedwait),
                                 int (*)(pthread_cond_t *__restrict,
                                         pthread_mutex_t *__restrict,
                                         const struct timespec *__restrict)),
    "pthread_cond_timedwait signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&sem_open),
                                            sem_t *(*)(const char *, int, ...)),
               "sem_open signature");

pthread_mutex_t  slate_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   slate_condition = PTHREAD_COND_INITIALIZER;
pthread_rwlock_t slate_rwlock    = PTHREAD_RWLOCK_INITIALIZER;
pthread_once_t   slate_once      = PTHREAD_ONCE_INIT;

void *slate_thread_entry(void *argument) { return argument; }

void slate_once_routine(void) {}

int slate_create_and_join(pthread_t *thread, void *argument) {
  void *result = 0;
  return pthread_create(thread, 0, slate_thread_entry, argument) ||
         pthread_join(*thread, &result);
}

int slate_wait_until(const struct timespec *deadline) {
  int result = pthread_mutex_lock(&slate_mutex);
  if (result == 0)
    result = pthread_cond_timedwait(&slate_condition, &slate_mutex, deadline);
  return pthread_mutex_unlock(&slate_mutex) || result;
}

int slate_read_lock(void) {
  return pthread_rwlock_rdlock(&slate_rwlock) ||
         pthread_rwlock_unlock(&slate_rwlock);
}

int slate_tls_once(pthread_key_t *key, void *value) {
  return pthread_once(&slate_once, slate_once_routine) ||
         pthread_key_create(key, 0) || pthread_setspecific(*key, value);
}

int slate_post_named_semaphore(const char *name) {
  sem_t *semaphore = sem_open(name, 0);
  return semaphore == SEM_FAILED || sem_post(semaphore) || sem_close(semaphore);
}

int slate_yield(void) { return sched_yield(); }


