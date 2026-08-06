#ifndef _SLATE_SYS_EVENTFD_H
#define _SLATE_SYS_EVENTFD_H

#include <fcntl.h>
#include <stdint.h>

typedef uint64_t eventfd_t;

#define EFD_SEMAPHORE 1
#define EFD_CLOEXEC   O_CLOEXEC
#define EFD_NONBLOCK  O_NONBLOCK

int eventfd(unsigned int, int);
int eventfd_read(int, eventfd_t *);
int eventfd_write(int, eventfd_t);

#endif /* sys/eventfd.h */
