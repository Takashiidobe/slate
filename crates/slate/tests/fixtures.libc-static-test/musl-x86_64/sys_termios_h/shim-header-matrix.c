#include <sys/termios.h>

_Static_assert(sizeof(struct termios) == 60, "struct termios size differs from oracle");

_Static_assert(_Alignof(struct termios) == 4, "struct termios alignment differs from oracle");

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

_Static_assert(__builtin_offsetof(struct termios, __c_ispeed) == 52, "struct termios.__c_ispeed offset differs from oracle");

typedef unsigned int slate_oracle_struct_termios___c_ispeed;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct termios *)0)->__c_ispeed), slate_oracle_struct_termios___c_ispeed), "struct termios.__c_ispeed field type differs from oracle");

_Static_assert(__builtin_offsetof(struct termios, __c_ospeed) == 56, "struct termios.__c_ospeed offset differs from oracle");

typedef unsigned int slate_oracle_struct_termios___c_ospeed;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct termios *)0)->__c_ospeed), slate_oracle_struct_termios___c_ospeed), "struct termios.__c_ospeed field type differs from oracle");

#ifndef B0
#error "sys/termios.h:B0 macro is missing from libc-shim"
#endif

#ifndef B1000000
#error "sys/termios.h:B1000000 macro is missing from libc-shim"
#endif

#ifndef B110
#error "sys/termios.h:B110 macro is missing from libc-shim"
#endif

#ifndef B115200
#error "sys/termios.h:B115200 macro is missing from libc-shim"
#endif

#ifndef B1152000
#error "sys/termios.h:B1152000 macro is missing from libc-shim"
#endif

#ifndef B1200
#error "sys/termios.h:B1200 macro is missing from libc-shim"
#endif

#ifndef B134
#error "sys/termios.h:B134 macro is missing from libc-shim"
#endif

#ifndef B150
#error "sys/termios.h:B150 macro is missing from libc-shim"
#endif

#ifndef B1500000
#error "sys/termios.h:B1500000 macro is missing from libc-shim"
#endif

#ifndef B1800
#error "sys/termios.h:B1800 macro is missing from libc-shim"
#endif

#ifndef B19200
#error "sys/termios.h:B19200 macro is missing from libc-shim"
#endif

#ifndef B200
#error "sys/termios.h:B200 macro is missing from libc-shim"
#endif

#ifndef B2000000
#error "sys/termios.h:B2000000 macro is missing from libc-shim"
#endif

#ifndef B230400
#error "sys/termios.h:B230400 macro is missing from libc-shim"
#endif

#ifndef B2400
#error "sys/termios.h:B2400 macro is missing from libc-shim"
#endif

#ifndef B2500000
#error "sys/termios.h:B2500000 macro is missing from libc-shim"
#endif

#ifndef B300
#error "sys/termios.h:B300 macro is missing from libc-shim"
#endif

#ifndef B3000000
#error "sys/termios.h:B3000000 macro is missing from libc-shim"
#endif

#ifndef B3500000
#error "sys/termios.h:B3500000 macro is missing from libc-shim"
#endif

#ifndef B38400
#error "sys/termios.h:B38400 macro is missing from libc-shim"
#endif

#ifndef B4000000
#error "sys/termios.h:B4000000 macro is missing from libc-shim"
#endif

#ifndef B460800
#error "sys/termios.h:B460800 macro is missing from libc-shim"
#endif

#ifndef B4800
#error "sys/termios.h:B4800 macro is missing from libc-shim"
#endif

#ifndef B50
#error "sys/termios.h:B50 macro is missing from libc-shim"
#endif

#ifndef B500000
#error "sys/termios.h:B500000 macro is missing from libc-shim"
#endif

#ifndef B57600
#error "sys/termios.h:B57600 macro is missing from libc-shim"
#endif

#ifndef B576000
#error "sys/termios.h:B576000 macro is missing from libc-shim"
#endif

#ifndef B600
#error "sys/termios.h:B600 macro is missing from libc-shim"
#endif

