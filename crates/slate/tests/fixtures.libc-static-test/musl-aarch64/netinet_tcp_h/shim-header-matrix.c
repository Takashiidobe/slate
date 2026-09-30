#include <netinet/tcp.h>

typedef unsigned int slate_oracle_typedef_tcp_seq;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_tcp_seq, tcp_seq), "typedef tcp_seq differs from oracle");

#ifndef SOL_TCP
#error "netinet/tcp.h:SOL_TCP macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_ECN
#error "netinet/tcp.h:TCPI_OPT_ECN macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_SACK
#error "netinet/tcp.h:TCPI_OPT_SACK macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_TIMESTAMPS
#error "netinet/tcp.h:TCPI_OPT_TIMESTAMPS macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_WSCALE
#error "netinet/tcp.h:TCPI_OPT_WSCALE macro is missing from libc-shim"
#endif

#ifndef TCPOLEN_MAXSEG
#error "netinet/tcp.h:TCPOLEN_MAXSEG macro is missing from libc-shim"
#endif

#ifndef TCPOLEN_SACK_PERMITTED
#error "netinet/tcp.h:TCPOLEN_SACK_PERMITTED macro is missing from libc-shim"
#endif

#ifndef TCPOLEN_TIMESTAMP
#error "netinet/tcp.h:TCPOLEN_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef TCPOLEN_WINDOW
#error "netinet/tcp.h:TCPOLEN_WINDOW macro is missing from libc-shim"
#endif

#ifndef TCPOPT_EOL
#error "netinet/tcp.h:TCPOPT_EOL macro is missing from libc-shim"
#endif

#ifndef TCPOPT_MAXSEG
#error "netinet/tcp.h:TCPOPT_MAXSEG macro is missing from libc-shim"
#endif

#ifndef TCPOPT_NOP
#error "netinet/tcp.h:TCPOPT_NOP macro is missing from libc-shim"
#endif

#ifndef TCPOPT_SACK
#error "netinet/tcp.h:TCPOPT_SACK macro is missing from libc-shim"
#endif

#ifndef TCPOPT_SACK_PERMITTED
#error "netinet/tcp.h:TCPOPT_SACK_PERMITTED macro is missing from libc-shim"
#endif

#ifndef TCPOPT_TIMESTAMP
#error "netinet/tcp.h:TCPOPT_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef TCPOPT_WINDOW
#error "netinet/tcp.h:TCPOPT_WINDOW macro is missing from libc-shim"
#endif

#ifndef TCP_CA_CWR
#error "netinet/tcp.h:TCP_CA_CWR macro is missing from libc-shim"
#endif

#ifndef TCP_CA_Disorder
#error "netinet/tcp.h:TCP_CA_Disorder macro is missing from libc-shim"
#endif

#ifndef TCP_CA_Loss
#error "netinet/tcp.h:TCP_CA_Loss macro is missing from libc-shim"
#endif

#ifndef TCP_CA_Open
#error "netinet/tcp.h:TCP_CA_Open macro is missing from libc-shim"
#endif

#ifndef TCP_CA_Recovery
#error "netinet/tcp.h:TCP_CA_Recovery macro is missing from libc-shim"
#endif

#ifndef TCP_CC_INFO
#error "netinet/tcp.h:TCP_CC_INFO macro is missing from libc-shim"
#endif

#ifndef TCP_CLOSE
#error "netinet/tcp.h:TCP_CLOSE macro is missing from libc-shim"
#endif

#ifndef TCP_CLOSE_WAIT
#error "netinet/tcp.h:TCP_CLOSE_WAIT macro is missing from libc-shim"
#endif

#ifndef TCP_CLOSING
#error "netinet/tcp.h:TCP_CLOSING macro is missing from libc-shim"
#endif

#ifndef TCP_CM_INQ
#error "netinet/tcp.h:TCP_CM_INQ macro is missing from libc-shim"
#endif

#ifndef TCP_CONGESTION
#error "netinet/tcp.h:TCP_CONGESTION macro is missing from libc-shim"
#endif

#ifndef TCP_CORK
#error "netinet/tcp.h:TCP_CORK macro is missing from libc-shim"
#endif

