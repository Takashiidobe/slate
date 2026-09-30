#include <sys/socket.h>

typedef unsigned int slate_oracle_typedef_socklen_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_socklen_t, socklen_t), "typedef socklen_t differs from oracle");

_Static_assert(sizeof(struct linger) == 8, "struct linger size differs from oracle");

_Static_assert(_Alignof(struct linger) == 4, "struct linger alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct linger, l_onoff) == 0, "struct linger.l_onoff offset differs from oracle");

typedef int slate_oracle_struct_linger_l_onoff;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct linger *)0)->l_onoff), slate_oracle_struct_linger_l_onoff), "struct linger.l_onoff field type differs from oracle");

_Static_assert(__builtin_offsetof(struct linger, l_linger) == 4, "struct linger.l_linger offset differs from oracle");

typedef int slate_oracle_struct_linger_l_linger;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct linger *)0)->l_linger), slate_oracle_struct_linger_l_linger), "struct linger.l_linger field type differs from oracle");

_Static_assert(sizeof(struct sockaddr) == 16, "struct sockaddr size differs from oracle");

_Static_assert(_Alignof(struct sockaddr) == 2, "struct sockaddr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr, sa_family) == 0, "struct sockaddr.sa_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_sa_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr *)0)->sa_family), slate_oracle_struct_sockaddr_sa_family), "struct sockaddr.sa_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr, sa_data) == 2, "struct sockaddr.sa_data offset differs from oracle");

#ifndef AF_ALG
#error "sys/socket.h:AF_ALG macro is missing from libc-shim"
#endif

#ifndef AF_APPLETALK
#error "sys/socket.h:AF_APPLETALK macro is missing from libc-shim"
#endif

#ifndef AF_ASH
#error "sys/socket.h:AF_ASH macro is missing from libc-shim"
#endif

#ifndef AF_ATMPVC
#error "sys/socket.h:AF_ATMPVC macro is missing from libc-shim"
#endif

#ifndef AF_ATMSVC
#error "sys/socket.h:AF_ATMSVC macro is missing from libc-shim"
#endif

#ifndef AF_AX25
#error "sys/socket.h:AF_AX25 macro is missing from libc-shim"
#endif

#ifndef AF_BLUETOOTH
#error "sys/socket.h:AF_BLUETOOTH macro is missing from libc-shim"
#endif

#ifndef AF_BRIDGE
#error "sys/socket.h:AF_BRIDGE macro is missing from libc-shim"
#endif

#ifndef AF_CAIF
#error "sys/socket.h:AF_CAIF macro is missing from libc-shim"
#endif

#ifndef AF_CAN
#error "sys/socket.h:AF_CAN macro is missing from libc-shim"
#endif

#ifndef AF_DECnet
#error "sys/socket.h:AF_DECnet macro is missing from libc-shim"
#endif

#ifndef AF_ECONET
#error "sys/socket.h:AF_ECONET macro is missing from libc-shim"
#endif

#ifndef AF_FILE
#error "sys/socket.h:AF_FILE macro is missing from libc-shim"
#endif

#ifndef AF_IB
#error "sys/socket.h:AF_IB macro is missing from libc-shim"
#endif

#ifndef AF_IEEE802154
#error "sys/socket.h:AF_IEEE802154 macro is missing from libc-shim"
#endif

#ifndef AF_INET
#error "sys/socket.h:AF_INET macro is missing from libc-shim"
#endif

#ifndef AF_INET6
#error "sys/socket.h:AF_INET6 macro is missing from libc-shim"
#endif

#ifndef AF_IPX
#error "sys/socket.h:AF_IPX macro is missing from libc-shim"
#endif

#ifndef AF_IRDA
#error "sys/socket.h:AF_IRDA macro is missing from libc-shim"
#endif

#ifndef AF_ISDN
#error "sys/socket.h:AF_ISDN macro is missing from libc-shim"
#endif

#ifndef AF_IUCV
#error "sys/socket.h:AF_IUCV macro is missing from libc-shim"
#endif

#ifndef AF_KCM
#error "sys/socket.h:AF_KCM macro is missing from libc-shim"
#endif

#ifndef AF_KEY
#error "sys/socket.h:AF_KEY macro is missing from libc-shim"
#endif

#ifndef AF_LLC
#error "sys/socket.h:AF_LLC macro is missing from libc-shim"
#endif

#ifndef AF_LOCAL
#error "sys/socket.h:AF_LOCAL macro is missing from libc-shim"
#endif

#ifndef AF_MAX
#error "sys/socket.h:AF_MAX macro is missing from libc-shim"
#endif

