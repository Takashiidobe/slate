#include <sys/socket.h>

_Static_assert(__builtin_offsetof(struct msghdr, msg_name) == 0, "struct msghdr.msg_name offset differs from oracle");

typedef void * slate_oracle_struct_msghdr_msg_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->msg_name), slate_oracle_struct_msghdr_msg_name), "struct msghdr.msg_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct msghdr, msg_namelen) == 8, "struct msghdr.msg_namelen offset differs from oracle");

typedef unsigned int slate_oracle_struct_msghdr_msg_namelen;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->msg_namelen), slate_oracle_struct_msghdr_msg_namelen), "struct msghdr.msg_namelen field type differs from oracle");

typedef struct iovec * slate_oracle_struct_msghdr_msg_iov;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->msg_iov), slate_oracle_struct_msghdr_msg_iov), "struct msghdr.msg_iov field type differs from oracle");

typedef int slate_oracle_struct_msghdr_msg_iovlen;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->msg_iovlen), slate_oracle_struct_msghdr_msg_iovlen), "struct msghdr.msg_iovlen field type differs from oracle");

typedef int slate_oracle_struct_msghdr___pad1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->__pad1), slate_oracle_struct_msghdr___pad1), "struct msghdr.__pad1 field type differs from oracle");

typedef void * slate_oracle_struct_msghdr_msg_control;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->msg_control), slate_oracle_struct_msghdr_msg_control), "struct msghdr.msg_control field type differs from oracle");

typedef unsigned int slate_oracle_struct_msghdr_msg_controllen;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->msg_controllen), slate_oracle_struct_msghdr_msg_controllen), "struct msghdr.msg_controllen field type differs from oracle");

typedef int slate_oracle_struct_msghdr___pad2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->__pad2), slate_oracle_struct_msghdr___pad2), "struct msghdr.__pad2 field type differs from oracle");

typedef int slate_oracle_struct_msghdr_msg_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msghdr *)0)->msg_flags), slate_oracle_struct_msghdr_msg_flags), "struct msghdr.msg_flags field type differs from oracle");

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

#ifndef MSG_SYN
#error "sys/socket.h:MSG_SYN macro is missing from libc-shim"
#endif

#ifndef MSG_TRUNC
#error "sys/socket.h:MSG_TRUNC macro is missing from libc-shim"
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

#ifndef SCM_RIGHTS
#error "sys/socket.h:SCM_RIGHTS macro is missing from libc-shim"
#endif

#ifndef SCM_TIMESTAMP
#error "sys/socket.h:SCM_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef SCM_TIMESTAMPING
#error "sys/socket.h:SCM_TIMESTAMPING macro is missing from libc-shim"
#endif

#ifndef SCM_TIMESTAMPING_OPT_STATS
#error "sys/socket.h:SCM_TIMESTAMPING_OPT_STATS macro is missing from libc-shim"
#endif

#ifndef SCM_TIMESTAMPING_PKTINFO
#error "sys/socket.h:SCM_TIMESTAMPING_PKTINFO macro is missing from libc-shim"
#endif

#ifndef SCM_TIMESTAMPNS
#error "sys/socket.h:SCM_TIMESTAMPNS macro is missing from libc-shim"
#endif

#ifndef SCM_TXTIME
#error "sys/socket.h:SCM_TXTIME macro is missing from libc-shim"
#endif

#ifndef SCM_WIFI_STATUS
#error "sys/socket.h:SCM_WIFI_STATUS macro is missing from libc-shim"
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

#ifndef SOL_ICMPV6
#error "sys/socket.h:SOL_ICMPV6 macro is missing from libc-shim"
#endif

#ifndef SOL_IP
#error "sys/socket.h:SOL_IP macro is missing from libc-shim"
#endif

#ifndef SOL_IPV6
#error "sys/socket.h:SOL_IPV6 macro is missing from libc-shim"
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

#ifndef SOL_SOCKET
#error "sys/socket.h:SOL_SOCKET macro is missing from libc-shim"
#endif

#ifndef SOL_TIPC
#error "sys/socket.h:SOL_TIPC macro is missing from libc-shim"
#endif

#ifndef SOL_TLS
#error "sys/socket.h:SOL_TLS macro is missing from libc-shim"
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

#ifndef SO_ACCEPTCONN
#error "sys/socket.h:SO_ACCEPTCONN macro is missing from libc-shim"
#endif

#ifndef SO_ATTACH_BPF
#error "sys/socket.h:SO_ATTACH_BPF macro is missing from libc-shim"
#endif

