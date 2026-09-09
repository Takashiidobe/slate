#include <arpa/inet.h>

extern unsigned int slate_oracle_inet_addr(const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_inet_addr), __typeof__(inet_addr)),
    "arpa/inet.h:inet_addr declaration differs from oracle");

static __typeof__(inet_addr) *const slate_reference_inet_addr = &inet_addr;

typedef long slate_oracle_typedef_clock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clock_t, clock_t), "typedef clock_t differs from oracle");

typedef int slate_oracle_typedef_clockid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clockid_t, clockid_t), "typedef clockid_t differs from oracle");

typedef signed char slate_oracle_typedef_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int8_t, int8_t), "typedef int8_t differs from oracle");

typedef unsigned long slate_oracle_typedef_pthread_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_t, pthread_t), "typedef pthread_t differs from oracle");

typedef unsigned short slate_oracle_typedef_sa_family_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sa_family_t, sa_family_t), "typedef sa_family_t differs from oracle");

typedef struct __sigset_t slate_oracle_typedef_sigset_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sigset_t, sigset_t), "typedef sigset_t differs from oracle");

typedef unsigned int slate_oracle_typedef_socklen_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_socklen_t, socklen_t), "typedef socklen_t differs from oracle");

typedef long slate_oracle_typedef_time_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_time_t, time_t), "typedef time_t differs from oracle");

typedef void * slate_oracle_typedef_timer_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_timer_t, timer_t), "typedef timer_t differs from oracle");

typedef unsigned char slate_oracle_typedef_uint8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint8_t, uint8_t), "typedef uint8_t differs from oracle");

_Static_assert(sizeof(struct iovec) == 16, "struct iovec size differs from oracle");

_Static_assert(_Alignof(struct iovec) == 8, "struct iovec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct iovec, iov_base) == 0, "struct iovec.iov_base offset differs from oracle");

typedef void * slate_oracle_struct_iovec_iov_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct iovec *)0)->iov_base), slate_oracle_struct_iovec_iov_base), "struct iovec.iov_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct iovec, iov_len) == 8, "struct iovec.iov_len offset differs from oracle");

typedef unsigned long slate_oracle_struct_iovec_iov_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct iovec *)0)->iov_len), slate_oracle_struct_iovec_iov_len), "struct iovec.iov_len field type differs from oracle");

typedef struct in_addr slate_oracle_struct_ip_opts_ip_dst;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ip_opts *)0)->ip_dst), slate_oracle_struct_ip_opts_ip_dst), "struct ip_opts.ip_dst field type differs from oracle");

_Static_assert(sizeof(struct linger) == 8, "struct linger size differs from oracle");

_Static_assert(_Alignof(struct linger) == 4, "struct linger alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct linger, l_onoff) == 0, "struct linger.l_onoff offset differs from oracle");

typedef int slate_oracle_struct_linger_l_onoff;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct linger *)0)->l_onoff), slate_oracle_struct_linger_l_onoff), "struct linger.l_onoff field type differs from oracle");

_Static_assert(__builtin_offsetof(struct linger, l_linger) == 4, "struct linger.l_linger offset differs from oracle");

typedef int slate_oracle_struct_linger_l_linger;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct linger *)0)->l_linger), slate_oracle_struct_linger_l_linger), "struct linger.l_linger field type differs from oracle");

_Static_assert(sizeof(struct osockaddr) == 16, "struct osockaddr size differs from oracle");

_Static_assert(_Alignof(struct osockaddr) == 2, "struct osockaddr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct osockaddr, sa_family) == 0, "struct osockaddr.sa_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_osockaddr_sa_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct osockaddr *)0)->sa_family), slate_oracle_struct_osockaddr_sa_family), "struct osockaddr.sa_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct osockaddr, sa_data) == 2, "struct osockaddr.sa_data offset differs from oracle");

_Static_assert(sizeof(struct sockaddr) == 16, "struct sockaddr size differs from oracle");

_Static_assert(_Alignof(struct sockaddr) == 2, "struct sockaddr alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr, sa_family) == 0, "struct sockaddr.sa_family offset differs from oracle");

typedef unsigned short slate_oracle_struct_sockaddr_sa_family;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sockaddr *)0)->sa_family), slate_oracle_struct_sockaddr_sa_family), "struct sockaddr.sa_family field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sockaddr, sa_data) == 2, "struct sockaddr.sa_data offset differs from oracle");

_Static_assert(sizeof(struct timespec) == 16, "struct timespec size differs from oracle");

_Static_assert(_Alignof(struct timespec) == 8, "struct timespec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_sec) == 0, "struct timespec.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_sec), slate_oracle_struct_timespec_tv_sec), "struct timespec.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_nsec) == 8, "struct timespec.tv_nsec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_nsec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_nsec), slate_oracle_struct_timespec_tv_nsec), "struct timespec.tv_nsec field type differs from oracle");

_Static_assert(sizeof(struct timeval) == 16, "struct timeval size differs from oracle");

_Static_assert(_Alignof(struct timeval) == 8, "struct timeval alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_sec) == 0, "struct timeval.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_sec), slate_oracle_struct_timeval_tv_sec), "struct timeval.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_usec) == 8, "struct timeval.tv_usec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_usec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_usec), slate_oracle_struct_timeval_tv_usec), "struct timeval.tv_usec field type differs from oracle");

#ifndef AF_ALG
#error "arpa/inet.h:AF_ALG macro is missing from libc-shim"
#endif

#ifndef AF_APPLETALK
#error "arpa/inet.h:AF_APPLETALK macro is missing from libc-shim"
#endif

#ifndef AF_ASH
#error "arpa/inet.h:AF_ASH macro is missing from libc-shim"
#endif

#ifndef AF_ATMPVC
#error "arpa/inet.h:AF_ATMPVC macro is missing from libc-shim"
#endif

