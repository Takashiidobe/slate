#include <sys/ioctl.h>

extern int slate_oracle_ioctl(int, int, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ioctl), __typeof__(ioctl)),
    "sys/ioctl.h:ioctl declaration differs from oracle");

static __typeof__(ioctl) *const slate_reference_ioctl = &ioctl;

#ifndef FIOASYNC
#error "sys/ioctl.h:FIOASYNC macro is missing from libc-shim"
#endif

#ifndef FIOCLEX
#error "sys/ioctl.h:FIOCLEX macro is missing from libc-shim"
#endif

#ifndef FIOGETOWN
#error "sys/ioctl.h:FIOGETOWN macro is missing from libc-shim"
#endif

#ifndef FIONBIO
#error "sys/ioctl.h:FIONBIO macro is missing from libc-shim"
#endif

#ifndef FIONCLEX
#error "sys/ioctl.h:FIONCLEX macro is missing from libc-shim"
#endif

#ifndef FIONREAD
#error "sys/ioctl.h:FIONREAD macro is missing from libc-shim"
#endif

#ifndef FIOQSIZE
#error "sys/ioctl.h:FIOQSIZE macro is missing from libc-shim"
#endif

#ifndef FIOSETOWN
#error "sys/ioctl.h:FIOSETOWN macro is missing from libc-shim"
#endif

#ifndef N_6PACK
#error "sys/ioctl.h:N_6PACK macro is missing from libc-shim"
#endif

#ifndef N_AX25
#error "sys/ioctl.h:N_AX25 macro is missing from libc-shim"
#endif

#ifndef N_CAIF
#error "sys/ioctl.h:N_CAIF macro is missing from libc-shim"
#endif

#ifndef N_GIGASET_M101
#error "sys/ioctl.h:N_GIGASET_M101 macro is missing from libc-shim"
#endif

#ifndef N_GSM0710
#error "sys/ioctl.h:N_GSM0710 macro is missing from libc-shim"
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

#ifndef N_NCI
#error "sys/ioctl.h:N_NCI macro is missing from libc-shim"
#endif

#ifndef N_NULL
#error "sys/ioctl.h:N_NULL macro is missing from libc-shim"
#endif

#ifndef N_PPP
#error "sys/ioctl.h:N_PPP macro is missing from libc-shim"
#endif

#ifndef N_PPS
#error "sys/ioctl.h:N_PPS macro is missing from libc-shim"
#endif

#ifndef N_PROFIBUS_FDL
#error "sys/ioctl.h:N_PROFIBUS_FDL macro is missing from libc-shim"
#endif

#ifndef N_R3964
#error "sys/ioctl.h:N_R3964 macro is missing from libc-shim"
#endif

#ifndef N_SLCAN
#error "sys/ioctl.h:N_SLCAN macro is missing from libc-shim"
#endif

#ifndef N_SLIP
#error "sys/ioctl.h:N_SLIP macro is missing from libc-shim"
#endif

#ifndef N_SMSBLOCK
#error "sys/ioctl.h:N_SMSBLOCK macro is missing from libc-shim"
#endif

#ifndef N_SPEAKUP
#error "sys/ioctl.h:N_SPEAKUP macro is missing from libc-shim"
#endif

#ifndef N_STRIP
#error "sys/ioctl.h:N_STRIP macro is missing from libc-shim"
#endif

#ifndef N_SYNC_PPP
#error "sys/ioctl.h:N_SYNC_PPP macro is missing from libc-shim"
#endif

#ifndef N_TI_WL
#error "sys/ioctl.h:N_TI_WL macro is missing from libc-shim"
#endif

#ifndef N_TRACEROUTER
#error "sys/ioctl.h:N_TRACEROUTER macro is missing from libc-shim"
#endif

#ifndef N_TRACESINK
#error "sys/ioctl.h:N_TRACESINK macro is missing from libc-shim"
#endif