#ifndef AF_MCTP
#error "sys/socket.h:AF_MCTP macro is missing from libc-shim"
#endif

#ifndef AF_MPLS
#error "sys/socket.h:AF_MPLS macro is missing from libc-shim"
#endif

#ifndef AF_NETBEUI
#error "sys/socket.h:AF_NETBEUI macro is missing from libc-shim"
#endif

#ifndef AF_NETLINK
#error "sys/socket.h:AF_NETLINK macro is missing from libc-shim"
#endif

#ifndef AF_NETROM
#error "sys/socket.h:AF_NETROM macro is missing from libc-shim"
#endif

#ifndef AF_NFC
#error "sys/socket.h:AF_NFC macro is missing from libc-shim"
#endif

#ifndef AF_PACKET
#error "sys/socket.h:AF_PACKET macro is missing from libc-shim"
#endif

#ifndef AF_PHONET
#error "sys/socket.h:AF_PHONET macro is missing from libc-shim"
#endif

#ifndef AF_PPPOX
#error "sys/socket.h:AF_PPPOX macro is missing from libc-shim"
#endif

#ifndef AF_QIPCRTR
#error "sys/socket.h:AF_QIPCRTR macro is missing from libc-shim"
#endif

#ifndef AF_RDS
#error "sys/socket.h:AF_RDS macro is missing from libc-shim"
#endif

#ifndef AF_ROSE
#error "sys/socket.h:AF_ROSE macro is missing from libc-shim"
#endif

#ifndef AF_ROUTE
#error "sys/socket.h:AF_ROUTE macro is missing from libc-shim"
#endif

#ifndef AF_RXRPC
#error "sys/socket.h:AF_RXRPC macro is missing from libc-shim"
#endif

#ifndef AF_SECURITY
#error "sys/socket.h:AF_SECURITY macro is missing from libc-shim"
#endif

#ifndef AF_SMC
#error "sys/socket.h:AF_SMC macro is missing from libc-shim"
#endif

#ifndef AF_SNA
#error "sys/socket.h:AF_SNA macro is missing from libc-shim"
#endif

#ifndef AF_TIPC
#error "sys/socket.h:AF_TIPC macro is missing from libc-shim"
#endif

#ifndef AF_UNIX
#error "sys/socket.h:AF_UNIX macro is missing from libc-shim"
#endif

#ifndef AF_UNSPEC
#error "sys/socket.h:AF_UNSPEC macro is missing from libc-shim"
#endif

#ifndef AF_VSOCK
#error "sys/socket.h:AF_VSOCK macro is missing from libc-shim"
#endif

#ifndef AF_WANPIPE
#error "sys/socket.h:AF_WANPIPE macro is missing from libc-shim"
#endif

#ifndef AF_X25
#error "sys/socket.h:AF_X25 macro is missing from libc-shim"
#endif

#ifndef AF_XDP
#error "sys/socket.h:AF_XDP macro is missing from libc-shim"
#endif

#ifndef CMSG_ALIGN
#error "sys/socket.h:CMSG_ALIGN macro is missing from libc-shim"
#endif

#ifndef CMSG_DATA
#error "sys/socket.h:CMSG_DATA macro is missing from libc-shim"
#endif

#ifndef CMSG_FIRSTHDR
#error "sys/socket.h:CMSG_FIRSTHDR macro is missing from libc-shim"
#endif

#ifndef CMSG_LEN
#error "sys/socket.h:CMSG_LEN macro is missing from libc-shim"
#endif

#ifndef CMSG_NXTHDR
#error "sys/socket.h:CMSG_NXTHDR macro is missing from libc-shim"
#endif

#ifndef CMSG_SPACE
#error "sys/socket.h:CMSG_SPACE macro is missing from libc-shim"
#endif

#ifndef MSG_BATCH
#error "sys/socket.h:MSG_BATCH macro is missing from libc-shim"
#endif

#ifndef MSG_CMSG_CLOEXEC
#error "sys/socket.h:MSG_CMSG_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef MSG_CONFIRM
#error "sys/socket.h:MSG_CONFIRM macro is missing from libc-shim"
#endif

#ifndef MSG_CTRUNC
#error "sys/socket.h:MSG_CTRUNC macro is missing from libc-shim"
#endif

#ifndef MSG_DONTROUTE
#error "sys/socket.h:MSG_DONTROUTE macro is missing from libc-shim"
#endif

#ifndef MSG_DONTWAIT
#error "sys/socket.h:MSG_DONTWAIT macro is missing from libc-shim"
#endif

#ifndef MSG_EOR
#error "sys/socket.h:MSG_EOR macro is missing from libc-shim"
#endif