#ifndef AF_ATMSVC
#error "arpa/inet.h:AF_ATMSVC macro is missing from libc-shim"
#endif

#ifndef AF_AX25
#error "arpa/inet.h:AF_AX25 macro is missing from libc-shim"
#endif

#ifndef AF_BLUETOOTH
#error "arpa/inet.h:AF_BLUETOOTH macro is missing from libc-shim"
#endif

#ifndef AF_BRIDGE
#error "arpa/inet.h:AF_BRIDGE macro is missing from libc-shim"
#endif

#ifndef AF_CAIF
#error "arpa/inet.h:AF_CAIF macro is missing from libc-shim"
#endif

#ifndef AF_CAN
#error "arpa/inet.h:AF_CAN macro is missing from libc-shim"
#endif

#ifndef AF_DECnet
#error "arpa/inet.h:AF_DECnet macro is missing from libc-shim"
#endif

#ifndef AF_ECONET
#error "arpa/inet.h:AF_ECONET macro is missing from libc-shim"
#endif

#ifndef AF_FILE
#error "arpa/inet.h:AF_FILE macro is missing from libc-shim"
#endif

#ifndef AF_IB
#error "arpa/inet.h:AF_IB macro is missing from libc-shim"
#endif

#ifndef AF_IEEE802154
#error "arpa/inet.h:AF_IEEE802154 macro is missing from libc-shim"
#endif

#ifndef AF_INET
#error "arpa/inet.h:AF_INET macro is missing from libc-shim"
#endif

#ifndef AF_INET6
#error "arpa/inet.h:AF_INET6 macro is missing from libc-shim"
#endif

#ifndef AF_IPX
#error "arpa/inet.h:AF_IPX macro is missing from libc-shim"
#endif

#ifndef AF_IRDA
#error "arpa/inet.h:AF_IRDA macro is missing from libc-shim"
#endif

#ifndef AF_ISDN
#error "arpa/inet.h:AF_ISDN macro is missing from libc-shim"
#endif

#ifndef AF_IUCV
#error "arpa/inet.h:AF_IUCV macro is missing from libc-shim"
#endif

#ifndef AF_KCM
#error "arpa/inet.h:AF_KCM macro is missing from libc-shim"
#endif

#ifndef AF_KEY
#error "arpa/inet.h:AF_KEY macro is missing from libc-shim"
#endif

#ifndef AF_LLC
#error "arpa/inet.h:AF_LLC macro is missing from libc-shim"
#endif

#ifndef AF_LOCAL
#error "arpa/inet.h:AF_LOCAL macro is missing from libc-shim"
#endif

#ifndef AF_MAX
#error "arpa/inet.h:AF_MAX macro is missing from libc-shim"
#endif

#ifndef AF_MCTP
#error "arpa/inet.h:AF_MCTP macro is missing from libc-shim"
#endif

#ifndef AF_MPLS
#error "arpa/inet.h:AF_MPLS macro is missing from libc-shim"
#endif

#ifndef AF_NETBEUI
#error "arpa/inet.h:AF_NETBEUI macro is missing from libc-shim"
#endif

#ifndef AF_NETLINK
#error "arpa/inet.h:AF_NETLINK macro is missing from libc-shim"
#endif

#ifndef AF_NETROM
#error "arpa/inet.h:AF_NETROM macro is missing from libc-shim"
#endif

#ifndef AF_NFC
#error "arpa/inet.h:AF_NFC macro is missing from libc-shim"
#endif

#ifndef AF_PACKET
#error "arpa/inet.h:AF_PACKET macro is missing from libc-shim"
#endif

#ifndef AF_PHONET
#error "arpa/inet.h:AF_PHONET macro is missing from libc-shim"
#endif

#ifndef AF_PPPOX
#error "arpa/inet.h:AF_PPPOX macro is missing from libc-shim"
#endif

#ifndef AF_QIPCRTR
#error "arpa/inet.h:AF_QIPCRTR macro is missing from libc-shim"
#endif

#ifndef AF_RDS
#error "arpa/inet.h:AF_RDS macro is missing from libc-shim"
#endif

#ifndef AF_ROSE
#error "arpa/inet.h:AF_ROSE macro is missing from libc-shim"
#endif

#ifndef AF_ROUTE
#error "arpa/inet.h:AF_ROUTE macro is missing from libc-shim"
#endif

#ifndef AF_RXRPC
#error "arpa/inet.h:AF_RXRPC macro is missing from libc-shim"
#endif

#ifndef AF_SECURITY
#error "arpa/inet.h:AF_SECURITY macro is missing from libc-shim"
#endif

#ifndef AF_SMC
#error "arpa/inet.h:AF_SMC macro is missing from libc-shim"
#endif

#ifndef AF_SNA
#error "arpa/inet.h:AF_SNA macro is missing from libc-shim"
#endif

#ifndef AF_TIPC
#error "arpa/inet.h:AF_TIPC macro is missing from libc-shim"
#endif

#ifndef AF_UNIX
#error "arpa/inet.h:AF_UNIX macro is missing from libc-shim"
#endif

#ifndef AF_UNSPEC
#error "arpa/inet.h:AF_UNSPEC macro is missing from libc-shim"
#endif

#ifndef AF_VSOCK
#error "arpa/inet.h:AF_VSOCK macro is missing from libc-shim"
#endif

#ifndef AF_WANPIPE
#error "arpa/inet.h:AF_WANPIPE macro is missing from libc-shim"
#endif

#ifndef AF_X25
#error "arpa/inet.h:AF_X25 macro is missing from libc-shim"
#endif

#ifndef AF_XDP
#error "arpa/inet.h:AF_XDP macro is missing from libc-shim"
#endif

#ifndef CMSG_ALIGN
#error "arpa/inet.h:CMSG_ALIGN macro is missing from libc-shim"
#endif

