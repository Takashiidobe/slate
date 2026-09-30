#include <netinet/icmp6.h>

_Static_assert(sizeof(struct icmp6_filter) == 32, "struct icmp6_filter size differs from oracle");

_Static_assert(_Alignof(struct icmp6_filter) == 4, "struct icmp6_filter alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct icmp6_filter, icmp6_filt) == 0, "struct icmp6_filter.icmp6_filt offset differs from oracle");

#ifndef ICMP6_DST_UNREACH
#error "netinet/icmp6.h:ICMP6_DST_UNREACH macro is missing from libc-shim"
#endif

#ifndef ICMP6_DST_UNREACH_ADDR
#error "netinet/icmp6.h:ICMP6_DST_UNREACH_ADDR macro is missing from libc-shim"
#endif

#ifndef ICMP6_DST_UNREACH_ADMIN
#error "netinet/icmp6.h:ICMP6_DST_UNREACH_ADMIN macro is missing from libc-shim"
#endif

#ifndef ICMP6_DST_UNREACH_BEYONDSCOPE
#error "netinet/icmp6.h:ICMP6_DST_UNREACH_BEYONDSCOPE macro is missing from libc-shim"
#endif

#ifndef ICMP6_DST_UNREACH_NOPORT
#error "netinet/icmp6.h:ICMP6_DST_UNREACH_NOPORT macro is missing from libc-shim"
#endif

#ifndef ICMP6_DST_UNREACH_NOROUTE
#error "netinet/icmp6.h:ICMP6_DST_UNREACH_NOROUTE macro is missing from libc-shim"
#endif

#ifndef ICMP6_ECHO_REPLY
#error "netinet/icmp6.h:ICMP6_ECHO_REPLY macro is missing from libc-shim"
#endif

#ifndef ICMP6_ECHO_REQUEST
#error "netinet/icmp6.h:ICMP6_ECHO_REQUEST macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER
#error "netinet/icmp6.h:ICMP6_FILTER macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_BLOCK
#error "netinet/icmp6.h:ICMP6_FILTER_BLOCK macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_BLOCKOTHERS
#error "netinet/icmp6.h:ICMP6_FILTER_BLOCKOTHERS macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_PASS
#error "netinet/icmp6.h:ICMP6_FILTER_PASS macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_PASSONLY
#error "netinet/icmp6.h:ICMP6_FILTER_PASSONLY macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_SETBLOCK
#error "netinet/icmp6.h:ICMP6_FILTER_SETBLOCK macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_SETBLOCKALL
#error "netinet/icmp6.h:ICMP6_FILTER_SETBLOCKALL macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_SETPASS
#error "netinet/icmp6.h:ICMP6_FILTER_SETPASS macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_SETPASSALL
#error "netinet/icmp6.h:ICMP6_FILTER_SETPASSALL macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_WILLBLOCK
#error "netinet/icmp6.h:ICMP6_FILTER_WILLBLOCK macro is missing from libc-shim"
#endif

#ifndef ICMP6_FILTER_WILLPASS
#error "netinet/icmp6.h:ICMP6_FILTER_WILLPASS macro is missing from libc-shim"
#endif

#ifndef ICMP6_INFOMSG_MASK
#error "netinet/icmp6.h:ICMP6_INFOMSG_MASK macro is missing from libc-shim"
#endif

#ifndef ICMP6_PACKET_TOO_BIG
#error "netinet/icmp6.h:ICMP6_PACKET_TOO_BIG macro is missing from libc-shim"
#endif

#ifndef ICMP6_PARAMPROB_HEADER
#error "netinet/icmp6.h:ICMP6_PARAMPROB_HEADER macro is missing from libc-shim"
#endif

#ifndef ICMP6_PARAMPROB_NEXTHEADER
#error "netinet/icmp6.h:ICMP6_PARAMPROB_NEXTHEADER macro is missing from libc-shim"
#endif

#ifndef ICMP6_PARAMPROB_OPTION
#error "netinet/icmp6.h:ICMP6_PARAMPROB_OPTION macro is missing from libc-shim"
#endif

#ifndef ICMP6_PARAM_PROB
#error "netinet/icmp6.h:ICMP6_PARAM_PROB macro is missing from libc-shim"
#endif

#ifndef ICMP6_ROUTER_RENUMBERING
#error "netinet/icmp6.h:ICMP6_ROUTER_RENUMBERING macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_FLAGS_FORCEAPPLY
#error "netinet/icmp6.h:ICMP6_RR_FLAGS_FORCEAPPLY macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_FLAGS_PREVDONE
#error "netinet/icmp6.h:ICMP6_RR_FLAGS_PREVDONE macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_FLAGS_REQRESULT
#error "netinet/icmp6.h:ICMP6_RR_FLAGS_REQRESULT macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_FLAGS_SPECSITE
#error "netinet/icmp6.h:ICMP6_RR_FLAGS_SPECSITE macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_FLAGS_TEST
#error "netinet/icmp6.h:ICMP6_RR_FLAGS_TEST macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_PCOUSE_FLAGS_DECRPLTIME
#error "netinet/icmp6.h:ICMP6_RR_PCOUSE_FLAGS_DECRPLTIME macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_PCOUSE_FLAGS_DECRVLTIME
#error "netinet/icmp6.h:ICMP6_RR_PCOUSE_FLAGS_DECRVLTIME macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_PCOUSE_RAFLAGS_AUTO
#error "netinet/icmp6.h:ICMP6_RR_PCOUSE_RAFLAGS_AUTO macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_PCOUSE_RAFLAGS_ONLINK
#error "netinet/icmp6.h:ICMP6_RR_PCOUSE_RAFLAGS_ONLINK macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_RESULT_FLAGS_FORBIDDEN
#error "netinet/icmp6.h:ICMP6_RR_RESULT_FLAGS_FORBIDDEN macro is missing from libc-shim"
#endif

#ifndef ICMP6_RR_RESULT_FLAGS_OOB
#error "netinet/icmp6.h:ICMP6_RR_RESULT_FLAGS_OOB macro is missing from libc-shim"
#endif

#ifndef ICMP6_TIME_EXCEEDED
#error "netinet/icmp6.h:ICMP6_TIME_EXCEEDED macro is missing from libc-shim"
#endif

#ifndef ICMP6_TIME_EXCEED_REASSEMBLY
#error "netinet/icmp6.h:ICMP6_TIME_EXCEED_REASSEMBLY macro is missing from libc-shim"
#endif

#ifndef ICMP6_TIME_EXCEED_TRANSIT
#error "netinet/icmp6.h:ICMP6_TIME_EXCEED_TRANSIT macro is missing from libc-shim"
#endif

#ifndef MLD_LISTENER_QUERY
#error "netinet/icmp6.h:MLD_LISTENER_QUERY macro is missing from libc-shim"
#endif

#ifndef MLD_LISTENER_REDUCTION
#error "netinet/icmp6.h:MLD_LISTENER_REDUCTION macro is missing from libc-shim"
#endif

#ifndef MLD_LISTENER_REPORT
#error "netinet/icmp6.h:MLD_LISTENER_REPORT macro is missing from libc-shim"
#endif

#ifndef ND_NA_FLAG_OVERRIDE
#error "netinet/icmp6.h:ND_NA_FLAG_OVERRIDE macro is missing from libc-shim"
#endif

#ifndef ND_NA_FLAG_ROUTER
#error "netinet/icmp6.h:ND_NA_FLAG_ROUTER macro is missing from libc-shim"
#endif

#ifndef ND_NA_FLAG_SOLICITED
#error "netinet/icmp6.h:ND_NA_FLAG_SOLICITED macro is missing from libc-shim"
#endif

#ifndef ND_NEIGHBOR_ADVERT
#error "netinet/icmp6.h:ND_NEIGHBOR_ADVERT macro is missing from libc-shim"
#endif

#ifndef ND_NEIGHBOR_SOLICIT
#error "netinet/icmp6.h:ND_NEIGHBOR_SOLICIT macro is missing from libc-shim"
#endif

#ifndef ND_OPT_HOME_AGENT_INFO
#error "netinet/icmp6.h:ND_OPT_HOME_AGENT_INFO macro is missing from libc-shim"
#endif

#ifndef ND_OPT_MTU
#error "netinet/icmp6.h:ND_OPT_MTU macro is missing from libc-shim"
#endif

#ifndef ND_OPT_PI_FLAG_AUTO
#error "netinet/icmp6.h:ND_OPT_PI_FLAG_AUTO macro is missing from libc-shim"
#endif

#ifndef ND_OPT_PI_FLAG_ONLINK
#error "netinet/icmp6.h:ND_OPT_PI_FLAG_ONLINK macro is missing from libc-shim"
#endif

#ifndef ND_OPT_PI_FLAG_RADDR
#error "netinet/icmp6.h:ND_OPT_PI_FLAG_RADDR macro is missing from libc-shim"
#endif

#ifndef ND_OPT_PREFIX_INFORMATION
#error "netinet/icmp6.h:ND_OPT_PREFIX_INFORMATION macro is missing from libc-shim"
#endif

#ifndef ND_OPT_REDIRECTED_HEADER
#error "netinet/icmp6.h:ND_OPT_REDIRECTED_HEADER macro is missing from libc-shim"
#endif

#ifndef ND_OPT_RTR_ADV_INTERVAL
#error "netinet/icmp6.h:ND_OPT_RTR_ADV_INTERVAL macro is missing from libc-shim"
#endif

#ifndef ND_OPT_SOURCE_LINKADDR
#error "netinet/icmp6.h:ND_OPT_SOURCE_LINKADDR macro is missing from libc-shim"
#endif

#ifndef ND_OPT_TARGET_LINKADDR
#error "netinet/icmp6.h:ND_OPT_TARGET_LINKADDR macro is missing from libc-shim"
#endif

#ifndef ND_RA_FLAG_HOME_AGENT
#error "netinet/icmp6.h:ND_RA_FLAG_HOME_AGENT macro is missing from libc-shim"
#endif

#ifndef ND_RA_FLAG_MANAGED
#error "netinet/icmp6.h:ND_RA_FLAG_MANAGED macro is missing from libc-shim"
#endif

#ifndef ND_RA_FLAG_OTHER
#error "netinet/icmp6.h:ND_RA_FLAG_OTHER macro is missing from libc-shim"
#endif

#ifndef ND_REDIRECT
#error "netinet/icmp6.h:ND_REDIRECT macro is missing from libc-shim"
#endif

#ifndef ND_ROUTER_ADVERT
#error "netinet/icmp6.h:ND_ROUTER_ADVERT macro is missing from libc-shim"
#endif

#ifndef ND_ROUTER_SOLICIT
#error "netinet/icmp6.h:ND_ROUTER_SOLICIT macro is missing from libc-shim"
#endif

#ifndef RPM_PCO_ADD
#error "netinet/icmp6.h:RPM_PCO_ADD macro is missing from libc-shim"
#endif

#ifndef RPM_PCO_CHANGE
#error "netinet/icmp6.h:RPM_PCO_CHANGE macro is missing from libc-shim"
#endif

#ifndef RPM_PCO_SETGLOBAL
#error "netinet/icmp6.h:RPM_PCO_SETGLOBAL macro is missing from libc-shim"
#endif

#ifndef icmp6_data16
#error "netinet/icmp6.h:icmp6_data16 macro is missing from libc-shim"
#endif

#ifndef icmp6_data32
#error "netinet/icmp6.h:icmp6_data32 macro is missing from libc-shim"
#endif

#ifndef icmp6_data8
#error "netinet/icmp6.h:icmp6_data8 macro is missing from libc-shim"
#endif

#ifndef icmp6_id
#error "netinet/icmp6.h:icmp6_id macro is missing from libc-shim"
#endif

#ifndef icmp6_maxdelay
#error "netinet/icmp6.h:icmp6_maxdelay macro is missing from libc-shim"
#endif

#ifndef icmp6_mtu
#error "netinet/icmp6.h:icmp6_mtu macro is missing from libc-shim"
#endif

#ifndef icmp6_pptr
#error "netinet/icmp6.h:icmp6_pptr macro is missing from libc-shim"
#endif

#ifndef icmp6_seq
#error "netinet/icmp6.h:icmp6_seq macro is missing from libc-shim"
#endif

#ifndef mld_cksum
#error "netinet/icmp6.h:mld_cksum macro is missing from libc-shim"
#endif

#ifndef mld_code
#error "netinet/icmp6.h:mld_code macro is missing from libc-shim"
#endif

#ifndef mld_maxdelay
#error "netinet/icmp6.h:mld_maxdelay macro is missing from libc-shim"
#endif

#ifndef mld_reserved
#error "netinet/icmp6.h:mld_reserved macro is missing from libc-shim"
#endif

#ifndef mld_type
#error "netinet/icmp6.h:mld_type macro is missing from libc-shim"
#endif

#ifndef nd_na_cksum
#error "netinet/icmp6.h:nd_na_cksum macro is missing from libc-shim"
#endif

#ifndef nd_na_code
#error "netinet/icmp6.h:nd_na_code macro is missing from libc-shim"
#endif

#ifndef nd_na_flags_reserved
#error "netinet/icmp6.h:nd_na_flags_reserved macro is missing from libc-shim"
#endif

#ifndef nd_na_type
#error "netinet/icmp6.h:nd_na_type macro is missing from libc-shim"
#endif

#ifndef nd_ns_cksum
#error "netinet/icmp6.h:nd_ns_cksum macro is missing from libc-shim"
#endif

#ifndef nd_ns_code
#error "netinet/icmp6.h:nd_ns_code macro is missing from libc-shim"
#endif

#ifndef nd_ns_reserved
#error "netinet/icmp6.h:nd_ns_reserved macro is missing from libc-shim"
#endif

#ifndef nd_ns_type
#error "netinet/icmp6.h:nd_ns_type macro is missing from libc-shim"
#endif

#ifndef nd_ra_cksum
#error "netinet/icmp6.h:nd_ra_cksum macro is missing from libc-shim"
#endif

#ifndef nd_ra_code
#error "netinet/icmp6.h:nd_ra_code macro is missing from libc-shim"
#endif

#ifndef nd_ra_curhoplimit
#error "netinet/icmp6.h:nd_ra_curhoplimit macro is missing from libc-shim"
#endif

#ifndef nd_ra_flags_reserved
#error "netinet/icmp6.h:nd_ra_flags_reserved macro is missing from libc-shim"
#endif

#ifndef nd_ra_router_lifetime
#error "netinet/icmp6.h:nd_ra_router_lifetime macro is missing from libc-shim"
#endif

#ifndef nd_ra_type
#error "netinet/icmp6.h:nd_ra_type macro is missing from libc-shim"
#endif

#ifndef nd_rd_cksum
#error "netinet/icmp6.h:nd_rd_cksum macro is missing from libc-shim"
#endif

#ifndef nd_rd_code
#error "netinet/icmp6.h:nd_rd_code macro is missing from libc-shim"
#endif

#ifndef nd_rd_reserved
#error "netinet/icmp6.h:nd_rd_reserved macro is missing from libc-shim"
#endif

#ifndef nd_rd_type
#error "netinet/icmp6.h:nd_rd_type macro is missing from libc-shim"
#endif

#ifndef nd_rs_cksum
#error "netinet/icmp6.h:nd_rs_cksum macro is missing from libc-shim"
#endif

#ifndef nd_rs_code
#error "netinet/icmp6.h:nd_rs_code macro is missing from libc-shim"
#endif

#ifndef nd_rs_reserved
#error "netinet/icmp6.h:nd_rs_reserved macro is missing from libc-shim"
#endif

#ifndef nd_rs_type
#error "netinet/icmp6.h:nd_rs_type macro is missing from libc-shim"
#endif

#ifndef rr_cksum
#error "netinet/icmp6.h:rr_cksum macro is missing from libc-shim"
#endif

#ifndef rr_code
#error "netinet/icmp6.h:rr_code macro is missing from libc-shim"
#endif

#ifndef rr_seqnum
#error "netinet/icmp6.h:rr_seqnum macro is missing from libc-shim"
#endif

#ifndef rr_type
#error "netinet/icmp6.h:rr_type macro is missing from libc-shim"
#endif

int main(void) { return 0; }