#ifndef MSG_ERRQUEUE
#error "sys/socket.h:MSG_ERRQUEUE macro is missing from libc-shim"
#endif

#ifndef MSG_FASTOPEN
#error "sys/socket.h:MSG_FASTOPEN macro is missing from libc-shim"
#endif

#ifndef MSG_FIN
#error "sys/socket.h:MSG_FIN macro is missing from libc-shim"
#endif

#ifndef MSG_MORE
#error "sys/socket.h:MSG_MORE macro is missing from libc-shim"
#endif

#ifndef MSG_NOSIGNAL
#error "sys/socket.h:MSG_NOSIGNAL macro is missing from libc-shim"
#endif

#ifndef MSG_OOB
#error "sys/socket.h:MSG_OOB macro is missing from libc-shim"
#endif

#ifndef MSG_PEEK
#error "sys/socket.h:MSG_PEEK macro is missing from libc-shim"
#endif

#ifndef MSG_PROXY
#error "sys/socket.h:MSG_PROXY macro is missing from libc-shim"
#endif

#ifndef MSG_RST
#error "sys/socket.h:MSG_RST macro is missing from libc-shim"
#endif

#ifndef MSG_SOCK_DEVMEM
#error "sys/socket.h:MSG_SOCK_DEVMEM macro is missing from libc-shim"
#endif

#ifndef MSG_SYN
#error "sys/socket.h:MSG_SYN macro is missing from libc-shim"
#endif

#ifndef MSG_TRUNC
#error "sys/socket.h:MSG_TRUNC macro is missing from libc-shim"
#endif

#ifndef MSG_TRYHARD
#error "sys/socket.h:MSG_TRYHARD macro is missing from libc-shim"
#endif

#ifndef MSG_WAITALL
#error "sys/socket.h:MSG_WAITALL macro is missing from libc-shim"
#endif

#ifndef MSG_WAITFORONE
#error "sys/socket.h:MSG_WAITFORONE macro is missing from libc-shim"
#endif

#ifndef MSG_ZEROCOPY
#error "sys/socket.h:MSG_ZEROCOPY macro is missing from libc-shim"
#endif

#ifndef PF_ALG
#error "sys/socket.h:PF_ALG macro is missing from libc-shim"
#endif

#ifndef PF_APPLETALK
#error "sys/socket.h:PF_APPLETALK macro is missing from libc-shim"
#endif

#ifndef PF_ASH
#error "sys/socket.h:PF_ASH macro is missing from libc-shim"
#endif

#ifndef PF_ATMPVC
#error "sys/socket.h:PF_ATMPVC macro is missing from libc-shim"
#endif

#ifndef PF_ATMSVC
#error "sys/socket.h:PF_ATMSVC macro is missing from libc-shim"
#endif

#ifndef PF_AX25
#error "sys/socket.h:PF_AX25 macro is missing from libc-shim"
#endif

#ifndef PF_BLUETOOTH
#error "sys/socket.h:PF_BLUETOOTH macro is missing from libc-shim"
#endif

#ifndef PF_BRIDGE
#error "sys/socket.h:PF_BRIDGE macro is missing from libc-shim"
#endif

#ifndef PF_CAIF
#error "sys/socket.h:PF_CAIF macro is missing from libc-shim"
#endif

#ifndef PF_CAN
#error "sys/socket.h:PF_CAN macro is missing from libc-shim"
#endif

#ifndef PF_DECnet
#error "sys/socket.h:PF_DECnet macro is missing from libc-shim"
#endif

#ifndef PF_ECONET
#error "sys/socket.h:PF_ECONET macro is missing from libc-shim"
#endif

#ifndef PF_FILE
#error "sys/socket.h:PF_FILE macro is missing from libc-shim"
#endif

#ifndef PF_IB
#error "sys/socket.h:PF_IB macro is missing from libc-shim"
#endif

#ifndef PF_IEEE802154
#error "sys/socket.h:PF_IEEE802154 macro is missing from libc-shim"
#endif

#ifndef PF_INET
#error "sys/socket.h:PF_INET macro is missing from libc-shim"
#endif

#ifndef PF_INET6
#error "sys/socket.h:PF_INET6 macro is missing from libc-shim"
#endif

#ifndef PF_IPX
#error "sys/socket.h:PF_IPX macro is missing from libc-shim"
#endif

#ifndef PF_IRDA
#error "sys/socket.h:PF_IRDA macro is missing from libc-shim"
#endif

#ifndef PF_ISDN
#error "sys/socket.h:PF_ISDN macro is missing from libc-shim"
#endif

#ifndef PF_IUCV
#error "sys/socket.h:PF_IUCV macro is missing from libc-shim"
#endif