#ifndef CMSG_DATA
#error "arpa/inet.h:CMSG_DATA macro is missing from libc-shim"
#endif

#ifndef CMSG_FIRSTHDR
#error "arpa/inet.h:CMSG_FIRSTHDR macro is missing from libc-shim"
#endif

#ifndef CMSG_LEN
#error "arpa/inet.h:CMSG_LEN macro is missing from libc-shim"
#endif

#ifndef CMSG_NXTHDR
#error "arpa/inet.h:CMSG_NXTHDR macro is missing from libc-shim"
#endif

#ifndef CMSG_SPACE
#error "arpa/inet.h:CMSG_SPACE macro is missing from libc-shim"
#endif

#ifndef IPV6_2292DSTOPTS
#error "arpa/inet.h:IPV6_2292DSTOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_2292HOPLIMIT
#error "arpa/inet.h:IPV6_2292HOPLIMIT macro is missing from libc-shim"
#endif

#ifndef IPV6_2292HOPOPTS
#error "arpa/inet.h:IPV6_2292HOPOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_2292PKTINFO
#error "arpa/inet.h:IPV6_2292PKTINFO macro is missing from libc-shim"
#endif

#ifndef IPV6_2292PKTOPTIONS
#error "arpa/inet.h:IPV6_2292PKTOPTIONS macro is missing from libc-shim"
#endif

#ifndef IPV6_2292RTHDR
#error "arpa/inet.h:IPV6_2292RTHDR macro is missing from libc-shim"
#endif

#ifndef IPV6_ADDRFORM
#error "arpa/inet.h:IPV6_ADDRFORM macro is missing from libc-shim"
#endif

#ifndef IPV6_ADDR_PREFERENCES
#error "arpa/inet.h:IPV6_ADDR_PREFERENCES macro is missing from libc-shim"
#endif

#ifndef IPV6_ADD_MEMBERSHIP
#error "arpa/inet.h:IPV6_ADD_MEMBERSHIP macro is missing from libc-shim"
#endif

#ifndef IPV6_AUTHHDR
#error "arpa/inet.h:IPV6_AUTHHDR macro is missing from libc-shim"
#endif

#ifndef IPV6_AUTOFLOWLABEL
#error "arpa/inet.h:IPV6_AUTOFLOWLABEL macro is missing from libc-shim"
#endif

#ifndef IPV6_CHECKSUM
#error "arpa/inet.h:IPV6_CHECKSUM macro is missing from libc-shim"
#endif

#ifndef IPV6_DONTFRAG
#error "arpa/inet.h:IPV6_DONTFRAG macro is missing from libc-shim"
#endif

#ifndef IPV6_DROP_MEMBERSHIP
#error "arpa/inet.h:IPV6_DROP_MEMBERSHIP macro is missing from libc-shim"
#endif

#ifndef IPV6_DSTOPTS
#error "arpa/inet.h:IPV6_DSTOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_FREEBIND
#error "arpa/inet.h:IPV6_FREEBIND macro is missing from libc-shim"
#endif

#ifndef IPV6_HDRINCL
#error "arpa/inet.h:IPV6_HDRINCL macro is missing from libc-shim"
#endif

#ifndef IPV6_HOPLIMIT
#error "arpa/inet.h:IPV6_HOPLIMIT macro is missing from libc-shim"
#endif

#ifndef IPV6_HOPOPTS
#error "arpa/inet.h:IPV6_HOPOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_IPSEC_POLICY
#error "arpa/inet.h:IPV6_IPSEC_POLICY macro is missing from libc-shim"
#endif

#ifndef IPV6_JOIN_ANYCAST
#error "arpa/inet.h:IPV6_JOIN_ANYCAST macro is missing from libc-shim"
#endif

#ifndef IPV6_JOIN_GROUP
#error "arpa/inet.h:IPV6_JOIN_GROUP macro is missing from libc-shim"
#endif

#ifndef IPV6_LEAVE_ANYCAST
#error "arpa/inet.h:IPV6_LEAVE_ANYCAST macro is missing from libc-shim"
#endif

#ifndef IPV6_LEAVE_GROUP
#error "arpa/inet.h:IPV6_LEAVE_GROUP macro is missing from libc-shim"
#endif

#ifndef IPV6_MINHOPCOUNT
#error "arpa/inet.h:IPV6_MINHOPCOUNT macro is missing from libc-shim"
#endif

#ifndef IPV6_MTU
#error "arpa/inet.h:IPV6_MTU macro is missing from libc-shim"
#endif

#ifndef IPV6_MTU_DISCOVER
#error "arpa/inet.h:IPV6_MTU_DISCOVER macro is missing from libc-shim"
#endif

#ifndef IPV6_MULTICAST_ALL
#error "arpa/inet.h:IPV6_MULTICAST_ALL macro is missing from libc-shim"
#endif

#ifndef IPV6_MULTICAST_HOPS
#error "arpa/inet.h:IPV6_MULTICAST_HOPS macro is missing from libc-shim"
#endif

#ifndef IPV6_MULTICAST_IF
#error "arpa/inet.h:IPV6_MULTICAST_IF macro is missing from libc-shim"
#endif

#ifndef IPV6_MULTICAST_LOOP
#error "arpa/inet.h:IPV6_MULTICAST_LOOP macro is missing from libc-shim"
#endif

#ifndef IPV6_NEXTHOP
#error "arpa/inet.h:IPV6_NEXTHOP macro is missing from libc-shim"
#endif

#ifndef IPV6_ORIGDSTADDR
#error "arpa/inet.h:IPV6_ORIGDSTADDR macro is missing from libc-shim"
#endif

#ifndef IPV6_PATHMTU
#error "arpa/inet.h:IPV6_PATHMTU macro is missing from libc-shim"
#endif