#ifndef SO_ATTACH_FILTER
#error "sys/socket.h:SO_ATTACH_FILTER macro is missing from libc-shim"
#endif

#ifndef SO_ATTACH_REUSEPORT_CBPF
#error "sys/socket.h:SO_ATTACH_REUSEPORT_CBPF macro is missing from libc-shim"
#endif

#ifndef SO_ATTACH_REUSEPORT_EBPF
#error "sys/socket.h:SO_ATTACH_REUSEPORT_EBPF macro is missing from libc-shim"
#endif

#ifndef SO_BINDTODEVICE
#error "sys/socket.h:SO_BINDTODEVICE macro is missing from libc-shim"
#endif

#ifndef SO_BINDTOIFINDEX
#error "sys/socket.h:SO_BINDTOIFINDEX macro is missing from libc-shim"
#endif

#ifndef SO_BPF_EXTENSIONS
#error "sys/socket.h:SO_BPF_EXTENSIONS macro is missing from libc-shim"
#endif

#ifndef SO_BROADCAST
#error "sys/socket.h:SO_BROADCAST macro is missing from libc-shim"
#endif

#ifndef SO_BSDCOMPAT
#error "sys/socket.h:SO_BSDCOMPAT macro is missing from libc-shim"
#endif

#ifndef SO_BUSY_POLL
#error "sys/socket.h:SO_BUSY_POLL macro is missing from libc-shim"
#endif

#ifndef SO_BUSY_POLL_BUDGET
#error "sys/socket.h:SO_BUSY_POLL_BUDGET macro is missing from libc-shim"
#endif

#ifndef SO_CNX_ADVICE
#error "sys/socket.h:SO_CNX_ADVICE macro is missing from libc-shim"
#endif

#ifndef SO_COOKIE
#error "sys/socket.h:SO_COOKIE macro is missing from libc-shim"
#endif

#ifndef SO_DEBUG
#error "sys/socket.h:SO_DEBUG macro is missing from libc-shim"
#endif

#ifndef SO_DETACH_BPF
#error "sys/socket.h:SO_DETACH_BPF macro is missing from libc-shim"
#endif

#ifndef SO_DETACH_FILTER
#error "sys/socket.h:SO_DETACH_FILTER macro is missing from libc-shim"
#endif

#ifndef SO_DETACH_REUSEPORT_BPF
#error "sys/socket.h:SO_DETACH_REUSEPORT_BPF macro is missing from libc-shim"
#endif

#ifndef SO_DOMAIN
#error "sys/socket.h:SO_DOMAIN macro is missing from libc-shim"
#endif

#ifndef SO_DONTROUTE
#error "sys/socket.h:SO_DONTROUTE macro is missing from libc-shim"
#endif

#ifndef SO_ERROR
#error "sys/socket.h:SO_ERROR macro is missing from libc-shim"
#endif

#ifndef SO_GET_FILTER
#error "sys/socket.h:SO_GET_FILTER macro is missing from libc-shim"
#endif

#ifndef SO_INCOMING_CPU
#error "sys/socket.h:SO_INCOMING_CPU macro is missing from libc-shim"
#endif

#ifndef SO_INCOMING_NAPI_ID
#error "sys/socket.h:SO_INCOMING_NAPI_ID macro is missing from libc-shim"
#endif

#ifndef SO_KEEPALIVE
#error "sys/socket.h:SO_KEEPALIVE macro is missing from libc-shim"
#endif

#ifndef SO_LINGER
#error "sys/socket.h:SO_LINGER macro is missing from libc-shim"
#endif

#ifndef SO_LOCK_FILTER
#error "sys/socket.h:SO_LOCK_FILTER macro is missing from libc-shim"
#endif

#ifndef SO_MARK
#error "sys/socket.h:SO_MARK macro is missing from libc-shim"
#endif

#ifndef SO_MAX_PACING_RATE
#error "sys/socket.h:SO_MAX_PACING_RATE macro is missing from libc-shim"
#endif

#ifndef SO_MEMINFO
#error "sys/socket.h:SO_MEMINFO macro is missing from libc-shim"
#endif

#ifndef SO_NOFCS
#error "sys/socket.h:SO_NOFCS macro is missing from libc-shim"
#endif

#ifndef SO_NO_CHECK
#error "sys/socket.h:SO_NO_CHECK macro is missing from libc-shim"
#endif

#ifndef SO_OOBINLINE
#error "sys/socket.h:SO_OOBINLINE macro is missing from libc-shim"
#endif