#ifndef PF_KCM
#error "sys/socket.h:PF_KCM macro is missing from libc-shim"
#endif

#ifndef PF_KEY
#error "sys/socket.h:PF_KEY macro is missing from libc-shim"
#endif

#ifndef PF_LLC
#error "sys/socket.h:PF_LLC macro is missing from libc-shim"
#endif

#ifndef PF_LOCAL
#error "sys/socket.h:PF_LOCAL macro is missing from libc-shim"
#endif

#ifndef PF_MAX
#error "sys/socket.h:PF_MAX macro is missing from libc-shim"
#endif

#ifndef PF_MCTP
#error "sys/socket.h:PF_MCTP macro is missing from libc-shim"
#endif

#ifndef PF_MPLS
#error "sys/socket.h:PF_MPLS macro is missing from libc-shim"
#endif

#ifndef PF_NETBEUI
#error "sys/socket.h:PF_NETBEUI macro is missing from libc-shim"
#endif

#ifndef PF_NETLINK
#error "sys/socket.h:PF_NETLINK macro is missing from libc-shim"
#endif

#ifndef PF_NETROM
#error "sys/socket.h:PF_NETROM macro is missing from libc-shim"
#endif

#ifndef PF_NFC
#error "sys/socket.h:PF_NFC macro is missing from libc-shim"
#endif

#ifndef PF_PACKET
#error "sys/socket.h:PF_PACKET macro is missing from libc-shim"
#endif

#ifndef PF_PHONET
#error "sys/socket.h:PF_PHONET macro is missing from libc-shim"
#endif

#ifndef PF_PPPOX
#error "sys/socket.h:PF_PPPOX macro is missing from libc-shim"
#endif

#ifndef PF_QIPCRTR
#error "sys/socket.h:PF_QIPCRTR macro is missing from libc-shim"
#endif

#ifndef PF_RDS
#error "sys/socket.h:PF_RDS macro is missing from libc-shim"
#endif

#ifndef PF_ROSE
#error "sys/socket.h:PF_ROSE macro is missing from libc-shim"
#endif

#ifndef PF_ROUTE
#error "sys/socket.h:PF_ROUTE macro is missing from libc-shim"
#endif

#ifndef PF_RXRPC
#error "sys/socket.h:PF_RXRPC macro is missing from libc-shim"
#endif

#ifndef PF_SECURITY
#error "sys/socket.h:PF_SECURITY macro is missing from libc-shim"
#endif

#ifndef PF_SMC
#error "sys/socket.h:PF_SMC macro is missing from libc-shim"
#endif

#ifndef PF_SNA
#error "sys/socket.h:PF_SNA macro is missing from libc-shim"
#endif

#ifndef PF_TIPC
#error "sys/socket.h:PF_TIPC macro is missing from libc-shim"
#endif

#ifndef PF_UNIX
#error "sys/socket.h:PF_UNIX macro is missing from libc-shim"
#endif

#ifndef PF_UNSPEC
#error "sys/socket.h:PF_UNSPEC macro is missing from libc-shim"
#endif

#ifndef PF_VSOCK
#error "sys/socket.h:PF_VSOCK macro is missing from libc-shim"
#endif

#ifndef PF_WANPIPE
#error "sys/socket.h:PF_WANPIPE macro is missing from libc-shim"
#endif

#ifndef PF_X25
#error "sys/socket.h:PF_X25 macro is missing from libc-shim"
#endif

#ifndef PF_XDP
#error "sys/socket.h:PF_XDP macro is missing from libc-shim"
#endif

#ifndef SCM_CREDENTIALS
#error "sys/socket.h:SCM_CREDENTIALS macro is missing from libc-shim"
#endif

#ifndef SCM_PIDFD
#error "sys/socket.h:SCM_PIDFD macro is missing from libc-shim"
#endif

#ifndef SCM_RIGHTS
#error "sys/socket.h:SCM_RIGHTS macro is missing from libc-shim"
#endif

#ifndef SCM_SECURITY
#error "sys/socket.h:SCM_SECURITY macro is missing from libc-shim"
#endif

#ifndef SHUT_RD
#error "sys/socket.h:SHUT_RD macro is missing from libc-shim"
#endif

#ifndef SHUT_RDWR
#error "sys/socket.h:SHUT_RDWR macro is missing from libc-shim"
#endif

#ifndef SHUT_WR
#error "sys/socket.h:SHUT_WR macro is missing from libc-shim"
#endif

#ifndef SOCK_CLOEXEC
#error "sys/socket.h:SOCK_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef SOCK_DCCP
#error "sys/socket.h:SOCK_DCCP macro is missing from libc-shim"
#endif

