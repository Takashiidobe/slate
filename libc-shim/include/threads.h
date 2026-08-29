#ifndef _SLATE_THREADS_H
#define _SLATE_THREADS_H

#include <features.h>
#include <time.h>

#define __NEED_cnd_t
#define __NEED_mtx_t

#include <bits/types.h>
#define thread_local _Thread_local

#define TSS_DTOR_ITERATIONS 4

typedef unsigned int tss_t;
typedef void         (*tss_dtor_t)(void *);

typedef unsigned long thrd_t;
typedef int           (*thrd_start_t)(void *);

enum {
  thrd_success  = 0,
  thrd_busy     = 1,
  thrd_error    = 2,
  thrd_nomem    = 3,
  thrd_timedout = 4
};

enum { mtx_plain = 0, mtx_recursive = 1, mtx_timed = 2 };

#if defined(__aarch64__) && !defined(__SLATE_LIBC_MUSL)
typedef union {
  char __size[48];
  long __align;
} mtx_t;
#elif defined(__LP64__)
typedef union {
  char __size[40];
  long __align;
} mtx_t;
#else
typedef union {
  char __size[24];
  long __align;
} mtx_t;
#endif

typedef union {
  char      __size[48];
  long long __align;
} cnd_t;

typedef int once_flag;

#define ONCE_FLAG_INIT 0

int    thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
int    thrd_equal(thrd_t lhs, thrd_t rhs);
thrd_t thrd_current(void);
int    thrd_sleep(const struct timespec *duration, struct timespec *remaining);
void   thrd_yield(void);
_Noreturn void thrd_exit(int res);
int            thrd_detach(thrd_t thr);
int            thrd_join(thrd_t thr, int *res);

void call_once(once_flag *flag, void (*func)(void));

int  mtx_init(mtx_t *mutex, int type);
int  mtx_lock(mtx_t *mutex);
int  mtx_timedlock(mtx_t *__restrict mutex, const struct timespec *__restrict ts);
int  mtx_trylock(mtx_t *mutex);
int  mtx_unlock(mtx_t *mutex);
void mtx_destroy(mtx_t *mutex);

int  cnd_init(cnd_t *cond);
int  cnd_signal(cnd_t *cond);
int  cnd_broadcast(cnd_t *cond);
int  cnd_wait(cnd_t *cond, mtx_t *mutex);
int  cnd_timedwait(cnd_t *__restrict cond, mtx_t *__restrict mutex,
                   const struct timespec *__restrict ts);
void cnd_destroy(cnd_t *cond);

int   tss_create(tss_t *key, tss_dtor_t dtor);
void *tss_get(tss_t key);
int   tss_set(tss_t key, void *val);
void  tss_delete(tss_t key);

#if _REDIR_TIME64
__REDIR(thrd_sleep, __thrd_sleep_time64);
__REDIR(mtx_timedlock, __mtx_timedlock_time64);
__REDIR(cnd_timedwait, __cnd_timedwait_time64);
#endif

#endif
