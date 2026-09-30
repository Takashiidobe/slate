#include <netinet/tcp.h>

typedef unsigned int slate_oracle_typedef_tcp_seq;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_tcp_seq, tcp_seq), "typedef tcp_seq differs from oracle");

#ifndef SOL_TCP
#error "netinet/tcp.h:SOL_TCP macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_ECN
#error "netinet/tcp.h:TCPI_OPT_ECN macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_ECN_SEEN
#error "netinet/tcp.h:TCPI_OPT_ECN_SEEN macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_SACK
#error "netinet/tcp.h:TCPI_OPT_SACK macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_SYN_DATA
#error "netinet/tcp.h:TCPI_OPT_SYN_DATA macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_TFO_CHILD
#error "netinet/tcp.h:TCPI_OPT_TFO_CHILD macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_TIMESTAMPS
#error "netinet/tcp.h:TCPI_OPT_TIMESTAMPS macro is missing from libc-shim"
#endif

#ifndef TCPI_OPT_USEC_TS
#error "netinet/tcp.h:TCPI_OPT_USEC_TS macro is missing from libc-shim"
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

#ifndef TCPOLEN_TSTAMP_APPA
#error "netinet/tcp.h:TCPOLEN_TSTAMP_APPA macro is missing from libc-shim"
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

#ifndef TCPOPT_TSTAMP_HDR
#error "netinet/tcp.h:TCPOPT_TSTAMP_HDR macro is missing from libc-shim"
#endif

#ifndef TCPOPT_WINDOW
#error "netinet/tcp.h:TCPOPT_WINDOW macro is missing from libc-shim"
#endif

#ifndef TCP_AO_KEYF_EXCLUDE_OPT
#error "netinet/tcp.h:TCP_AO_KEYF_EXCLUDE_OPT macro is missing from libc-shim"
#endif

#ifndef TCP_AO_KEYF_IFINDEX
#error "netinet/tcp.h:TCP_AO_KEYF_IFINDEX macro is missing from libc-shim"
#endif

#ifndef TCP_AO_MAXKEYLEN
#error "netinet/tcp.h:TCP_AO_MAXKEYLEN macro is missing from libc-shim"
#endif

#ifndef TCP_CC_INFO
#error "netinet/tcp.h:TCP_CC_INFO macro is missing from libc-shim"
#endif

#ifndef TCP_CM_INQ
#error "netinet/tcp.h:TCP_CM_INQ macro is missing from libc-shim"
#endif

#ifndef TCP_CONGESTION
#error "netinet/tcp.h:TCP_CONGESTION macro is missing from libc-shim"
#endif

#ifndef TCP_COOKIE_IN_ALWAYS
#error "netinet/tcp.h:TCP_COOKIE_IN_ALWAYS macro is missing from libc-shim"
#endif

#ifndef TCP_COOKIE_MAX
#error "netinet/tcp.h:TCP_COOKIE_MAX macro is missing from libc-shim"
#endif

#ifndef TCP_COOKIE_MIN
#error "netinet/tcp.h:TCP_COOKIE_MIN macro is missing from libc-shim"
#endif

#ifndef TCP_COOKIE_OUT_NEVER
#error "netinet/tcp.h:TCP_COOKIE_OUT_NEVER macro is missing from libc-shim"
#endif

#ifndef TCP_COOKIE_PAIR_SIZE
#error "netinet/tcp.h:TCP_COOKIE_PAIR_SIZE macro is missing from libc-shim"
#endif

#ifndef TCP_COOKIE_TRANSACTIONS
#error "netinet/tcp.h:TCP_COOKIE_TRANSACTIONS macro is missing from libc-shim"
#endif

#ifndef TCP_CORK
#error "netinet/tcp.h:TCP_CORK macro is missing from libc-shim"
#endif

#ifndef TCP_DEFER_ACCEPT
#error "netinet/tcp.h:TCP_DEFER_ACCEPT macro is missing from libc-shim"
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

#ifndef TCP_LINGER2
#error "netinet/tcp.h:TCP_LINGER2 macro is missing from libc-shim"
#endif

#ifndef TCP_MAXSEG
#error "netinet/tcp.h:TCP_MAXSEG macro is missing from libc-shim"
#endif

#ifndef TCP_MAXWIN
#error "netinet/tcp.h:TCP_MAXWIN macro is missing from libc-shim"
#endif

#ifndef TCP_MAX_WINSHIFT
#error "netinet/tcp.h:TCP_MAX_WINSHIFT macro is missing from libc-shim"
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

#ifndef TCP_MSS
#error "netinet/tcp.h:TCP_MSS macro is missing from libc-shim"
#endif

#ifndef TCP_MSS_DEFAULT
#error "netinet/tcp.h:TCP_MSS_DEFAULT macro is missing from libc-shim"
#endif

#ifndef TCP_MSS_DESIRED
#error "netinet/tcp.h:TCP_MSS_DESIRED macro is missing from libc-shim"
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

#ifndef TCP_S_DATA_IN
#error "netinet/tcp.h:TCP_S_DATA_IN macro is missing from libc-shim"
#endif

#ifndef TCP_S_DATA_OUT
#error "netinet/tcp.h:TCP_S_DATA_OUT macro is missing from libc-shim"
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
