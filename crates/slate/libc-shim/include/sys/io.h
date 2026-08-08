#ifndef _SLATE_SYS_IO_H
#define _SLATE_SYS_IO_H

#include <features.h>

#include <bits/io.h>

int iopl(int);
int ioperm(unsigned long, unsigned long, int);

#endif
