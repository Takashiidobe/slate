#include <termios.h>

extern unsigned int slate_oracle_cfgetospeed(const struct termios *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cfgetospeed), __typeof__(cfgetospeed)),
    "termios.h:cfgetospeed declaration differs from oracle");

static __typeof__(cfgetospeed) *const slate_reference_cfgetospeed = &cfgetospeed;

typedef unsigned char slate_oracle_typedef_cc_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_cc_t, cc_t), "typedef cc_t differs from oracle");

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

_Static_assert(__builtin_offsetof(struct termios, c_iflag) == 0, "struct termios.c_iflag offset differs from oracle");

typedef unsigned int slate_oracle_struct_termios_c_iflag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct termios *)0)->c_iflag), slate_oracle_struct_termios_c_iflag), "struct termios.c_iflag field type differs from oracle");

_Static_assert(__builtin_offsetof(struct termios, c_oflag) == 4, "struct termios.c_oflag offset differs from oracle");

typedef unsigned int slate_oracle_struct_termios_c_oflag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct termios *)0)->c_oflag), slate_oracle_struct_termios_c_oflag), "struct termios.c_oflag field type differs from oracle");

_Static_assert(__builtin_offsetof(struct termios, c_cflag) == 8, "struct termios.c_cflag offset differs from oracle");

typedef unsigned int slate_oracle_struct_termios_c_cflag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct termios *)0)->c_cflag), slate_oracle_struct_termios_c_cflag), "struct termios.c_cflag field type differs from oracle");

_Static_assert(__builtin_offsetof(struct termios, c_lflag) == 12, "struct termios.c_lflag offset differs from oracle");

typedef unsigned int slate_oracle_struct_termios_c_lflag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct termios *)0)->c_lflag), slate_oracle_struct_termios_c_lflag), "struct termios.c_lflag field type differs from oracle");

_Static_assert(__builtin_offsetof(struct termios, c_line) == 16, "struct termios.c_line offset differs from oracle");

typedef unsigned char slate_oracle_struct_termios_c_line;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct termios *)0)->c_line), slate_oracle_struct_termios_c_line), "struct termios.c_line field type differs from oracle");

_Static_assert(__builtin_offsetof(struct termios, c_cc) == 17, "struct termios.c_cc offset differs from oracle");

#ifndef ADDRB
#error "termios.h:ADDRB macro is missing from libc-shim"
#endif

#ifndef B0
#error "termios.h:B0 macro is missing from libc-shim"
#endif

#ifndef B1000000
#error "termios.h:B1000000 macro is missing from libc-shim"
#endif

#ifndef B10000000
#error "termios.h:B10000000 macro is missing from libc-shim"
#endif

#ifndef B110
#error "termios.h:B110 macro is missing from libc-shim"
#endif

#ifndef B115200
#error "termios.h:B115200 macro is missing from libc-shim"
#endif

#ifndef B1152000
#error "termios.h:B1152000 macro is missing from libc-shim"
#endif

#ifndef B1200
#error "termios.h:B1200 macro is missing from libc-shim"
#endif

#ifndef B134
#error "termios.h:B134 macro is missing from libc-shim"
#endif

#ifndef B14400
#error "termios.h:B14400 macro is missing from libc-shim"
#endif

#ifndef B150
#error "termios.h:B150 macro is missing from libc-shim"
#endif

#ifndef B1500000
#error "termios.h:B1500000 macro is missing from libc-shim"
#endif

#ifndef B153600
#error "termios.h:B153600 macro is missing from libc-shim"
#endif

#ifndef B1800
#error "termios.h:B1800 macro is missing from libc-shim"
#endif

#ifndef B19200
#error "termios.h:B19200 macro is missing from libc-shim"
#endif

#ifndef B200
#error "termios.h:B200 macro is missing from libc-shim"
#endif

#ifndef B2000000
#error "termios.h:B2000000 macro is missing from libc-shim"
#endif

#ifndef B230400
#error "termios.h:B230400 macro is missing from libc-shim"
#endif

#ifndef B2400
#error "termios.h:B2400 macro is missing from libc-shim"
#endif

#ifndef B2500000
#error "termios.h:B2500000 macro is missing from libc-shim"
#endif

#ifndef B28800
#error "termios.h:B28800 macro is missing from libc-shim"
#endif

#ifndef B300
#error "termios.h:B300 macro is missing from libc-shim"
#endif

#ifndef B3000000
#error "termios.h:B3000000 macro is missing from libc-shim"
#endif