#ifndef IPV6_PKTINFO
#error "arpa/inet.h:IPV6_PKTINFO macro is missing from libc-shim"
#endif

#ifndef IPV6_PMTUDISC_DO
#error "arpa/inet.h:IPV6_PMTUDISC_DO macro is missing from libc-shim"
#endif

#ifndef IPV6_PMTUDISC_DONT
#error "arpa/inet.h:IPV6_PMTUDISC_DONT macro is missing from libc-shim"
#endif

#ifndef IPV6_PMTUDISC_INTERFACE
#error "arpa/inet.h:IPV6_PMTUDISC_INTERFACE macro is missing from libc-shim"
#endif

#ifndef IPV6_PMTUDISC_OMIT
#error "arpa/inet.h:IPV6_PMTUDISC_OMIT macro is missing from libc-shim"
#endif

#ifndef IPV6_PMTUDISC_PROBE
#error "arpa/inet.h:IPV6_PMTUDISC_PROBE macro is missing from libc-shim"
#endif

#ifndef IPV6_PMTUDISC_WANT
#error "arpa/inet.h:IPV6_PMTUDISC_WANT macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVDSTOPTS
#error "arpa/inet.h:IPV6_RECVDSTOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVERR
#error "arpa/inet.h:IPV6_RECVERR macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVERR_RFC4884
#error "arpa/inet.h:IPV6_RECVERR_RFC4884 macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVFRAGSIZE
#error "arpa/inet.h:IPV6_RECVFRAGSIZE macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVHOPLIMIT
#error "arpa/inet.h:IPV6_RECVHOPLIMIT macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVHOPOPTS
#error "arpa/inet.h:IPV6_RECVHOPOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVORIGDSTADDR
#error "arpa/inet.h:IPV6_RECVORIGDSTADDR macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVPATHMTU
#error "arpa/inet.h:IPV6_RECVPATHMTU macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVPKTINFO
#error "arpa/inet.h:IPV6_RECVPKTINFO macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVRTHDR
#error "arpa/inet.h:IPV6_RECVRTHDR macro is missing from libc-shim"
#endif

#ifndef IPV6_RECVTCLASS
#error "arpa/inet.h:IPV6_RECVTCLASS macro is missing from libc-shim"
#endif

#ifndef IPV6_ROUTER_ALERT
#error "arpa/inet.h:IPV6_ROUTER_ALERT macro is missing from libc-shim"
#endif

#ifndef IPV6_ROUTER_ALERT_ISOLATE
#error "arpa/inet.h:IPV6_ROUTER_ALERT_ISOLATE macro is missing from libc-shim"
#endif

#ifndef IPV6_RTHDR
#error "arpa/inet.h:IPV6_RTHDR macro is missing from libc-shim"
#endif

#ifndef IPV6_RTHDRDSTOPTS
#error "arpa/inet.h:IPV6_RTHDRDSTOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_RTHDR_LOOSE
#error "arpa/inet.h:IPV6_RTHDR_LOOSE macro is missing from libc-shim"
#endif

#ifndef IPV6_RTHDR_STRICT
#error "arpa/inet.h:IPV6_RTHDR_STRICT macro is missing from libc-shim"
#endif

#ifndef IPV6_RTHDR_TYPE_0
#error "arpa/inet.h:IPV6_RTHDR_TYPE_0 macro is missing from libc-shim"
#endif

#ifndef IPV6_RXDSTOPTS
#error "arpa/inet.h:IPV6_RXDSTOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_RXHOPOPTS
#error "arpa/inet.h:IPV6_RXHOPOPTS macro is missing from libc-shim"
#endif

#ifndef IPV6_TCLASS
#error "arpa/inet.h:IPV6_TCLASS macro is missing from libc-shim"
#endif

#ifndef IPV6_TRANSPARENT
#error "arpa/inet.h:IPV6_TRANSPARENT macro is missing from libc-shim"
#endif

#ifndef IPV6_UNICAST_HOPS
#error "arpa/inet.h:IPV6_UNICAST_HOPS macro is missing from libc-shim"
#endif

#ifndef IPV6_UNICAST_IF
#error "arpa/inet.h:IPV6_UNICAST_IF macro is missing from libc-shim"
#endif

#ifndef IPV6_V6ONLY
#error "arpa/inet.h:IPV6_V6ONLY macro is missing from libc-shim"
#endif

#ifndef IPV6_XFRM_POLICY
#error "arpa/inet.h:IPV6_XFRM_POLICY macro is missing from libc-shim"
#endif

#ifndef IP_ADD_MEMBERSHIP
#error "arpa/inet.h:IP_ADD_MEMBERSHIP macro is missing from libc-shim"
#endif

#ifndef IP_ADD_SOURCE_MEMBERSHIP
#error "arpa/inet.h:IP_ADD_SOURCE_MEMBERSHIP macro is missing from libc-shim"
#endif

#ifndef IP_BIND_ADDRESS_NO_PORT
#error "arpa/inet.h:IP_BIND_ADDRESS_NO_PORT macro is missing from libc-shim"
#endif

#ifndef IP_BLOCK_SOURCE
#error "arpa/inet.h:IP_BLOCK_SOURCE macro is missing from libc-shim"
#endif

#ifndef IP_CHECKSUM
#error "arpa/inet.h:IP_CHECKSUM macro is missing from libc-shim"
#endif

#ifndef IP_DEFAULT_MULTICAST_LOOP
#error "arpa/inet.h:IP_DEFAULT_MULTICAST_LOOP macro is missing from libc-shim"
#endif

#ifndef IP_DEFAULT_MULTICAST_TTL
#error "arpa/inet.h:IP_DEFAULT_MULTICAST_TTL macro is missing from libc-shim"
#endif