#ifndef N_TTY
#error "sys/ioctl.h:N_TTY macro is missing from libc-shim"
#endif

#ifndef N_V253
#error "sys/ioctl.h:N_V253 macro is missing from libc-shim"
#endif

#ifndef N_X25
#error "sys/ioctl.h:N_X25 macro is missing from libc-shim"
#endif

#ifndef SIOCADDDLCI
#error "sys/ioctl.h:SIOCADDDLCI macro is missing from libc-shim"
#endif

#ifndef SIOCADDMULTI
#error "sys/ioctl.h:SIOCADDMULTI macro is missing from libc-shim"
#endif

#ifndef SIOCADDRT
#error "sys/ioctl.h:SIOCADDRT macro is missing from libc-shim"
#endif

#ifndef SIOCATMARK
#error "sys/ioctl.h:SIOCATMARK macro is missing from libc-shim"
#endif

#ifndef SIOCDARP
#error "sys/ioctl.h:SIOCDARP macro is missing from libc-shim"
#endif

#ifndef SIOCDELDLCI
#error "sys/ioctl.h:SIOCDELDLCI macro is missing from libc-shim"
#endif

#ifndef SIOCDELMULTI
#error "sys/ioctl.h:SIOCDELMULTI macro is missing from libc-shim"
#endif

#ifndef SIOCDELRT
#error "sys/ioctl.h:SIOCDELRT macro is missing from libc-shim"
#endif

#ifndef SIOCDEVPRIVATE
#error "sys/ioctl.h:SIOCDEVPRIVATE macro is missing from libc-shim"
#endif

#ifndef SIOCDIFADDR
#error "sys/ioctl.h:SIOCDIFADDR macro is missing from libc-shim"
#endif

#ifndef SIOCDRARP
#error "sys/ioctl.h:SIOCDRARP macro is missing from libc-shim"
#endif

#ifndef SIOCGARP
#error "sys/ioctl.h:SIOCGARP macro is missing from libc-shim"
#endif

#ifndef SIOCGIFADDR
#error "sys/ioctl.h:SIOCGIFADDR macro is missing from libc-shim"
#endif

#ifndef SIOCGIFBR
#error "sys/ioctl.h:SIOCGIFBR macro is missing from libc-shim"
#endif

#ifndef SIOCGIFBRDADDR
#error "sys/ioctl.h:SIOCGIFBRDADDR macro is missing from libc-shim"
#endif

#ifndef SIOCGIFCONF
#error "sys/ioctl.h:SIOCGIFCONF macro is missing from libc-shim"
#endif

#ifndef SIOCGIFCOUNT
#error "sys/ioctl.h:SIOCGIFCOUNT macro is missing from libc-shim"
#endif

#ifndef SIOCGIFDSTADDR
#error "sys/ioctl.h:SIOCGIFDSTADDR macro is missing from libc-shim"
#endif

#ifndef SIOCGIFENCAP
#error "sys/ioctl.h:SIOCGIFENCAP macro is missing from libc-shim"
#endif

#ifndef SIOCGIFFLAGS
#error "sys/ioctl.h:SIOCGIFFLAGS macro is missing from libc-shim"
#endif

#ifndef SIOCGIFHWADDR
#error "sys/ioctl.h:SIOCGIFHWADDR macro is missing from libc-shim"
#endif

#ifndef SIOCGIFINDEX
#error "sys/ioctl.h:SIOCGIFINDEX macro is missing from libc-shim"
#endif

#ifndef SIOCGIFMAP
#error "sys/ioctl.h:SIOCGIFMAP macro is missing from libc-shim"
#endif

#ifndef SIOCGIFMEM
#error "sys/ioctl.h:SIOCGIFMEM macro is missing from libc-shim"
#endif

#ifndef SIOCGIFMETRIC
#error "sys/ioctl.h:SIOCGIFMETRIC macro is missing from libc-shim"
#endif

