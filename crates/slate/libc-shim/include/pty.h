#ifndef _SLATE_PTY_H
#define _SLATE_PTY_H

#include <sys/ioctl.h>
#include <termios.h>

struct winsize;

int openpty(int *, int *, char *, const struct termios *,
            const struct winsize *);
int forkpty(int *, char *, const struct termios *, const struct winsize *);

#endif