#ifndef IP_DROP_MEMBERSHIP
#error "arpa/inet.h:IP_DROP_MEMBERSHIP macro is missing from libc-shim"
#endif

#ifndef IP_DROP_SOURCE_MEMBERSHIP
#error "arpa/inet.h:IP_DROP_SOURCE_MEMBERSHIP macro is missing from libc-shim"
#endif

#ifndef IP_FREEBIND
#error "arpa/inet.h:IP_FREEBIND macro is missing from libc-shim"
#endif

#ifndef IP_HDRINCL
#error "arpa/inet.h:IP_HDRINCL macro is missing from libc-shim"
#endif

#ifndef IP_IPSEC_POLICY
#error "arpa/inet.h:IP_IPSEC_POLICY macro is missing from libc-shim"
#endif

#ifndef IP_LOCAL_PORT_RANGE
#error "arpa/inet.h:IP_LOCAL_PORT_RANGE macro is missing from libc-shim"
#endif

#ifndef IP_MAX_MEMBERSHIPS
#error "arpa/inet.h:IP_MAX_MEMBERSHIPS macro is missing from libc-shim"
#endif

#ifndef IP_MINTTL
#error "arpa/inet.h:IP_MINTTL macro is missing from libc-shim"
#endif

#ifndef IP_MSFILTER
#error "arpa/inet.h:IP_MSFILTER macro is missing from libc-shim"
#endif

#ifndef IP_MTU
#error "arpa/inet.h:IP_MTU macro is missing from libc-shim"
#endif

#ifndef IP_MTU_DISCOVER
#error "arpa/inet.h:IP_MTU_DISCOVER macro is missing from libc-shim"
#endif

#ifndef IP_MULTICAST_ALL
#error "arpa/inet.h:IP_MULTICAST_ALL macro is missing from libc-shim"
#endif

#ifndef IP_MULTICAST_IF
#error "arpa/inet.h:IP_MULTICAST_IF macro is missing from libc-shim"
#endif

#ifndef IP_MULTICAST_LOOP
#error "arpa/inet.h:IP_MULTICAST_LOOP macro is missing from libc-shim"
#endif

#ifndef IP_MULTICAST_TTL
#error "arpa/inet.h:IP_MULTICAST_TTL macro is missing from libc-shim"
#endif

#ifndef IP_NODEFRAG
#error "arpa/inet.h:IP_NODEFRAG macro is missing from libc-shim"
#endif

#ifndef IP_OPTIONS
#error "arpa/inet.h:IP_OPTIONS macro is missing from libc-shim"
#endif

#ifndef IP_ORIGDSTADDR
#error "arpa/inet.h:IP_ORIGDSTADDR macro is missing from libc-shim"
#endif

#ifndef IP_PASSSEC
#error "arpa/inet.h:IP_PASSSEC macro is missing from libc-shim"
#endif

#ifndef IP_PKTINFO
#error "arpa/inet.h:IP_PKTINFO macro is missing from libc-shim"
#endif

#ifndef IP_PKTOPTIONS
#error "arpa/inet.h:IP_PKTOPTIONS macro is missing from libc-shim"
#endif

#ifndef IP_PMTUDISC
#error "arpa/inet.h:IP_PMTUDISC macro is missing from libc-shim"
#endif

#ifndef IP_PMTUDISC_DO
#error "arpa/inet.h:IP_PMTUDISC_DO macro is missing from libc-shim"
#endif

#ifndef IP_PMTUDISC_DONT
#error "arpa/inet.h:IP_PMTUDISC_DONT macro is missing from libc-shim"
#endif

#ifndef IP_PMTUDISC_INTERFACE
#error "arpa/inet.h:IP_PMTUDISC_INTERFACE macro is missing from libc-shim"
#endif

#ifndef IP_PMTUDISC_OMIT
#error "arpa/inet.h:IP_PMTUDISC_OMIT macro is missing from libc-shim"
#endif

#ifndef IP_PMTUDISC_PROBE
#error "arpa/inet.h:IP_PMTUDISC_PROBE macro is missing from libc-shim"
#endif

#ifndef IP_PMTUDISC_WANT
#error "arpa/inet.h:IP_PMTUDISC_WANT macro is missing from libc-shim"
#endif

#ifndef IP_PROTOCOL
#error "arpa/inet.h:IP_PROTOCOL macro is missing from libc-shim"
#endif

#ifndef IP_RECVERR
#error "arpa/inet.h:IP_RECVERR macro is missing from libc-shim"
#endif

#ifndef IP_RECVERR_RFC4884
#error "arpa/inet.h:IP_RECVERR_RFC4884 macro is missing from libc-shim"
#endif

#ifndef IP_RECVFRAGSIZE
#error "arpa/inet.h:IP_RECVFRAGSIZE macro is missing from libc-shim"
#endif

#ifndef IP_RECVOPTS
#error "arpa/inet.h:IP_RECVOPTS macro is missing from libc-shim"
#endif

#ifndef IP_RECVORIGDSTADDR
#error "arpa/inet.h:IP_RECVORIGDSTADDR macro is missing from libc-shim"
#endif

#ifndef IP_RECVRETOPTS
#error "arpa/inet.h:IP_RECVRETOPTS macro is missing from libc-shim"
#endif

#ifndef IP_RECVTOS
#error "arpa/inet.h:IP_RECVTOS macro is missing from libc-shim"
#endif

#ifndef IP_RECVTTL
#error "arpa/inet.h:IP_RECVTTL macro is missing from libc-shim"
#endif

#ifndef IP_RETOPTS
#error "arpa/inet.h:IP_RETOPTS macro is missing from libc-shim"
#endif

#ifndef IP_ROUTER_ALERT
#error "arpa/inet.h:IP_ROUTER_ALERT macro is missing from libc-shim"
#endif

#ifndef IP_TOS
#error "arpa/inet.h:IP_TOS macro is missing from libc-shim"
#endif