#ifndef SIOCGIFMTU
#error "sys/ioctl.h:SIOCGIFMTU macro is missing from libc-shim"
#endif

#ifndef SIOCGIFNAME
#error "sys/ioctl.h:SIOCGIFNAME macro is missing from libc-shim"
#endif

#ifndef SIOCGIFNETMASK
#error "sys/ioctl.h:SIOCGIFNETMASK macro is missing from libc-shim"
#endif

#ifndef SIOCGIFPFLAGS
#error "sys/ioctl.h:SIOCGIFPFLAGS macro is missing from libc-shim"
#endif

#ifndef SIOCGIFSLAVE
#error "sys/ioctl.h:SIOCGIFSLAVE macro is missing from libc-shim"
#endif

#ifndef SIOCGIFTXQLEN
#error "sys/ioctl.h:SIOCGIFTXQLEN macro is missing from libc-shim"
#endif

#ifndef SIOCGPGRP
#error "sys/ioctl.h:SIOCGPGRP macro is missing from libc-shim"
#endif

#ifndef SIOCGRARP
#error "sys/ioctl.h:SIOCGRARP macro is missing from libc-shim"
#endif

#ifndef SIOCGSTAMP
#error "sys/ioctl.h:SIOCGSTAMP macro is missing from libc-shim"
#endif

#ifndef SIOCGSTAMPNS
#error "sys/ioctl.h:SIOCGSTAMPNS macro is missing from libc-shim"
#endif

#ifndef SIOCPROTOPRIVATE
#error "sys/ioctl.h:SIOCPROTOPRIVATE macro is missing from libc-shim"
#endif

#ifndef SIOCRTMSG
#error "sys/ioctl.h:SIOCRTMSG macro is missing from libc-shim"
#endif

#ifndef SIOCSARP
#error "sys/ioctl.h:SIOCSARP macro is missing from libc-shim"
#endif

#ifndef SIOCSIFADDR
#error "sys/ioctl.h:SIOCSIFADDR macro is missing from libc-shim"
#endif

#ifndef SIOCSIFBR
#error "sys/ioctl.h:SIOCSIFBR macro is missing from libc-shim"
#endif

#ifndef SIOCSIFBRDADDR
#error "sys/ioctl.h:SIOCSIFBRDADDR macro is missing from libc-shim"
#endif

#ifndef SIOCSIFDSTADDR
#error "sys/ioctl.h:SIOCSIFDSTADDR macro is missing from libc-shim"
#endif

#ifndef SIOCSIFENCAP
#error "sys/ioctl.h:SIOCSIFENCAP macro is missing from libc-shim"
#endif

#ifndef SIOCSIFFLAGS
#error "sys/ioctl.h:SIOCSIFFLAGS macro is missing from libc-shim"
#endif

#ifndef SIOCSIFHWADDR
#error "sys/ioctl.h:SIOCSIFHWADDR macro is missing from libc-shim"
#endif

#ifndef SIOCSIFHWBROADCAST
#error "sys/ioctl.h:SIOCSIFHWBROADCAST macro is missing from libc-shim"
#endif

#ifndef SIOCSIFLINK
#error "sys/ioctl.h:SIOCSIFLINK macro is missing from libc-shim"
#endif

#ifndef SIOCSIFMAP
#error "sys/ioctl.h:SIOCSIFMAP macro is missing from libc-shim"
#endif

#ifndef SIOCSIFMEM
#error "sys/ioctl.h:SIOCSIFMEM macro is missing from libc-shim"
#endif

#ifndef SIOCSIFMETRIC
#error "sys/ioctl.h:SIOCSIFMETRIC macro is missing from libc-shim"
#endif

#ifndef SIOCSIFMTU
#error "sys/ioctl.h:SIOCSIFMTU macro is missing from libc-shim"
#endif

#ifndef SIOCSIFNAME
#error "sys/ioctl.h:SIOCSIFNAME macro is missing from libc-shim"
#endif