#ifndef B307200
#error "termios.h:B307200 macro is missing from libc-shim"
#endif

#ifndef B33600
#error "termios.h:B33600 macro is missing from libc-shim"
#endif

#ifndef B3500000
#error "termios.h:B3500000 macro is missing from libc-shim"
#endif

#ifndef B38400
#error "termios.h:B38400 macro is missing from libc-shim"
#endif

#ifndef B4000000
#error "termios.h:B4000000 macro is missing from libc-shim"
#endif

#ifndef B460800
#error "termios.h:B460800 macro is missing from libc-shim"
#endif

#ifndef B4800
#error "termios.h:B4800 macro is missing from libc-shim"
#endif

#ifndef B50
#error "termios.h:B50 macro is missing from libc-shim"
#endif

#ifndef B500000
#error "termios.h:B500000 macro is missing from libc-shim"
#endif

#ifndef B5000000
#error "termios.h:B5000000 macro is missing from libc-shim"
#endif

#ifndef B57600
#error "termios.h:B57600 macro is missing from libc-shim"
#endif

#ifndef B576000
#error "termios.h:B576000 macro is missing from libc-shim"
#endif

#ifndef B600
#error "termios.h:B600 macro is missing from libc-shim"
#endif

#ifndef B614400
#error "termios.h:B614400 macro is missing from libc-shim"
#endif

#ifndef B7200
#error "termios.h:B7200 macro is missing from libc-shim"
#endif

#ifndef B75
#error "termios.h:B75 macro is missing from libc-shim"
#endif

#ifndef B76800
#error "termios.h:B76800 macro is missing from libc-shim"
#endif

#ifndef B921600
#error "termios.h:B921600 macro is missing from libc-shim"
#endif

#ifndef B9600
#error "termios.h:B9600 macro is missing from libc-shim"
#endif

#ifndef BAUD_MAX
#error "termios.h:BAUD_MAX macro is missing from libc-shim"
#endif

#ifndef BOTHER
#error "termios.h:BOTHER macro is missing from libc-shim"
#endif

#ifndef BRKINT
#error "termios.h:BRKINT macro is missing from libc-shim"
#endif

#ifndef BS0
#error "termios.h:BS0 macro is missing from libc-shim"
#endif

#ifndef BS1
#error "termios.h:BS1 macro is missing from libc-shim"
#endif

#ifndef BSDLY
#error "termios.h:BSDLY macro is missing from libc-shim"
#endif

#ifndef CBAUD
#error "termios.h:CBAUD macro is missing from libc-shim"
#endif

#ifndef CBAUDEX
#error "termios.h:CBAUDEX macro is missing from libc-shim"
#endif

#ifndef CCEQ
#error "termios.h:CCEQ macro is missing from libc-shim"
#endif

#ifndef CIBAUD
#error "termios.h:CIBAUD macro is missing from libc-shim"
#endif

#ifndef CLOCAL
#error "termios.h:CLOCAL macro is missing from libc-shim"
#endif

#ifndef CMSPAR
#error "termios.h:CMSPAR macro is missing from libc-shim"
#endif

#ifndef CR0
#error "termios.h:CR0 macro is missing from libc-shim"
#endif

#ifndef CR1
#error "termios.h:CR1 macro is missing from libc-shim"
#endif

#ifndef CR2
#error "termios.h:CR2 macro is missing from libc-shim"
#endif

#ifndef CR3
#error "termios.h:CR3 macro is missing from libc-shim"
#endif

#ifndef CRDLY
#error "termios.h:CRDLY macro is missing from libc-shim"
#endif

#ifndef CREAD
#error "termios.h:CREAD macro is missing from libc-shim"
#endif

#ifndef CRTSCTS
#error "termios.h:CRTSCTS macro is missing from libc-shim"
#endif

#ifndef CS5
#error "termios.h:CS5 macro is missing from libc-shim"
#endif

#ifndef CS6
#error "termios.h:CS6 macro is missing from libc-shim"
#endif

#ifndef CS7
#error "termios.h:CS7 macro is missing from libc-shim"
#endif

#ifndef CS8
#error "termios.h:CS8 macro is missing from libc-shim"
#endif

#ifndef CSIZE
#error "termios.h:CSIZE macro is missing from libc-shim"
#endif

#ifndef CSTOPB
#error "termios.h:CSTOPB macro is missing from libc-shim"
#endif

#ifndef ECHO
#error "termios.h:ECHO macro is missing from libc-shim"
#endif

#ifndef ECHOCTL
#error "termios.h:ECHOCTL macro is missing from libc-shim"
#endif