#ifndef TCP_DEFER_ACCEPT
#error "netinet/tcp.h:TCP_DEFER_ACCEPT macro is missing from libc-shim"
#endif

#ifndef TCP_ESTABLISHED
#error "netinet/tcp.h:TCP_ESTABLISHED macro is missing from libc-shim"
#endif

#ifndef TCP_FASTOPEN
#error "netinet/tcp.h:TCP_FASTOPEN macro is missing from libc-shim"
#endif

#ifndef TCP_FASTOPEN_CONNECT
#error "netinet/tcp.h:TCP_FASTOPEN_CONNECT macro is missing from libc-shim"
#endif

#ifndef TCP_FASTOPEN_KEY
#error "netinet/tcp.h:TCP_FASTOPEN_KEY macro is missing from libc-shim"
#endif

#ifndef TCP_FASTOPEN_NO_COOKIE
#error "netinet/tcp.h:TCP_FASTOPEN_NO_COOKIE macro is missing from libc-shim"
#endif

#ifndef TCP_FIN_WAIT1
#error "netinet/tcp.h:TCP_FIN_WAIT1 macro is missing from libc-shim"
#endif

#ifndef TCP_FIN_WAIT2
#error "netinet/tcp.h:TCP_FIN_WAIT2 macro is missing from libc-shim"
#endif

#ifndef TCP_INFO
#error "netinet/tcp.h:TCP_INFO macro is missing from libc-shim"
#endif

#ifndef TCP_INQ
#error "netinet/tcp.h:TCP_INQ macro is missing from libc-shim"
#endif

#ifndef TCP_KEEPCNT
#error "netinet/tcp.h:TCP_KEEPCNT macro is missing from libc-shim"
#endif

#ifndef TCP_KEEPIDLE
#error "netinet/tcp.h:TCP_KEEPIDLE macro is missing from libc-shim"
#endif

#ifndef TCP_KEEPINTVL
#error "netinet/tcp.h:TCP_KEEPINTVL macro is missing from libc-shim"
#endif

#ifndef TCP_LAST_ACK
#error "netinet/tcp.h:TCP_LAST_ACK macro is missing from libc-shim"
#endif

#ifndef TCP_LINGER2
#error "netinet/tcp.h:TCP_LINGER2 macro is missing from libc-shim"
#endif

#ifndef TCP_LISTEN
#error "netinet/tcp.h:TCP_LISTEN macro is missing from libc-shim"
#endif

#ifndef TCP_MAXSEG
#error "netinet/tcp.h:TCP_MAXSEG macro is missing from libc-shim"
#endif

#ifndef TCP_MD5SIG
#error "netinet/tcp.h:TCP_MD5SIG macro is missing from libc-shim"
#endif

#ifndef TCP_MD5SIG_EXT
#error "netinet/tcp.h:TCP_MD5SIG_EXT macro is missing from libc-shim"
#endif

#ifndef TCP_MD5SIG_FLAG_IFINDEX
#error "netinet/tcp.h:TCP_MD5SIG_FLAG_IFINDEX macro is missing from libc-shim"
#endif

#ifndef TCP_MD5SIG_FLAG_PREFIX
#error "netinet/tcp.h:TCP_MD5SIG_FLAG_PREFIX macro is missing from libc-shim"
#endif

#ifndef TCP_MD5SIG_MAXKEYLEN
#error "netinet/tcp.h:TCP_MD5SIG_MAXKEYLEN macro is missing from libc-shim"
#endif

#ifndef TCP_NODELAY
#error "netinet/tcp.h:TCP_NODELAY macro is missing from libc-shim"
#endif

#ifndef TCP_NOTSENT_LOWAT
#error "netinet/tcp.h:TCP_NOTSENT_LOWAT macro is missing from libc-shim"
#endif

#ifndef TCP_QUEUE_SEQ
#error "netinet/tcp.h:TCP_QUEUE_SEQ macro is missing from libc-shim"
#endif

#ifndef TCP_QUICKACK
#error "netinet/tcp.h:TCP_QUICKACK macro is missing from libc-shim"
#endif