#ifndef SIOCSIFNETMASK
#error "sys/ioctl.h:SIOCSIFNETMASK macro is missing from libc-shim"
#endif

#ifndef SIOCSIFPFLAGS
#error "sys/ioctl.h:SIOCSIFPFLAGS macro is missing from libc-shim"
#endif

#ifndef SIOCSIFSLAVE
#error "sys/ioctl.h:SIOCSIFSLAVE macro is missing from libc-shim"
#endif

#ifndef SIOCSIFTXQLEN
#error "sys/ioctl.h:SIOCSIFTXQLEN macro is missing from libc-shim"
#endif

#ifndef SIOCSPGRP
#error "sys/ioctl.h:SIOCSPGRP macro is missing from libc-shim"
#endif

#ifndef SIOCSRARP
#error "sys/ioctl.h:SIOCSRARP macro is missing from libc-shim"
#endif

#ifndef SIOGIFINDEX
#error "sys/ioctl.h:SIOGIFINDEX macro is missing from libc-shim"
#endif

#ifndef TCFLSH
#error "sys/ioctl.h:TCFLSH macro is missing from libc-shim"
#endif

#ifndef TCGETA
#error "sys/ioctl.h:TCGETA macro is missing from libc-shim"
#endif

#ifndef TCGETS
#error "sys/ioctl.h:TCGETS macro is missing from libc-shim"
#endif

#ifndef TCGETX
#error "sys/ioctl.h:TCGETX macro is missing from libc-shim"
#endif

#ifndef TCSBRK
#error "sys/ioctl.h:TCSBRK macro is missing from libc-shim"
#endif

#ifndef TCSBRKP
#error "sys/ioctl.h:TCSBRKP macro is missing from libc-shim"
#endif

#ifndef TCSETA
#error "sys/ioctl.h:TCSETA macro is missing from libc-shim"
#endif

#ifndef TCSETAF
#error "sys/ioctl.h:TCSETAF macro is missing from libc-shim"
#endif

#ifndef TCSETAW
#error "sys/ioctl.h:TCSETAW macro is missing from libc-shim"
#endif

#ifndef TCSETS
#error "sys/ioctl.h:TCSETS macro is missing from libc-shim"
#endif

#ifndef TCSETSF
#error "sys/ioctl.h:TCSETSF macro is missing from libc-shim"
#endif

#ifndef TCSETSW
#error "sys/ioctl.h:TCSETSW macro is missing from libc-shim"
#endif

#ifndef TCSETX
#error "sys/ioctl.h:TCSETX macro is missing from libc-shim"
#endif

#ifndef TCSETXF
#error "sys/ioctl.h:TCSETXF macro is missing from libc-shim"
#endif

#ifndef TCSETXW
#error "sys/ioctl.h:TCSETXW macro is missing from libc-shim"
#endif

#ifndef TCXONC
#error "sys/ioctl.h:TCXONC macro is missing from libc-shim"
#endif

#ifndef TIOCCBRK
#error "sys/ioctl.h:TIOCCBRK macro is missing from libc-shim"
#endif

#ifndef TIOCCONS
#error "sys/ioctl.h:TIOCCONS macro is missing from libc-shim"
#endif

#ifndef TIOCEXCL
#error "sys/ioctl.h:TIOCEXCL macro is missing from libc-shim"
#endif

#ifndef TIOCGDEV
#error "sys/ioctl.h:TIOCGDEV macro is missing from libc-shim"
#endif

#ifndef TIOCGETD
#error "sys/ioctl.h:TIOCGETD macro is missing from libc-shim"
#endif

#ifndef TIOCGEXCL
#error "sys/ioctl.h:TIOCGEXCL macro is missing from libc-shim"
#endif

#ifndef TIOCGICOUNT
#error "sys/ioctl.h:TIOCGICOUNT macro is missing from libc-shim"
#endif

#ifndef TIOCGISO7816
#error "sys/ioctl.h:TIOCGISO7816 macro is missing from libc-shim"
#endif

