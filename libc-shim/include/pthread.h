#ifndef _SLATE_PTHREAD_H
#define _SLATE_PTHREAD_H

typedef unsigned long pthread_t;

/* Opaque storage sized/aligned to match the real glibc pthread_attr_t
   since the real pthread_create symbol reads it. */
typedef union {
  char __size[56];
  long __align;
} pthread_attr_t;

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine)(void *), void *arg);
int pthread_join(pthread_t thread, void **retval);

#endif
