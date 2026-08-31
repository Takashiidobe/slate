#ifndef _SLATE_BITS_FREEBSD_PTHREAD_TYPES_H
#define _SLATE_BITS_FREEBSD_PTHREAD_TYPES_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/pthread-types.h> directly; include a public header instead."
#endif

struct _slate_freebsd_pthread;
struct _slate_freebsd_pthread_attr;
struct _slate_freebsd_pthread_mutex;
struct _slate_freebsd_pthread_mutex_attr;
struct _slate_freebsd_pthread_cond;
struct _slate_freebsd_pthread_cond_attr;
struct _slate_freebsd_pthread_rwlock;
struct _slate_freebsd_pthread_rwlockattr;
struct _slate_freebsd_pthread_barrier;
struct _slate_freebsd_pthread_barrier_attr;
struct _slate_freebsd_pthread_spinlock;

typedef struct _slate_freebsd_pthread             *__pthread_t;
typedef struct _slate_freebsd_pthread_attr         *__pthread_attr_t;
typedef struct _slate_freebsd_pthread_mutex        *__pthread_mutex_t;
typedef struct _slate_freebsd_pthread_mutex_attr   *__pthread_mutexattr_t;
typedef struct _slate_freebsd_pthread_cond         *__pthread_cond_t;
typedef struct _slate_freebsd_pthread_cond_attr    *__pthread_condattr_t;
typedef struct _slate_freebsd_pthread_rwlock       *__pthread_rwlock_t;
typedef struct _slate_freebsd_pthread_rwlockattr   *__pthread_rwlockattr_t;
typedef struct _slate_freebsd_pthread_barrier      *__pthread_barrier_t;
typedef struct _slate_freebsd_pthread_barrier_attr *__pthread_barrierattr_t;
typedef struct _slate_freebsd_pthread_spinlock     *__pthread_spinlock_t;

typedef int __pthread_key_t;

struct __pthread_once_s {
  int               state;
  __pthread_mutex_t mutex;
};
typedef struct __pthread_once_s __pthread_once_t;

#endif