#ifndef TIOCGLCKTRMIOS
#error "sys/ioctl.h:TIOCGLCKTRMIOS macro is missing from libc-shim"
#endif

#ifndef TIOCGPGRP
#error "sys/ioctl.h:TIOCGPGRP macro is missing from libc-shim"
#endif

#ifndef TIOCGPKT
#error "sys/ioctl.h:TIOCGPKT macro is missing from libc-shim"
#endif

#ifndef TIOCGPTLCK
#error "sys/ioctl.h:TIOCGPTLCK macro is missing from libc-shim"
#endif

#ifndef TIOCGPTN
#error "sys/ioctl.h:TIOCGPTN macro is missing from libc-shim"
#endif

#ifndef TIOCGPTPEER
#error "sys/ioctl.h:TIOCGPTPEER macro is missing from libc-shim"
#endif

#ifndef TIOCGRS485
#error "sys/ioctl.h:TIOCGRS485 macro is missing from libc-shim"
#endif

#ifndef TIOCGSERIAL
#error "sys/ioctl.h:TIOCGSERIAL macro is missing from libc-shim"
#endif

#ifndef TIOCGSID
#error "sys/ioctl.h:TIOCGSID macro is missing from libc-shim"
#endif

#ifndef TIOCGSOFTCAR
#error "sys/ioctl.h:TIOCGSOFTCAR macro is missing from libc-shim"
#endif

#ifndef TIOCGWINSZ
#error "sys/ioctl.h:TIOCGWINSZ macro is missing from libc-shim"
#endif

#ifndef TIOCINQ
#error "sys/ioctl.h:TIOCINQ macro is missing from libc-shim"
#endif

#ifndef TIOCLINUX
#error "sys/ioctl.h:TIOCLINUX macro is missing from libc-shim"
#endif

#ifndef TIOCMBIC
#error "sys/ioctl.h:TIOCMBIC macro is missing from libc-shim"
#endif

#ifndef TIOCMBIS
#error "sys/ioctl.h:TIOCMBIS macro is missing from libc-shim"
#endif

#ifndef TIOCMGET
#error "sys/ioctl.h:TIOCMGET macro is missing from libc-shim"
#endif

#ifndef TIOCMIWAIT
#error "sys/ioctl.h:TIOCMIWAIT macro is missing from libc-shim"
#endif

#ifndef TIOCMSET
#error "sys/ioctl.h:TIOCMSET macro is missing from libc-shim"
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

#ifndef TIOCM_LOOP
#error "sys/ioctl.h:TIOCM_LOOP macro is missing from libc-shim"
#endif

#ifndef TIOCM_OUT1
#error "sys/ioctl.h:TIOCM_OUT1 macro is missing from libc-shim"
#endif

#ifndef TIOCM_OUT2
#error "sys/ioctl.h:TIOCM_OUT2 macro is missing from libc-shim"
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

#ifndef TIOCNOTTY
#error "sys/ioctl.h:TIOCNOTTY macro is missing from libc-shim"
#endif

#ifndef TIOCNXCL
#error "sys/ioctl.h:TIOCNXCL macro is missing from libc-shim"
#endif

#ifndef TIOCOUTQ
#error "sys/ioctl.h:TIOCOUTQ macro is missing from libc-shim"
#endif

#ifndef TIOCPKT
#error "sys/ioctl.h:TIOCPKT macro is missing from libc-shim"
#endif

#ifndef TIOCPKT_DATA
#error "sys/ioctl.h:TIOCPKT_DATA macro is missing from libc-shim"
#endif

#ifndef TIOCPKT_DOSTOP
#error "sys/ioctl.h:TIOCPKT_DOSTOP macro is missing from libc-shim"
#endif

#ifndef TIOCPKT_FLUSHREAD
#error "sys/ioctl.h:TIOCPKT_FLUSHREAD macro is missing from libc-shim"
#endif

