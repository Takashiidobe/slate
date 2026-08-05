#ifndef _SLATE_TERMIOS_H
#define _SLATE_TERMIOS_H

#include <bits/termios.h>

speed_t cfgetospeed(const struct termios *);
speed_t cfgetispeed(const struct termios *);
int cfsetospeed(struct termios *, speed_t);
int cfsetispeed(struct termios *, speed_t);

int tcgetattr(int, struct termios *);
int tcsetattr(int, int, const struct termios *);

int tcsendbreak(int, int);
int tcdrain(int);
int tcflush(int, int);
int tcflow(int, int);

#define __NEED_pid_t
#include <bits/types.h>

pid_t tcgetsid(int);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
void cfmakeraw(struct termios *);
int cfsetspeed(struct termios *, speed_t);
#endif

#endif