#ifndef TCP_RECEIVE_ZEROCOPY_FLAG_TLB_CLEAN_HINT
#error "netinet/tcp.h:TCP_RECEIVE_ZEROCOPY_FLAG_TLB_CLEAN_HINT macro is missing from libc-shim"
#endif

#ifndef TCP_REPAIR
#error "netinet/tcp.h:TCP_REPAIR macro is missing from libc-shim"
#endif

#ifndef TCP_REPAIR_OFF
#error "netinet/tcp.h:TCP_REPAIR_OFF macro is missing from libc-shim"
#endif

#ifndef TCP_REPAIR_OFF_NO_WP
#error "netinet/tcp.h:TCP_REPAIR_OFF_NO_WP macro is missing from libc-shim"
#endif

#ifndef TCP_REPAIR_ON
#error "netinet/tcp.h:TCP_REPAIR_ON macro is missing from libc-shim"
#endif

#ifndef TCP_REPAIR_OPTIONS
#error "netinet/tcp.h:TCP_REPAIR_OPTIONS macro is missing from libc-shim"
#endif

#ifndef TCP_REPAIR_QUEUE
#error "netinet/tcp.h:TCP_REPAIR_QUEUE macro is missing from libc-shim"
#endif

#ifndef TCP_REPAIR_WINDOW
#error "netinet/tcp.h:TCP_REPAIR_WINDOW macro is missing from libc-shim"
#endif

#ifndef TCP_SAVED_SYN
#error "netinet/tcp.h:TCP_SAVED_SYN macro is missing from libc-shim"
#endif

#ifndef TCP_SAVE_SYN
#error "netinet/tcp.h:TCP_SAVE_SYN macro is missing from libc-shim"
#endif

#ifndef TCP_SYNCNT
#error "netinet/tcp.h:TCP_SYNCNT macro is missing from libc-shim"
#endif

#ifndef TCP_SYN_RECV
#error "netinet/tcp.h:TCP_SYN_RECV macro is missing from libc-shim"
#endif

#ifndef TCP_SYN_SENT
#error "netinet/tcp.h:TCP_SYN_SENT macro is missing from libc-shim"
#endif

#ifndef TCP_THIN_DUPACK
#error "netinet/tcp.h:TCP_THIN_DUPACK macro is missing from libc-shim"
#endif

#ifndef TCP_THIN_LINEAR_TIMEOUTS
#error "netinet/tcp.h:TCP_THIN_LINEAR_TIMEOUTS macro is missing from libc-shim"
#endif

#ifndef TCP_TIMESTAMP
#error "netinet/tcp.h:TCP_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef TCP_TIME_WAIT
#error "netinet/tcp.h:TCP_TIME_WAIT macro is missing from libc-shim"
#endif

#ifndef TCP_TX_DELAY
#error "netinet/tcp.h:TCP_TX_DELAY macro is missing from libc-shim"
#endif

#ifndef TCP_ULP
#error "netinet/tcp.h:TCP_ULP macro is missing from libc-shim"
#endif

#ifndef TCP_USER_TIMEOUT
#error "netinet/tcp.h:TCP_USER_TIMEOUT macro is missing from libc-shim"
#endif

#ifndef TCP_WINDOW_CLAMP
#error "netinet/tcp.h:TCP_WINDOW_CLAMP macro is missing from libc-shim"
#endif

#ifndef TCP_ZEROCOPY_RECEIVE
#error "netinet/tcp.h:TCP_ZEROCOPY_RECEIVE macro is missing from libc-shim"
#endif

#ifndef TH_ACK
#error "netinet/tcp.h:TH_ACK macro is missing from libc-shim"
#endif

#ifndef TH_FIN
#error "netinet/tcp.h:TH_FIN macro is missing from libc-shim"
#endif

#ifndef TH_PUSH
#error "netinet/tcp.h:TH_PUSH macro is missing from libc-shim"
#endif

#ifndef TH_RST
#error "netinet/tcp.h:TH_RST macro is missing from libc-shim"
#endif

#ifndef TH_SYN
#error "netinet/tcp.h:TH_SYN macro is missing from libc-shim"
#endif

#ifndef TH_URG
#error "netinet/tcp.h:TH_URG macro is missing from libc-shim"
#endif

int main(void) { return 0; }
