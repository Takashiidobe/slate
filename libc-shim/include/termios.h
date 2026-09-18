#ifndef _SLATE_TERMIOS_H
#define _SLATE_TERMIOS_H

#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/termios.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/termios.h>

#else

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

#if !defined(__SLATE_LIBC_DARWIN) && !defined(__SLATE_LIBC_FREEBSD)
#define IGNBRK 0000001
#define TCSANOW 0
#define OPOST 0000001
#define CSIZE 0000060
#define ISIG 0000001
#define VINTR 0
#define VQUIT 1
#define CS5 0000000
#define ICANON 0000002
#define BRKINT 0000002
#define OLCUC 0000002
#define TCSADRAIN 1
#define IGNPAR 0000004
#define CS6 0000020
#define VERASE 2
#define TCSAFLUSH 2
#define PARMRK 0000010
#define VKILL 3
#define CIBAUD 002003600000
#define ONLCR 0000004
#define CS7 0000040
#define XCASE 0000004
#define CS8 0000060
#define VEOF 4
#define OCRNL 0000010
#define INPCK 0000020
#define ISTRIP 0000040
#define VTIME 5
#define ECHO 0000010
#define ONOCR 0000020
#define CSTOPB 0000100
#define CREAD 0000200
#define ONLRET 0000040
#define ECHOE 0000020
#define INLCR 0000100
#define VMIN 6
#define PARENB 0000400
#define IGNCR 0000200
#define OFILL 0000100
#define VSWTC 7
#define OFDEL 0000200
#define VSTART 8
#define PARODD 0001000
#define ICRNL 0000400
#define ECHOK 0000040
#define VSTOP 9
#define HUPCL 0002000
#define IUCLC 0001000
#define ECHONL 0000100
#define NOFLSH 0000200
#define CLOCAL 0004000
#define VSUSP 10
#define NLDLY 0000400
#define IXON 0002000
#define TOSTOP 0000400
#define NL0 0000000
#define VEOL 11
#define VREPRINT 12
#define IXANY 0004000
#define NL1 0000400
#define IXOFF 0010000
#define CRDLY 0003000
#define VDISCARD 13
#define ECHOCTL 0001000
#define CMSPAR 010000000000
#define IMAXBEL 0020000
#define VWERASE 14
#define CR0 0000000
#define CR1 0001000
#define CRTSCTS 020000000000
#define VLNEXT 15
#define CR2 0002000
#define VEOL2 16
#define IUTF8 0040000
#define CR3 0003000
#define TABDLY 0014000
#define ECHOPRT 0002000
#define TAB0 0000000
#define TAB1 0004000
#define ECHOKE 0004000
#define TAB2 0010000
#define TAB3 0014000
#define BSDLY 0020000
#define BS0 0000000
#define FLUSHO 0010000
#define BS1 0020000
#define FFDLY 0100000
#define PENDIN 0040000
#define FF0 0000000
#define FF1 0100000
#define IEXTEN 0100000
#define VTDLY 0040000
#define VT0 0000000
#define EXTPROC 0200000
#define VT1 0040000
#define XTABS 0014000
#define TCOOFF 0
#define TCOON 1
#define TCIOFF 2
#define TCION 3
#define TCIFLUSH 0
#define TCOFLUSH 1
#define TCIOFLUSH 2
#define B0 0000000
#define B50 0000001
#define B75 0000002
#define B110 0000003
#define B134 0000004
#define B150 0000005
#define B200 0000006
#define B300 0000007
#define B600 0000010
#define B1200 0000011
#define B1800 0000012
#define B2400 0000013
#define B4800 0000014
#define B9600 0000015
#define B19200 0000016
#define B38400 0000017
#define EXTA B19200
#define EXTB B38400
#define CBAUDEX 0010000
#define BOTHER 0010000
#define B57600 0010001
#define B115200 0010002
#define B230400 0010003
#define B460800 0010004
#define B500000 0010005
#define B576000 0010006
#define B921600 0010007
#define B1000000 0010010
#define B1152000 0010011
#define B1500000 0010012
#define B2000000 0010013
#define B2500000 0010014
#define B3000000 0010015
#define B3500000 0010016
#define B4000000 0010017
#define CBAUD 0010017
#if defined(__SLATE_LIBC_GLIBC)
#define IBSHIFT 16
#define ADDRB 04000000000
#define CCEQ ((c) == (val) && (val) != _POSIX_VDISABLE)
#define B7200 7200U
#define B14400 14400U
#define B28800 28800U
#define B33600 33600U
#define B76800 76800U
#define B153600 153600U
#define B307200 307200U
#define B614400 614400U
#undef BOTHER
#define BOTHER __BOTHER
#define TIOCSER_TEMT 0x01
#define BAUD_MAX SPEED_MAX
#define B5000000 5000000U
#define B10000000 10000000U
#define SPEED_MAX 4294967295U
#endif

#endif

#endif