#ifndef IP_TRANSPARENT
#error "arpa/inet.h:IP_TRANSPARENT macro is missing from libc-shim"
#endif

#ifndef IP_TTL
#error "arpa/inet.h:IP_TTL macro is missing from libc-shim"
#endif

#ifndef IP_UNBLOCK_SOURCE
#error "arpa/inet.h:IP_UNBLOCK_SOURCE macro is missing from libc-shim"
#endif

#ifndef IP_UNICAST_IF
#error "arpa/inet.h:IP_UNICAST_IF macro is missing from libc-shim"
#endif

#ifndef IP_XFRM_POLICY
#error "arpa/inet.h:IP_XFRM_POLICY macro is missing from libc-shim"
#endif

#ifndef MCAST_BLOCK_SOURCE
#error "arpa/inet.h:MCAST_BLOCK_SOURCE macro is missing from libc-shim"
#endif

#ifndef MCAST_EXCLUDE
#error "arpa/inet.h:MCAST_EXCLUDE macro is missing from libc-shim"
#endif

#ifndef MCAST_INCLUDE
#error "arpa/inet.h:MCAST_INCLUDE macro is missing from libc-shim"
#endif

#ifndef MCAST_JOIN_GROUP
#error "arpa/inet.h:MCAST_JOIN_GROUP macro is missing from libc-shim"
#endif

#ifndef MCAST_JOIN_SOURCE_GROUP
#error "arpa/inet.h:MCAST_JOIN_SOURCE_GROUP macro is missing from libc-shim"
#endif

#ifndef MCAST_LEAVE_GROUP
#error "arpa/inet.h:MCAST_LEAVE_GROUP macro is missing from libc-shim"
#endif

#ifndef MCAST_LEAVE_SOURCE_GROUP
#error "arpa/inet.h:MCAST_LEAVE_SOURCE_GROUP macro is missing from libc-shim"
#endif

#ifndef MCAST_MSFILTER
#error "arpa/inet.h:MCAST_MSFILTER macro is missing from libc-shim"
#endif

#ifndef MCAST_UNBLOCK_SOURCE
#error "arpa/inet.h:MCAST_UNBLOCK_SOURCE macro is missing from libc-shim"
#endif

#ifndef MSG_BATCH
#error "arpa/inet.h:MSG_BATCH macro is missing from libc-shim"
#endif

#ifndef MSG_CMSG_CLOEXEC
#error "arpa/inet.h:MSG_CMSG_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef MSG_CONFIRM
#error "arpa/inet.h:MSG_CONFIRM macro is missing from libc-shim"
#endif

#ifndef MSG_CTRUNC
#error "arpa/inet.h:MSG_CTRUNC macro is missing from libc-shim"
#endif

#ifndef MSG_DONTROUTE
#error "arpa/inet.h:MSG_DONTROUTE macro is missing from libc-shim"
#endif

#ifndef MSG_DONTWAIT
#error "arpa/inet.h:MSG_DONTWAIT macro is missing from libc-shim"
#endif

#ifndef MSG_EOR
#error "arpa/inet.h:MSG_EOR macro is missing from libc-shim"
#endif

#ifndef MSG_ERRQUEUE
#error "arpa/inet.h:MSG_ERRQUEUE macro is missing from libc-shim"
#endif

#ifndef MSG_FASTOPEN
#error "arpa/inet.h:MSG_FASTOPEN macro is missing from libc-shim"
#endif

#ifndef MSG_FIN
#error "arpa/inet.h:MSG_FIN macro is missing from libc-shim"
#endif

#ifndef MSG_MORE
#error "arpa/inet.h:MSG_MORE macro is missing from libc-shim"
#endif

#ifndef MSG_NOSIGNAL
#error "arpa/inet.h:MSG_NOSIGNAL macro is missing from libc-shim"
#endif

#ifndef MSG_OOB
#error "arpa/inet.h:MSG_OOB macro is missing from libc-shim"
#endif

#ifndef MSG_PEEK
#error "arpa/inet.h:MSG_PEEK macro is missing from libc-shim"
#endif

#ifndef MSG_PROXY
#error "arpa/inet.h:MSG_PROXY macro is missing from libc-shim"
#endif

#ifndef MSG_RST
#error "arpa/inet.h:MSG_RST macro is missing from libc-shim"
#endif

#ifndef MSG_SOCK_DEVMEM
#error "arpa/inet.h:MSG_SOCK_DEVMEM macro is missing from libc-shim"
#endif

#ifndef MSG_SYN
#error "arpa/inet.h:MSG_SYN macro is missing from libc-shim"
#endif

#ifndef MSG_TRUNC
#error "arpa/inet.h:MSG_TRUNC macro is missing from libc-shim"
#endif

#ifndef MSG_TRYHARD
#error "arpa/inet.h:MSG_TRYHARD macro is missing from libc-shim"
#endif

#ifndef MSG_WAITALL
#error "arpa/inet.h:MSG_WAITALL macro is missing from libc-shim"
#endif

#ifndef MSG_WAITFORONE
#error "arpa/inet.h:MSG_WAITFORONE macro is missing from libc-shim"
#endif

#ifndef MSG_ZEROCOPY
#error "arpa/inet.h:MSG_ZEROCOPY macro is missing from libc-shim"
#endif

#ifndef PF_ALG
#error "arpa/inet.h:PF_ALG macro is missing from libc-shim"
#endif

#ifndef PF_APPLETALK
#error "arpa/inet.h:PF_APPLETALK macro is missing from libc-shim"
#endif

#ifndef PF_ASH
#error "arpa/inet.h:PF_ASH macro is missing from libc-shim"
#endif

#ifndef PF_ATMPVC
#error "arpa/inet.h:PF_ATMPVC macro is missing from libc-shim"
#endif

