#ifndef _SLATE_TERMIOS_H
#define _SLATE_TERMIOS_H

#include <features.h>

#define NCCS 32

#include <bits/termios.h>

speed_t cfgetospeed(const struct termios *);
speed_t cfgetispeed(const struct termios *);
int     cfsetospeed(struct termios *, speed_t);
int     cfsetispeed(struct termios *, speed_t);

int tcgetattr(int, struct termios *);
int tcsetattr(int, int, const struct termios *);

#if defined(__SLATE_LIBC_MUSL)
#define __NEED_struct_winsize
#endif
#define __NEED_pid_t
#include <bits/types.h>

#if defined(__SLATE_LIBC_MUSL)
int tcgetwinsize(int, struct winsize *);
int tcsetwinsize(int, const struct winsize *);
#endif

int tcsendbreak(int, int);
int tcdrain(int);
int tcflush(int, int);
int tcflow(int, int);

pid_t tcgetsid(int);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
void cfmakeraw(struct termios *);
int  cfsetspeed(struct termios *, speed_t);
#endif

#endif
