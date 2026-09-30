#include <sys/ioctl.h>

extern int slate_oracle_ioctl(int, unsigned long, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ioctl), __typeof__(ioctl)),
    "sys/ioctl.h:ioctl declaration differs from oracle");

static __typeof__(ioctl) *const slate_reference_ioctl = &ioctl;

_Static_assert(sizeof(struct winsize) == 8, "struct winsize size differs from oracle");

_Static_assert(_Alignof(struct winsize) == 2, "struct winsize alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct winsize, ws_row) == 0, "struct winsize.ws_row offset differs from oracle");

typedef unsigned short slate_oracle_struct_winsize_ws_row;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct winsize *)0)->ws_row), slate_oracle_struct_winsize_ws_row), "struct winsize.ws_row field type differs from oracle");

_Static_assert(__builtin_offsetof(struct winsize, ws_col) == 2, "struct winsize.ws_col offset differs from oracle");

typedef unsigned short slate_oracle_struct_winsize_ws_col;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct winsize *)0)->ws_col), slate_oracle_struct_winsize_ws_col), "struct winsize.ws_col field type differs from oracle");

_Static_assert(__builtin_offsetof(struct winsize, ws_xpixel) == 4, "struct winsize.ws_xpixel offset differs from oracle");

typedef unsigned short slate_oracle_struct_winsize_ws_xpixel;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct winsize *)0)->ws_xpixel), slate_oracle_struct_winsize_ws_xpixel), "struct winsize.ws_xpixel field type differs from oracle");

_Static_assert(__builtin_offsetof(struct winsize, ws_ypixel) == 6, "struct winsize.ws_ypixel offset differs from oracle");

typedef unsigned short slate_oracle_struct_winsize_ws_ypixel;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct winsize *)0)->ws_ypixel), slate_oracle_struct_winsize_ws_ypixel), "struct winsize.ws_ypixel field type differs from oracle");

#ifndef N_6PACK
#error "sys/ioctl.h:N_6PACK macro is missing from libc-shim"
#endif

#ifndef N_AX25
#error "sys/ioctl.h:N_AX25 macro is missing from libc-shim"
#endif

#ifndef N_HCI
#error "sys/ioctl.h:N_HCI macro is missing from libc-shim"
#endif

#ifndef N_HDLC
#error "sys/ioctl.h:N_HDLC macro is missing from libc-shim"
#endif

#ifndef N_IRDA
#error "sys/ioctl.h:N_IRDA macro is missing from libc-shim"
#endif

#ifndef N_MASC
#error "sys/ioctl.h:N_MASC macro is missing from libc-shim"
#endif

#ifndef N_MOUSE
#error "sys/ioctl.h:N_MOUSE macro is missing from libc-shim"
#endif

#ifndef N_PPP
#error "sys/ioctl.h:N_PPP macro is missing from libc-shim"
#endif

#ifndef N_PROFIBUS_FDL
#error "sys/ioctl.h:N_PROFIBUS_FDL macro is missing from libc-shim"
#endif

#ifndef N_R3964
#error "sys/ioctl.h:N_R3964 macro is missing from libc-shim"
#endif

#ifndef N_SLIP
#error "sys/ioctl.h:N_SLIP macro is missing from libc-shim"
#endif

#ifndef N_SMSBLOCK
#error "sys/ioctl.h:N_SMSBLOCK macro is missing from libc-shim"
#endif

#ifndef N_STRIP
#error "sys/ioctl.h:N_STRIP macro is missing from libc-shim"
#endif

#ifndef N_SYNC_PPP
#error "sys/ioctl.h:N_SYNC_PPP macro is missing from libc-shim"
#endif

#ifndef N_TTY
#error "sys/ioctl.h:N_TTY macro is missing from libc-shim"
#endif

#ifndef N_X25
#error "sys/ioctl.h:N_X25 macro is missing from libc-shim"
#endif

#ifndef TIOCM_CAR
#error "sys/ioctl.h:TIOCM_CAR macro is missing from libc-shim"
#endif

#ifndef TIOCM_CD
#error "sys/ioctl.h:TIOCM_CD macro is missing from libc-shim"
#endif

#ifndef TIOCM_CTS
#error "sys/ioctl.h:TIOCM_CTS macro is missing from libc-shim"
#endif

#ifndef TIOCM_DSR
#error "sys/ioctl.h:TIOCM_DSR macro is missing from libc-shim"
#endif

#ifndef TIOCM_DTR
#error "sys/ioctl.h:TIOCM_DTR macro is missing from libc-shim"
#endif

#ifndef TIOCM_LE
#error "sys/ioctl.h:TIOCM_LE macro is missing from libc-shim"
#endif

#ifndef TIOCM_RI
#error "sys/ioctl.h:TIOCM_RI macro is missing from libc-shim"
#endif

#ifndef TIOCM_RNG
#error "sys/ioctl.h:TIOCM_RNG macro is missing from libc-shim"
#endif

#ifndef TIOCM_RTS
#error "sys/ioctl.h:TIOCM_RTS macro is missing from libc-shim"
#endif

#ifndef TIOCM_SR
#error "sys/ioctl.h:TIOCM_SR macro is missing from libc-shim"
#endif

#ifndef TIOCM_ST
#error "sys/ioctl.h:TIOCM_ST macro is missing from libc-shim"
#endif

int main(void) { return 0; }
