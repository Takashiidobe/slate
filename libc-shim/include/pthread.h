#ifndef _SLATE_PTHREAD_H
#define _SLATE_PTHREAD_H

typedef unsigned long pthread_t;

/* Opaque storage sized/aligned to match the real (glibc or musl)
   pthread_attr_t, since the real pthread_create symbol reads it. See
   threads.h's mtx_t comment for how these sizes were verified per-arch,
   and for what __SLATE_LIBC_MUSL is: 32-bit needs 36 bytes; 64-bit needs
   56 except glibc's aarch64, which needs 64. */
#if defined(__aarch64__) && !defined(__SLATE_LIBC_MUSL)
typedef union {
  char __size[64];
  long __align;
} pthread_attr_t;
#elif defined(__LP64__)
typedef union {
  char __size[56];
  long __align;
} pthread_attr_t;
#else
typedef union {
  char __size[36];
  long __align;
} pthread_attr_t;
#endif

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine)(void *), void *arg);
int pthread_join(pthread_t thread, void **retval);

#endif