#ifndef PF_ATMSVC
#error "arpa/inet.h:PF_ATMSVC macro is missing from libc-shim"
#endif

#ifndef PF_AX25
#error "arpa/inet.h:PF_AX25 macro is missing from libc-shim"
#endif

#ifndef PF_BLUETOOTH
#error "arpa/inet.h:PF_BLUETOOTH macro is missing from libc-shim"
#endif

#ifndef PF_BRIDGE
#error "arpa/inet.h:PF_BRIDGE macro is missing from libc-shim"
#endif

#ifndef PF_CAIF
#error "arpa/inet.h:PF_CAIF macro is missing from libc-shim"
#endif

#ifndef PF_CAN
#error "arpa/inet.h:PF_CAN macro is missing from libc-shim"
#endif

#ifndef PF_DECnet
#error "arpa/inet.h:PF_DECnet macro is missing from libc-shim"
#endif

#ifndef PF_ECONET
#error "arpa/inet.h:PF_ECONET macro is missing from libc-shim"
#endif

#ifndef PF_FILE
#error "arpa/inet.h:PF_FILE macro is missing from libc-shim"
#endif

#ifndef PF_IB
#error "arpa/inet.h:PF_IB macro is missing from libc-shim"
#endif

#ifndef PF_IEEE802154
#error "arpa/inet.h:PF_IEEE802154 macro is missing from libc-shim"
#endif

#ifndef PF_INET
#error "arpa/inet.h:PF_INET macro is missing from libc-shim"
#endif

#ifndef PF_INET6
#error "arpa/inet.h:PF_INET6 macro is missing from libc-shim"
#endif

#ifndef PF_IPX
#error "arpa/inet.h:PF_IPX macro is missing from libc-shim"
#endif

#ifndef PF_IRDA
#error "arpa/inet.h:PF_IRDA macro is missing from libc-shim"
#endif

#ifndef PF_ISDN
#error "arpa/inet.h:PF_ISDN macro is missing from libc-shim"
#endif

#ifndef PF_IUCV
#error "arpa/inet.h:PF_IUCV macro is missing from libc-shim"
#endif

#ifndef PF_KCM
#error "arpa/inet.h:PF_KCM macro is missing from libc-shim"
#endif

#ifndef PF_KEY
#error "arpa/inet.h:PF_KEY macro is missing from libc-shim"
#endif

#ifndef PF_LLC
#error "arpa/inet.h:PF_LLC macro is missing from libc-shim"
#endif

#ifndef PF_LOCAL
#error "arpa/inet.h:PF_LOCAL macro is missing from libc-shim"
#endif

#ifndef PF_MAX
#error "arpa/inet.h:PF_MAX macro is missing from libc-shim"
#endif

#ifndef PF_MCTP
#error "arpa/inet.h:PF_MCTP macro is missing from libc-shim"
#endif

#ifndef PF_MPLS
#error "arpa/inet.h:PF_MPLS macro is missing from libc-shim"
#endif

#ifndef PF_NETBEUI
#error "arpa/inet.h:PF_NETBEUI macro is missing from libc-shim"
#endif

#ifndef PF_NETLINK
#error "arpa/inet.h:PF_NETLINK macro is missing from libc-shim"
#endif

#ifndef PF_NETROM
#error "arpa/inet.h:PF_NETROM macro is missing from libc-shim"
#endif

#ifndef PF_NFC
#error "arpa/inet.h:PF_NFC macro is missing from libc-shim"
#endif

#ifndef PF_PACKET
#error "arpa/inet.h:PF_PACKET macro is missing from libc-shim"
#endif

#ifndef PF_PHONET
#error "arpa/inet.h:PF_PHONET macro is missing from libc-shim"
#endif

#ifndef PF_PPPOX
#error "arpa/inet.h:PF_PPPOX macro is missing from libc-shim"
#endif

#ifndef PF_QIPCRTR
#error "arpa/inet.h:PF_QIPCRTR macro is missing from libc-shim"
#endif

#ifndef PF_RDS
#error "arpa/inet.h:PF_RDS macro is missing from libc-shim"
#endif

#ifndef PF_ROSE
#error "arpa/inet.h:PF_ROSE macro is missing from libc-shim"
#endif

#ifndef PF_ROUTE
#error "arpa/inet.h:PF_ROUTE macro is missing from libc-shim"
#endif

#ifndef PF_RXRPC
#error "arpa/inet.h:PF_RXRPC macro is missing from libc-shim"
#endif

#ifndef PF_SECURITY
#error "arpa/inet.h:PF_SECURITY macro is missing from libc-shim"
#endif

#ifndef PF_SMC
#error "arpa/inet.h:PF_SMC macro is missing from libc-shim"
#endif

#ifndef PF_SNA
#error "arpa/inet.h:PF_SNA macro is missing from libc-shim"
#endif

#ifndef PF_TIPC
#error "arpa/inet.h:PF_TIPC macro is missing from libc-shim"
#endif

#ifndef PF_UNIX
#error "arpa/inet.h:PF_UNIX macro is missing from libc-shim"
#endif

#ifndef PF_UNSPEC
#error "arpa/inet.h:PF_UNSPEC macro is missing from libc-shim"
#endif

#ifndef PF_VSOCK
#error "arpa/inet.h:PF_VSOCK macro is missing from libc-shim"
#endif

#ifndef PF_WANPIPE
#error "arpa/inet.h:PF_WANPIPE macro is missing from libc-shim"
#endif

#ifndef PF_X25
#error "arpa/inet.h:PF_X25 macro is missing from libc-shim"
#endif

#ifndef PF_XDP
#error "arpa/inet.h:PF_XDP macro is missing from libc-shim"
#endif

#ifndef SCM_CREDENTIALS
#error "arpa/inet.h:SCM_CREDENTIALS macro is missing from libc-shim"
#endif