#ifndef TIOCPKT_FLUSHWRITE
#error "sys/ioctl.h:TIOCPKT_FLUSHWRITE macro is missing from libc-shim"
#endif

#ifndef TIOCPKT_IOCTL
#error "sys/ioctl.h:TIOCPKT_IOCTL macro is missing from libc-shim"
#endif

#ifndef TIOCPKT_NOSTOP
#error "sys/ioctl.h:TIOCPKT_NOSTOP macro is missing from libc-shim"
#endif

#ifndef TIOCPKT_START
#error "sys/ioctl.h:TIOCPKT_START macro is missing from libc-shim"
#endif

#ifndef TIOCPKT_STOP
#error "sys/ioctl.h:TIOCPKT_STOP macro is missing from libc-shim"
#endif

#ifndef TIOCSBRK
#error "sys/ioctl.h:TIOCSBRK macro is missing from libc-shim"
#endif

#ifndef TIOCSCTTY
#error "sys/ioctl.h:TIOCSCTTY macro is missing from libc-shim"
#endif

#ifndef TIOCSERCONFIG
#error "sys/ioctl.h:TIOCSERCONFIG macro is missing from libc-shim"
#endif

#ifndef TIOCSERGETLSR
#error "sys/ioctl.h:TIOCSERGETLSR macro is missing from libc-shim"
#endif

#ifndef TIOCSERGETMULTI
#error "sys/ioctl.h:TIOCSERGETMULTI macro is missing from libc-shim"
#endif

#ifndef TIOCSERGSTRUCT
#error "sys/ioctl.h:TIOCSERGSTRUCT macro is missing from libc-shim"
#endif

#ifndef TIOCSERGWILD
#error "sys/ioctl.h:TIOCSERGWILD macro is missing from libc-shim"
#endif

#ifndef TIOCSERSETMULTI
#error "sys/ioctl.h:TIOCSERSETMULTI macro is missing from libc-shim"
#endif

#ifndef TIOCSERSWILD
#error "sys/ioctl.h:TIOCSERSWILD macro is missing from libc-shim"
#endif

#ifndef TIOCSER_TEMT
#error "sys/ioctl.h:TIOCSER_TEMT macro is missing from libc-shim"
#endif

#ifndef TIOCSETD
#error "sys/ioctl.h:TIOCSETD macro is missing from libc-shim"
#endif

#ifndef TIOCSIG
#error "sys/ioctl.h:TIOCSIG macro is missing from libc-shim"
#endif

#ifndef TIOCSISO7816
#error "sys/ioctl.h:TIOCSISO7816 macro is missing from libc-shim"
#endif

#ifndef TIOCSLCKTRMIOS
#error "sys/ioctl.h:TIOCSLCKTRMIOS macro is missing from libc-shim"
#endif

#ifndef TIOCSPGRP
#error "sys/ioctl.h:TIOCSPGRP macro is missing from libc-shim"
#endif

#ifndef TIOCSPTLCK
#error "sys/ioctl.h:TIOCSPTLCK macro is missing from libc-shim"
#endif

#ifndef TIOCSRS485
#error "sys/ioctl.h:TIOCSRS485 macro is missing from libc-shim"
#endif

#ifndef TIOCSSERIAL
#error "sys/ioctl.h:TIOCSSERIAL macro is missing from libc-shim"
#endif

#ifndef TIOCSSOFTCAR
#error "sys/ioctl.h:TIOCSSOFTCAR macro is missing from libc-shim"
#endif

#ifndef TIOCSTI
#error "sys/ioctl.h:TIOCSTI macro is missing from libc-shim"
#endif

#ifndef TIOCSWINSZ
#error "sys/ioctl.h:TIOCSWINSZ macro is missing from libc-shim"
#endif

#ifndef TIOCVHANGUP
#error "sys/ioctl.h:TIOCVHANGUP macro is missing from libc-shim"
#endif

int main(void) { return 0; }
