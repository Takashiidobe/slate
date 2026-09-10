#ifndef _SGTTY_H
#define _SGTTY_H

#include <features.h>
#include <sys/ioctl.h>

struct sgttyb;

int gtty(int, struct sgttyb *) __THROW;
int stty(int, const struct sgttyb *) __THROW;

#endif