#ifndef ECHOE
#error "termios.h:ECHOE macro is missing from libc-shim"
#endif

#ifndef ECHOK
#error "termios.h:ECHOK macro is missing from libc-shim"
#endif

#ifndef ECHOKE
#error "termios.h:ECHOKE macro is missing from libc-shim"
#endif

#ifndef ECHONL
#error "termios.h:ECHONL macro is missing from libc-shim"
#endif

#ifndef ECHOPRT
#error "termios.h:ECHOPRT macro is missing from libc-shim"
#endif

#ifndef EXTA
#error "termios.h:EXTA macro is missing from libc-shim"
#endif

#ifndef EXTB
#error "termios.h:EXTB macro is missing from libc-shim"
#endif

#ifndef EXTPROC
#error "termios.h:EXTPROC macro is missing from libc-shim"
#endif

#ifndef FF0
#error "termios.h:FF0 macro is missing from libc-shim"
#endif

#ifndef FF1
#error "termios.h:FF1 macro is missing from libc-shim"
#endif

#ifndef FFDLY
#error "termios.h:FFDLY macro is missing from libc-shim"
#endif

#ifndef FLUSHO
#error "termios.h:FLUSHO macro is missing from libc-shim"
#endif

#ifndef HUPCL
#error "termios.h:HUPCL macro is missing from libc-shim"
#endif

#ifndef IBSHIFT
#error "termios.h:IBSHIFT macro is missing from libc-shim"
#endif

#ifndef ICANON
#error "termios.h:ICANON macro is missing from libc-shim"
#endif

#ifndef ICRNL
#error "termios.h:ICRNL macro is missing from libc-shim"
#endif

#ifndef IEXTEN
#error "termios.h:IEXTEN macro is missing from libc-shim"
#endif

#ifndef IGNBRK
#error "termios.h:IGNBRK macro is missing from libc-shim"
#endif

#ifndef IGNCR
#error "termios.h:IGNCR macro is missing from libc-shim"
#endif

#ifndef IGNPAR
#error "termios.h:IGNPAR macro is missing from libc-shim"
#endif

#ifndef IMAXBEL
#error "termios.h:IMAXBEL macro is missing from libc-shim"
#endif

#ifndef INLCR
#error "termios.h:INLCR macro is missing from libc-shim"
#endif

#ifndef INPCK
#error "termios.h:INPCK macro is missing from libc-shim"
#endif

#ifndef ISIG
#error "termios.h:ISIG macro is missing from libc-shim"
#endif

#ifndef ISTRIP
#error "termios.h:ISTRIP macro is missing from libc-shim"
#endif

#ifndef IUCLC
#error "termios.h:IUCLC macro is missing from libc-shim"
#endif

#ifndef IUTF8
#error "termios.h:IUTF8 macro is missing from libc-shim"
#endif

#ifndef IXANY
#error "termios.h:IXANY macro is missing from libc-shim"
#endif

#ifndef IXOFF
#error "termios.h:IXOFF macro is missing from libc-shim"
#endif

#ifndef IXON
#error "termios.h:IXON macro is missing from libc-shim"
#endif

#ifndef NCCS
#error "termios.h:NCCS macro is missing from libc-shim"
#endif

#ifndef NL0
#error "termios.h:NL0 macro is missing from libc-shim"
#endif

#ifndef NL1
#error "termios.h:NL1 macro is missing from libc-shim"
#endif

#ifndef NLDLY
#error "termios.h:NLDLY macro is missing from libc-shim"
#endif

#ifndef NOFLSH
#error "termios.h:NOFLSH macro is missing from libc-shim"
#endif

#ifndef OCRNL
#error "termios.h:OCRNL macro is missing from libc-shim"
#endif

#ifndef OFDEL
#error "termios.h:OFDEL macro is missing from libc-shim"
#endif

#ifndef OFILL
#error "termios.h:OFILL macro is missing from libc-shim"
#endif

#ifndef OLCUC
#error "termios.h:OLCUC macro is missing from libc-shim"
#endif

#ifndef ONLCR
#error "termios.h:ONLCR macro is missing from libc-shim"
#endif

#ifndef ONLRET
#error "termios.h:ONLRET macro is missing from libc-shim"
#endif

#ifndef ONOCR
#error "termios.h:ONOCR macro is missing from libc-shim"
#endif

#ifndef OPOST
#error "termios.h:OPOST macro is missing from libc-shim"
#endif

#ifndef PARENB
#error "termios.h:PARENB macro is missing from libc-shim"
#endif

