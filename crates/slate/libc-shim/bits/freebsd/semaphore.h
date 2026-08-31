#ifndef _SLATE_BITS_FREEBSD_SEMAPHORE_H
#define _SLATE_BITS_FREEBSD_SEMAPHORE_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/semaphore.h> directly; include a public header instead."
#endif

struct _slate_freebsd_usem2 {
  volatile unsigned int _count;
  unsigned int           _flags;
};

typedef struct _sem {
  unsigned int                 _magic;
  struct _slate_freebsd_usem2 _kern;
  unsigned int                 _padding;
} sem_t;

#define SEM_FAILED    ((sem_t *)0)
#define SEM_VALUE_MAX 0x7fffffff

int    sem_close(sem_t *);
int    sem_destroy(sem_t *);
int    sem_getvalue(sem_t *__restrict, int *__restrict);
int    sem_init(sem_t *, int, unsigned int);
sem_t *sem_open(const char *, int, ...);
int    sem_post(sem_t *);
int    sem_timedwait(sem_t *__restrict, const struct timespec *__restrict);
int    sem_trywait(sem_t *);
int    sem_unlink(const char *);
int    sem_wait(sem_t *);

#endif
