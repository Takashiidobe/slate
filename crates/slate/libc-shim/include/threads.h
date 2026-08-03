#ifndef _SLATE_THREADS_H
#define _SLATE_THREADS_H

#include <time.h>

#define thread_local _Thread_local

#define TSS_DTOR_ITERATIONS 4

typedef unsigned int tss_t;
typedef void (*tss_dtor_t)(void *);

typedef unsigned long thrd_t;
typedef int (*thrd_start_t)(void *);

enum {
  thrd_success = 0,
  thrd_busy = 1,
  thrd_error = 2,
  thrd_nomem = 3,
  thrd_timedout = 4
};

enum { mtx_plain = 0, mtx_recursive = 1, mtx_timed = 2 };

/* Opaque storage sized/aligned to match the real glibc mtx_t and cnd_t
   (which are themselves pthread_mutex_t/pthread_cond_t-sized unions)
   since the real mtx_ and cnd_ symbols read and write them directly. */
typedef union {
  char __size[40];
  long __align;
} mtx_t;

typedef union {
  char __size[48];
  long long __align;
} cnd_t;

typedef int once_flag;

#define ONCE_FLAG_INIT 0

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
int thrd_equal(thrd_t lhs, thrd_t rhs);
thrd_t thrd_current(void);
int thrd_sleep(const struct timespec *duration, struct timespec *remaining);
void thrd_yield(void);
_Noreturn void thrd_exit(int res);
int thrd_detach(thrd_t thr);
int thrd_join(thrd_t thr, int *res);

void call_once(once_flag *flag, void (*func)(void));

int mtx_init(mtx_t *mutex, int type);
int mtx_lock(mtx_t *mutex);
int mtx_timedlock(mtx_t *restrict mutex, const struct timespec *restrict ts);
int mtx_trylock(mtx_t *mutex);
int mtx_unlock(mtx_t *mutex);
void mtx_destroy(mtx_t *mutex);

int cnd_init(cnd_t *cond);
int cnd_signal(cnd_t *cond);
int cnd_broadcast(cnd_t *cond);
int cnd_wait(cnd_t *cond, mtx_t *mutex);
int cnd_timedwait(cnd_t *restrict cond, mtx_t *restrict mutex,
                   const struct timespec *restrict ts);
void cnd_destroy(cnd_t *cond);

int tss_create(tss_t *key, tss_dtor_t dtor);
void *tss_get(tss_t key);
int tss_set(tss_t key, void *val);
void tss_delete(tss_t key);

#endif