#ifndef SO_PASSCRED
#error "sys/socket.h:SO_PASSCRED macro is missing from libc-shim"
#endif

#ifndef SO_PASSSEC
#error "sys/socket.h:SO_PASSSEC macro is missing from libc-shim"
#endif

#ifndef SO_PEEK_OFF
#error "sys/socket.h:SO_PEEK_OFF macro is missing from libc-shim"
#endif

#ifndef SO_PEERCRED
#error "sys/socket.h:SO_PEERCRED macro is missing from libc-shim"
#endif

#ifndef SO_PEERGROUPS
#error "sys/socket.h:SO_PEERGROUPS macro is missing from libc-shim"
#endif

#ifndef SO_PEERNAME
#error "sys/socket.h:SO_PEERNAME macro is missing from libc-shim"
#endif

#ifndef SO_PEERSEC
#error "sys/socket.h:SO_PEERSEC macro is missing from libc-shim"
#endif

#ifndef SO_PREFER_BUSY_POLL
#error "sys/socket.h:SO_PREFER_BUSY_POLL macro is missing from libc-shim"
#endif

#ifndef SO_PRIORITY
#error "sys/socket.h:SO_PRIORITY macro is missing from libc-shim"
#endif

#ifndef SO_PROTOCOL
#error "sys/socket.h:SO_PROTOCOL macro is missing from libc-shim"
#endif

#ifndef SO_RCVBUF
#error "sys/socket.h:SO_RCVBUF macro is missing from libc-shim"
#endif

#ifndef SO_RCVBUFFORCE
#error "sys/socket.h:SO_RCVBUFFORCE macro is missing from libc-shim"
#endif

#ifndef SO_RCVLOWAT
#error "sys/socket.h:SO_RCVLOWAT macro is missing from libc-shim"
#endif

#ifndef SO_RCVTIMEO
#error "sys/socket.h:SO_RCVTIMEO macro is missing from libc-shim"
#endif

#ifndef SO_REUSEADDR
#error "sys/socket.h:SO_REUSEADDR macro is missing from libc-shim"
#endif

#ifndef SO_REUSEPORT
#error "sys/socket.h:SO_REUSEPORT macro is missing from libc-shim"
#endif

#ifndef SO_RXQ_OVFL
#error "sys/socket.h:SO_RXQ_OVFL macro is missing from libc-shim"
#endif

#ifndef SO_SECURITY_AUTHENTICATION
#error "sys/socket.h:SO_SECURITY_AUTHENTICATION macro is missing from libc-shim"
#endif

#ifndef SO_SECURITY_ENCRYPTION_NETWORK
#error "sys/socket.h:SO_SECURITY_ENCRYPTION_NETWORK macro is missing from libc-shim"
#endif

#ifndef SO_SECURITY_ENCRYPTION_TRANSPORT
#error "sys/socket.h:SO_SECURITY_ENCRYPTION_TRANSPORT macro is missing from libc-shim"
#endif

#ifndef SO_SELECT_ERR_QUEUE
#error "sys/socket.h:SO_SELECT_ERR_QUEUE macro is missing from libc-shim"
#endif

#ifndef SO_SNDBUF
#error "sys/socket.h:SO_SNDBUF macro is missing from libc-shim"
#endif

#ifndef SO_SNDBUFFORCE
#error "sys/socket.h:SO_SNDBUFFORCE macro is missing from libc-shim"
#endif

#ifndef SO_SNDLOWAT
#error "sys/socket.h:SO_SNDLOWAT macro is missing from libc-shim"
#endif

#ifndef SO_SNDTIMEO
#error "sys/socket.h:SO_SNDTIMEO macro is missing from libc-shim"
#endif

#ifndef SO_TIMESTAMP
#error "sys/socket.h:SO_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef SO_TIMESTAMPING
#error "sys/socket.h:SO_TIMESTAMPING macro is missing from libc-shim"
#endif

#ifndef SO_TIMESTAMPNS
#error "sys/socket.h:SO_TIMESTAMPNS macro is missing from libc-shim"
#endif

#ifndef SO_TXTIME
#error "sys/socket.h:SO_TXTIME macro is missing from libc-shim"
#endif

#ifndef SO_TYPE
#error "sys/socket.h:SO_TYPE macro is missing from libc-shim"
#endif

#ifndef SO_WIFI_STATUS
#error "sys/socket.h:SO_WIFI_STATUS macro is missing from libc-shim"
#endif

#ifndef SO_ZEROCOPY
#error "sys/socket.h:SO_ZEROCOPY macro is missing from libc-shim"
#endif

int main(void) { return 0; }
