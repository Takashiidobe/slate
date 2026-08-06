#ifndef _SLATE_SYS_RANDOM_H
#define _SLATE_SYS_RANDOM_H

#include <features.h>

#define __NEED_size_t
#define __NEED_ssize_t
#include <bits/types.h>

#define GRND_NONBLOCK 0x0001
#define GRND_RANDOM   0x0002
#define GRND_INSECURE 0x0004

ssize_t getrandom(void *, size_t, unsigned);

#endif