#ifndef PARMRK
#error "termios.h:PARMRK macro is missing from libc-shim"
#endif

#ifndef PARODD
#error "termios.h:PARODD macro is missing from libc-shim"
#endif

#ifndef PENDIN
#error "termios.h:PENDIN macro is missing from libc-shim"
#endif

#ifndef SPEED_MAX
#error "termios.h:SPEED_MAX macro is missing from libc-shim"
#endif

#ifndef TAB0
#error "termios.h:TAB0 macro is missing from libc-shim"
#endif

#ifndef TAB1
#error "termios.h:TAB1 macro is missing from libc-shim"
#endif

#ifndef TAB2
#error "termios.h:TAB2 macro is missing from libc-shim"
#endif

#ifndef TAB3
#error "termios.h:TAB3 macro is missing from libc-shim"
#endif

#ifndef TABDLY
#error "termios.h:TABDLY macro is missing from libc-shim"
#endif

#ifndef TCIFLUSH
#error "termios.h:TCIFLUSH macro is missing from libc-shim"
#endif

#ifndef TCIOFF
#error "termios.h:TCIOFF macro is missing from libc-shim"
#endif

#ifndef TCIOFLUSH
#error "termios.h:TCIOFLUSH macro is missing from libc-shim"
#endif

#ifndef TCION
#error "termios.h:TCION macro is missing from libc-shim"
#endif

#ifndef TCOFLUSH
#error "termios.h:TCOFLUSH macro is missing from libc-shim"
#endif

#ifndef TCOOFF
#error "termios.h:TCOOFF macro is missing from libc-shim"
#endif

#ifndef TCOON
#error "termios.h:TCOON macro is missing from libc-shim"
#endif

#ifndef TCSADRAIN
#error "termios.h:TCSADRAIN macro is missing from libc-shim"
#endif

#ifndef TCSAFLUSH
#error "termios.h:TCSAFLUSH macro is missing from libc-shim"
#endif

#ifndef TCSANOW
#error "termios.h:TCSANOW macro is missing from libc-shim"
#endif

#ifndef TIOCSER_TEMT
#error "termios.h:TIOCSER_TEMT macro is missing from libc-shim"
#endif

#ifndef TOSTOP
#error "termios.h:TOSTOP macro is missing from libc-shim"
#endif

#ifndef VDISCARD
#error "termios.h:VDISCARD macro is missing from libc-shim"
#endif

#ifndef VEOF
#error "termios.h:VEOF macro is missing from libc-shim"
#endif

#ifndef VEOL
#error "termios.h:VEOL macro is missing from libc-shim"
#endif

#ifndef VEOL2
#error "termios.h:VEOL2 macro is missing from libc-shim"
#endif

#ifndef VERASE
#error "termios.h:VERASE macro is missing from libc-shim"
#endif

#ifndef VINTR
#error "termios.h:VINTR macro is missing from libc-shim"
#endif

#ifndef VKILL
#error "termios.h:VKILL macro is missing from libc-shim"
#endif

#ifndef VLNEXT
#error "termios.h:VLNEXT macro is missing from libc-shim"
#endif

#ifndef VMIN
#error "termios.h:VMIN macro is missing from libc-shim"
#endif

#ifndef VQUIT
#error "termios.h:VQUIT macro is missing from libc-shim"
#endif

#ifndef VREPRINT
#error "termios.h:VREPRINT macro is missing from libc-shim"
#endif

#ifndef VSTART
#error "termios.h:VSTART macro is missing from libc-shim"
#endif

#ifndef VSTOP
#error "termios.h:VSTOP macro is missing from libc-shim"
#endif

#ifndef VSUSP
#error "termios.h:VSUSP macro is missing from libc-shim"
#endif

#ifndef VSWTC
#error "termios.h:VSWTC macro is missing from libc-shim"
#endif

#ifndef VT0
#error "termios.h:VT0 macro is missing from libc-shim"
#endif

#ifndef VT1
#error "termios.h:VT1 macro is missing from libc-shim"
#endif

#ifndef VTDLY
#error "termios.h:VTDLY macro is missing from libc-shim"
#endif

#ifndef VTIME
#error "termios.h:VTIME macro is missing from libc-shim"
#endif

#ifndef VWERASE
#error "termios.h:VWERASE macro is missing from libc-shim"
#endif

#ifndef XCASE
#error "termios.h:XCASE macro is missing from libc-shim"
#endif

#ifndef XTABS
#error "termios.h:XTABS macro is missing from libc-shim"
#endif

int main(void) { return 0; }