#ifndef SCM_PIDFD
#error "arpa/inet.h:SCM_PIDFD macro is missing from libc-shim"
#endif

#ifndef SCM_RIGHTS
#error "arpa/inet.h:SCM_RIGHTS macro is missing from libc-shim"
#endif

#ifndef SCM_SECURITY
#error "arpa/inet.h:SCM_SECURITY macro is missing from libc-shim"
#endif

#ifndef SCM_SRCRT
#error "arpa/inet.h:SCM_SRCRT macro is missing from libc-shim"
#endif

#ifndef SOCK_CLOEXEC
#error "arpa/inet.h:SOCK_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef SOCK_DCCP
#error "arpa/inet.h:SOCK_DCCP macro is missing from libc-shim"
#endif

#ifndef SOCK_DGRAM
#error "arpa/inet.h:SOCK_DGRAM macro is missing from libc-shim"
#endif

#ifndef SOCK_NONBLOCK
#error "arpa/inet.h:SOCK_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef SOCK_PACKET
#error "arpa/inet.h:SOCK_PACKET macro is missing from libc-shim"
#endif

#ifndef SOCK_RAW
#error "arpa/inet.h:SOCK_RAW macro is missing from libc-shim"
#endif

#ifndef SOCK_RDM
#error "arpa/inet.h:SOCK_RDM macro is missing from libc-shim"
#endif

#ifndef SOCK_SEQPACKET
#error "arpa/inet.h:SOCK_SEQPACKET macro is missing from libc-shim"
#endif

#ifndef SOCK_STREAM
#error "arpa/inet.h:SOCK_STREAM macro is missing from libc-shim"
#endif

#ifndef SOL_AAL
#error "arpa/inet.h:SOL_AAL macro is missing from libc-shim"
#endif

#ifndef SOL_ALG
#error "arpa/inet.h:SOL_ALG macro is missing from libc-shim"
#endif

#ifndef SOL_ATM
#error "arpa/inet.h:SOL_ATM macro is missing from libc-shim"
#endif

#ifndef SOL_BLUETOOTH
#error "arpa/inet.h:SOL_BLUETOOTH macro is missing from libc-shim"
#endif

#ifndef SOL_CAIF
#error "arpa/inet.h:SOL_CAIF macro is missing from libc-shim"
#endif

#ifndef SOL_DCCP
#error "arpa/inet.h:SOL_DCCP macro is missing from libc-shim"
#endif

#ifndef SOL_DECNET
#error "arpa/inet.h:SOL_DECNET macro is missing from libc-shim"
#endif

#ifndef SOL_ICMPV6
#error "arpa/inet.h:SOL_ICMPV6 macro is missing from libc-shim"
#endif

#ifndef SOL_IP
#error "arpa/inet.h:SOL_IP macro is missing from libc-shim"
#endif

#ifndef SOL_IPV6
#error "arpa/inet.h:SOL_IPV6 macro is missing from libc-shim"
#endif

#ifndef SOL_IRDA
#error "arpa/inet.h:SOL_IRDA macro is missing from libc-shim"
#endif

#ifndef SOL_IUCV
#error "arpa/inet.h:SOL_IUCV macro is missing from libc-shim"
#endif

#ifndef SOL_KCM
#error "arpa/inet.h:SOL_KCM macro is missing from libc-shim"
#endif

#ifndef SOL_LLC
#error "arpa/inet.h:SOL_LLC macro is missing from libc-shim"
#endif

#ifndef SOL_MCTP
#error "arpa/inet.h:SOL_MCTP macro is missing from libc-shim"
#endif

#ifndef SOL_MPTCP
#error "arpa/inet.h:SOL_MPTCP macro is missing from libc-shim"
#endif

#ifndef SOL_NETBEUI
#error "arpa/inet.h:SOL_NETBEUI macro is missing from libc-shim"
#endif

#ifndef SOL_NETLINK
#error "arpa/inet.h:SOL_NETLINK macro is missing from libc-shim"
#endif

#ifndef SOL_NFC
#error "arpa/inet.h:SOL_NFC macro is missing from libc-shim"
#endif

#ifndef SOL_PACKET
#error "arpa/inet.h:SOL_PACKET macro is missing from libc-shim"
#endif

#ifndef SOL_PNPIPE
#error "arpa/inet.h:SOL_PNPIPE macro is missing from libc-shim"
#endif

#ifndef SOL_PPPOL2TP
#error "arpa/inet.h:SOL_PPPOL2TP macro is missing from libc-shim"
#endif

#ifndef SOL_RAW
#error "arpa/inet.h:SOL_RAW macro is missing from libc-shim"
#endif

#ifndef SOL_RDS
#error "arpa/inet.h:SOL_RDS macro is missing from libc-shim"
#endif

#ifndef SOL_RXRPC
#error "arpa/inet.h:SOL_RXRPC macro is missing from libc-shim"
#endif

#ifndef SOL_SMC
#error "arpa/inet.h:SOL_SMC macro is missing from libc-shim"
#endif

#ifndef SOL_TIPC
#error "arpa/inet.h:SOL_TIPC macro is missing from libc-shim"
#endif

#ifndef SOL_TLS
#error "arpa/inet.h:SOL_TLS macro is missing from libc-shim"
#endif

#ifndef SOL_VSOCK
#error "arpa/inet.h:SOL_VSOCK macro is missing from libc-shim"
#endif

#ifndef SOL_X25
#error "arpa/inet.h:SOL_X25 macro is missing from libc-shim"
#endif

#ifndef SOL_XDP
#error "arpa/inet.h:SOL_XDP macro is missing from libc-shim"
#endif

#ifndef SOMAXCONN
#error "arpa/inet.h:SOMAXCONN macro is missing from libc-shim"
#endif

int main(void) { return 0; }