#ifndef B75
#error "sys/termios.h:B75 macro is missing from libc-shim"
#endif

#ifndef B921600
#error "sys/termios.h:B921600 macro is missing from libc-shim"
#endif

#ifndef B9600
#error "sys/termios.h:B9600 macro is missing from libc-shim"
#endif

#ifndef BRKINT
#error "sys/termios.h:BRKINT macro is missing from libc-shim"
#endif

#ifndef BS0
#error "sys/termios.h:BS0 macro is missing from libc-shim"
#endif

#ifndef BS1
#error "sys/termios.h:BS1 macro is missing from libc-shim"
#endif

#ifndef BSDLY
#error "sys/termios.h:BSDLY macro is missing from libc-shim"
#endif

#ifndef CBAUD
#error "sys/termios.h:CBAUD macro is missing from libc-shim"
#endif

#ifndef CBAUDEX
#error "sys/termios.h:CBAUDEX macro is missing from libc-shim"
#endif

#ifndef CIBAUD
#error "sys/termios.h:CIBAUD macro is missing from libc-shim"
#endif

#ifndef CLOCAL
#error "sys/termios.h:CLOCAL macro is missing from libc-shim"
#endif

#ifndef CMSPAR
#error "sys/termios.h:CMSPAR macro is missing from libc-shim"
#endif

#ifndef CR0
#error "sys/termios.h:CR0 macro is missing from libc-shim"
#endif

#ifndef CR1
#error "sys/termios.h:CR1 macro is missing from libc-shim"
#endif

#ifndef CR2
#error "sys/termios.h:CR2 macro is missing from libc-shim"
#endif

#ifndef CR3
#error "sys/termios.h:CR3 macro is missing from libc-shim"
#endif

#ifndef CRDLY
#error "sys/termios.h:CRDLY macro is missing from libc-shim"
#endif

#ifndef CREAD
#error "sys/termios.h:CREAD macro is missing from libc-shim"
#endif

#ifndef CRTSCTS
#error "sys/termios.h:CRTSCTS macro is missing from libc-shim"
#endif

#ifndef CS5
#error "sys/termios.h:CS5 macro is missing from libc-shim"
#endif

#ifndef CS6
#error "sys/termios.h:CS6 macro is missing from libc-shim"
#endif

#ifndef CS7
#error "sys/termios.h:CS7 macro is missing from libc-shim"
#endif

#ifndef CS8
#error "sys/termios.h:CS8 macro is missing from libc-shim"
#endif

#ifndef CSIZE
#error "sys/termios.h:CSIZE macro is missing from libc-shim"
#endif

#ifndef CSTOPB
#error "sys/termios.h:CSTOPB macro is missing from libc-shim"
#endif

#ifndef ECHO
#error "sys/termios.h:ECHO macro is missing from libc-shim"
#endif

#ifndef ECHOCTL
#error "sys/termios.h:ECHOCTL macro is missing from libc-shim"
#endif

#ifndef ECHOE
#error "sys/termios.h:ECHOE macro is missing from libc-shim"
#endif

#ifndef ECHOK
#error "sys/termios.h:ECHOK macro is missing from libc-shim"
#endif

#ifndef ECHOKE
#error "sys/termios.h:ECHOKE macro is missing from libc-shim"
#endif

#ifndef ECHONL
#error "sys/termios.h:ECHONL macro is missing from libc-shim"
#endif

#ifndef ECHOPRT
#error "sys/termios.h:ECHOPRT macro is missing from libc-shim"
#endif

#ifndef EXTA
#error "sys/termios.h:EXTA macro is missing from libc-shim"
#endif

#ifndef EXTB
#error "sys/termios.h:EXTB macro is missing from libc-shim"
#endif

#ifndef EXTPROC
#error "sys/termios.h:EXTPROC macro is missing from libc-shim"
#endif

#ifndef FF0
#error "sys/termios.h:FF0 macro is missing from libc-shim"
#endif

#ifndef FF1
#error "sys/termios.h:FF1 macro is missing from libc-shim"
#endif

