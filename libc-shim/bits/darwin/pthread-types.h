#ifndef _SLATE_BITS_DARWIN_PTHREAD_TYPES_H
#define _SLATE_BITS_DARWIN_PTHREAD_TYPES_H

struct __darwin_pthread_handler_rec {
  void (*__routine)(void *);
  void                               *__arg;
  struct __darwin_pthread_handler_rec *__next;
};

struct _opaque_pthread_attr_t {
  long __sig;
  char __opaque[56];
};

struct _opaque_pthread_cond_t {
  long          __sig;
  unsigned char __opaque[40];
};

struct _opaque_pthread_condattr_t {
  long          __sig;
  unsigned char __opaque[8];
};

struct _opaque_pthread_mutex_t {
  long          __sig;
  unsigned char __opaque[56];
};

struct _opaque_pthread_mutexattr_t {
  long          __sig;
  unsigned char __opaque[8];
};

struct _opaque_pthread_once_t {
  long __sig;
  char __opaque[8];
};

struct _opaque_pthread_rwlock_t {
  long          __sig;
  unsigned char __opaque[192];
};

struct _opaque_pthread_rwlockattr_t {
  long          __sig;
  unsigned char __opaque[16];
};

struct _opaque_pthread_t {
  long                                __sig;
  struct __darwin_pthread_handler_rec *__cleanup_stack;
  char                                __opaque[8176];
};

typedef struct _opaque_pthread_attr_t       __pthread_attr_t;
typedef struct _opaque_pthread_cond_t       __pthread_cond_t;
typedef struct _opaque_pthread_condattr_t   __pthread_condattr_t;
typedef struct _opaque_pthread_mutex_t      __pthread_mutex_t;
typedef struct _opaque_pthread_mutexattr_t  __pthread_mutexattr_t;
typedef struct _opaque_pthread_once_t       __pthread_once_t;
typedef struct _opaque_pthread_rwlock_t     __pthread_rwlock_t;
typedef struct _opaque_pthread_rwlockattr_t __pthread_rwlockattr_t;
typedef struct _opaque_pthread_t           *__pthread_t;
typedef unsigned long                       __pthread_key_t;

#endif