#ifndef SOCK_DGRAM
#error "sys/socket.h:SOCK_DGRAM macro is missing from libc-shim"
#endif

#ifndef SOCK_NONBLOCK
#error "sys/socket.h:SOCK_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef SOCK_PACKET
#error "sys/socket.h:SOCK_PACKET macro is missing from libc-shim"
#endif

#ifndef SOCK_RAW
#error "sys/socket.h:SOCK_RAW macro is missing from libc-shim"
#endif

#ifndef SOCK_RDM
#error "sys/socket.h:SOCK_RDM macro is missing from libc-shim"
#endif

#ifndef SOCK_SEQPACKET
#error "sys/socket.h:SOCK_SEQPACKET macro is missing from libc-shim"
#endif

#ifndef SOCK_STREAM
#error "sys/socket.h:SOCK_STREAM macro is missing from libc-shim"
#endif

#ifndef SOL_AAL
#error "sys/socket.h:SOL_AAL macro is missing from libc-shim"
#endif

#ifndef SOL_ALG
#error "sys/socket.h:SOL_ALG macro is missing from libc-shim"
#endif

#ifndef SOL_ATM
#error "sys/socket.h:SOL_ATM macro is missing from libc-shim"
#endif

#ifndef SOL_BLUETOOTH
#error "sys/socket.h:SOL_BLUETOOTH macro is missing from libc-shim"
#endif

#ifndef SOL_CAIF
#error "sys/socket.h:SOL_CAIF macro is missing from libc-shim"
#endif

#ifndef SOL_DCCP
#error "sys/socket.h:SOL_DCCP macro is missing from libc-shim"
#endif

#ifndef SOL_DECNET
#error "sys/socket.h:SOL_DECNET macro is missing from libc-shim"
#endif

#ifndef SOL_IRDA
#error "sys/socket.h:SOL_IRDA macro is missing from libc-shim"
#endif

#ifndef SOL_IUCV
#error "sys/socket.h:SOL_IUCV macro is missing from libc-shim"
#endif

#ifndef SOL_KCM
#error "sys/socket.h:SOL_KCM macro is missing from libc-shim"
#endif

#ifndef SOL_LLC
#error "sys/socket.h:SOL_LLC macro is missing from libc-shim"
#endif

#ifndef SOL_MCTP
#error "sys/socket.h:SOL_MCTP macro is missing from libc-shim"
#endif

#ifndef SOL_MPTCP
#error "sys/socket.h:SOL_MPTCP macro is missing from libc-shim"
#endif

#ifndef SOL_NETBEUI
#error "sys/socket.h:SOL_NETBEUI macro is missing from libc-shim"
#endif

#ifndef SOL_NETLINK
#error "sys/socket.h:SOL_NETLINK macro is missing from libc-shim"
#endif

#ifndef SOL_NFC
#error "sys/socket.h:SOL_NFC macro is missing from libc-shim"
#endif

#ifndef SOL_PACKET
#error "sys/socket.h:SOL_PACKET macro is missing from libc-shim"
#endif

#ifndef SOL_PNPIPE
#error "sys/socket.h:SOL_PNPIPE macro is missing from libc-shim"
#endif

#ifndef SOL_PPPOL2TP
#error "sys/socket.h:SOL_PPPOL2TP macro is missing from libc-shim"
#endif

#ifndef SOL_RAW
#error "sys/socket.h:SOL_RAW macro is missing from libc-shim"
#endif

#ifndef SOL_RDS
#error "sys/socket.h:SOL_RDS macro is missing from libc-shim"
#endif

#ifndef SOL_RXRPC
#error "sys/socket.h:SOL_RXRPC macro is missing from libc-shim"
#endif

#ifndef SOL_SMC
#error "sys/socket.h:SOL_SMC macro is missing from libc-shim"
#endif

#ifndef SOL_TIPC
#error "sys/socket.h:SOL_TIPC macro is missing from libc-shim"
#endif

#ifndef SOL_TLS
#error "sys/socket.h:SOL_TLS macro is missing from libc-shim"
#endif

#ifndef SOL_VSOCK
#error "sys/socket.h:SOL_VSOCK macro is missing from libc-shim"
#endif

#ifndef SOL_X25
#error "sys/socket.h:SOL_X25 macro is missing from libc-shim"
#endif

#ifndef SOL_XDP
#error "sys/socket.h:SOL_XDP macro is missing from libc-shim"
#endif

#ifndef SOMAXCONN
#error "sys/socket.h:SOMAXCONN macro is missing from libc-shim"
#endif

int main(void) { return 0; }