#ifndef FFDLY
#error "sys/termios.h:FFDLY macro is missing from libc-shim"
#endif

#ifndef FLUSHO
#error "sys/termios.h:FLUSHO macro is missing from libc-shim"
#endif

#ifndef HUPCL
#error "sys/termios.h:HUPCL macro is missing from libc-shim"
#endif

#ifndef ICANON
#error "sys/termios.h:ICANON macro is missing from libc-shim"
#endif

#ifndef ICRNL
#error "sys/termios.h:ICRNL macro is missing from libc-shim"
#endif

#ifndef IEXTEN
#error "sys/termios.h:IEXTEN macro is missing from libc-shim"
#endif

#ifndef IGNBRK
#error "sys/termios.h:IGNBRK macro is missing from libc-shim"
#endif

#ifndef IGNCR
#error "sys/termios.h:IGNCR macro is missing from libc-shim"
#endif

#ifndef IGNPAR
#error "sys/termios.h:IGNPAR macro is missing from libc-shim"
#endif

#ifndef IMAXBEL
#error "sys/termios.h:IMAXBEL macro is missing from libc-shim"
#endif

#ifndef INLCR
#error "sys/termios.h:INLCR macro is missing from libc-shim"
#endif

#ifndef INPCK
#error "sys/termios.h:INPCK macro is missing from libc-shim"
#endif

#ifndef ISIG
#error "sys/termios.h:ISIG macro is missing from libc-shim"
#endif

#ifndef ISTRIP
#error "sys/termios.h:ISTRIP macro is missing from libc-shim"
#endif

#ifndef IUCLC
#error "sys/termios.h:IUCLC macro is missing from libc-shim"
#endif

#ifndef IUTF8
#error "sys/termios.h:IUTF8 macro is missing from libc-shim"
#endif

#ifndef IXANY
#error "sys/termios.h:IXANY macro is missing from libc-shim"
#endif

#ifndef IXOFF
#error "sys/termios.h:IXOFF macro is missing from libc-shim"
#endif

#ifndef IXON
#error "sys/termios.h:IXON macro is missing from libc-shim"
#endif

#ifndef NL0
#error "sys/termios.h:NL0 macro is missing from libc-shim"
#endif

#ifndef NL1
#error "sys/termios.h:NL1 macro is missing from libc-shim"
#endif

#ifndef NLDLY
#error "sys/termios.h:NLDLY macro is missing from libc-shim"
#endif

#ifndef NOFLSH
#error "sys/termios.h:NOFLSH macro is missing from libc-shim"
#endif

#ifndef OCRNL
#error "sys/termios.h:OCRNL macro is missing from libc-shim"
#endif

#ifndef OFDEL
#error "sys/termios.h:OFDEL macro is missing from libc-shim"
#endif

#ifndef OFILL
#error "sys/termios.h:OFILL macro is missing from libc-shim"
#endif

#ifndef OLCUC
#error "sys/termios.h:OLCUC macro is missing from libc-shim"
#endif

#ifndef ONLCR
#error "sys/termios.h:ONLCR macro is missing from libc-shim"
#endif

#ifndef ONLRET
#error "sys/termios.h:ONLRET macro is missing from libc-shim"
#endif

#ifndef ONOCR
#error "sys/termios.h:ONOCR macro is missing from libc-shim"
#endif

#ifndef OPOST
#error "sys/termios.h:OPOST macro is missing from libc-shim"
#endif

#ifndef PARENB
#error "sys/termios.h:PARENB macro is missing from libc-shim"
#endif

#ifndef PARMRK
#error "sys/termios.h:PARMRK macro is missing from libc-shim"
#endif

#ifndef PARODD
#error "sys/termios.h:PARODD macro is missing from libc-shim"
#endif

#ifndef PENDIN
#error "sys/termios.h:PENDIN macro is missing from libc-shim"
#endif

#ifndef TAB0
#error "sys/termios.h:TAB0 macro is missing from libc-shim"
#endif

#ifndef TAB1
#error "sys/termios.h:TAB1 macro is missing from libc-shim"
#endif

