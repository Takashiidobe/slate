#ifndef _SLATE_BITS_DARWIN_SEMAPHORE_H
#define _SLATE_BITS_DARWIN_SEMAPHORE_H

typedef int sem_t;

#define SEM_VALUE_MAX 32767
#define SEM_FAILED    ((sem_t *)-1)

int    sem_close(sem_t *);
int    sem_destroy(sem_t *);
int    sem_getvalue(sem_t *__restrict, int *__restrict);
int    sem_init(sem_t *, int, unsigned int);
sem_t *sem_open(const char *, int, ...);
int    sem_post(sem_t *);
int    sem_trywait(sem_t *);
int    sem_unlink(const char *);
int    sem_wait(sem_t *);

#endif