#ifndef TAB2
#error "sys/termios.h:TAB2 macro is missing from libc-shim"
#endif

#ifndef TAB3
#error "sys/termios.h:TAB3 macro is missing from libc-shim"
#endif

#ifndef TABDLY
#error "sys/termios.h:TABDLY macro is missing from libc-shim"
#endif

#ifndef TCIFLUSH
#error "sys/termios.h:TCIFLUSH macro is missing from libc-shim"
#endif

#ifndef TCIOFF
#error "sys/termios.h:TCIOFF macro is missing from libc-shim"
#endif

#ifndef TCIOFLUSH
#error "sys/termios.h:TCIOFLUSH macro is missing from libc-shim"
#endif

#ifndef TCION
#error "sys/termios.h:TCION macro is missing from libc-shim"
#endif

#ifndef TCOFLUSH
#error "sys/termios.h:TCOFLUSH macro is missing from libc-shim"
#endif

#ifndef TCOOFF
#error "sys/termios.h:TCOOFF macro is missing from libc-shim"
#endif

#ifndef TCOON
#error "sys/termios.h:TCOON macro is missing from libc-shim"
#endif

#ifndef TCSADRAIN
#error "sys/termios.h:TCSADRAIN macro is missing from libc-shim"
#endif

#ifndef TCSAFLUSH
#error "sys/termios.h:TCSAFLUSH macro is missing from libc-shim"
#endif

#ifndef TCSANOW
#error "sys/termios.h:TCSANOW macro is missing from libc-shim"
#endif

#ifndef TOSTOP
#error "sys/termios.h:TOSTOP macro is missing from libc-shim"
#endif

#ifndef VDISCARD
#error "sys/termios.h:VDISCARD macro is missing from libc-shim"
#endif

#ifndef VEOF
#error "sys/termios.h:VEOF macro is missing from libc-shim"
#endif

#ifndef VEOL
#error "sys/termios.h:VEOL macro is missing from libc-shim"
#endif

#ifndef VEOL2
#error "sys/termios.h:VEOL2 macro is missing from libc-shim"
#endif

#ifndef VERASE
#error "sys/termios.h:VERASE macro is missing from libc-shim"
#endif

#ifndef VINTR
#error "sys/termios.h:VINTR macro is missing from libc-shim"
#endif

#ifndef VKILL
#error "sys/termios.h:VKILL macro is missing from libc-shim"
#endif

#ifndef VLNEXT
#error "sys/termios.h:VLNEXT macro is missing from libc-shim"
#endif

#ifndef VMIN
#error "sys/termios.h:VMIN macro is missing from libc-shim"
#endif

#ifndef VQUIT
#error "sys/termios.h:VQUIT macro is missing from libc-shim"
#endif

#ifndef VREPRINT
#error "sys/termios.h:VREPRINT macro is missing from libc-shim"
#endif

#ifndef VSTART
#error "sys/termios.h:VSTART macro is missing from libc-shim"
#endif

#ifndef VSTOP
#error "sys/termios.h:VSTOP macro is missing from libc-shim"
#endif

#ifndef VSUSP
#error "sys/termios.h:VSUSP macro is missing from libc-shim"
#endif

#ifndef VSWTC
#error "sys/termios.h:VSWTC macro is missing from libc-shim"
#endif

#ifndef VT0
#error "sys/termios.h:VT0 macro is missing from libc-shim"
#endif

#ifndef VT1
#error "sys/termios.h:VT1 macro is missing from libc-shim"
#endif

#ifndef VTDLY
#error "sys/termios.h:VTDLY macro is missing from libc-shim"
#endif

#ifndef VTIME
#error "sys/termios.h:VTIME macro is missing from libc-shim"
#endif

#ifndef VWERASE
#error "sys/termios.h:VWERASE macro is missing from libc-shim"
#endif

#ifndef XCASE
#error "sys/termios.h:XCASE macro is missing from libc-shim"
#endif

#ifndef XTABS
#error "sys/termios.h:XTABS macro is missing from libc-shim"
#endif

int main(void) { return 0; }
