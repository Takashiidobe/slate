#include <netinet/ip_icmp.h>

_Static_assert(__builtin_offsetof(struct icmphdr, type) == 0, "struct icmphdr.type offset differs from oracle");

typedef unsigned char slate_oracle_struct_icmphdr_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct icmphdr *)0)->type), slate_oracle_struct_icmphdr_type), "struct icmphdr.type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct icmphdr, code) == 1, "struct icmphdr.code offset differs from oracle");

typedef unsigned char slate_oracle_struct_icmphdr_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct icmphdr *)0)->code), slate_oracle_struct_icmphdr_code), "struct icmphdr.code field type differs from oracle");

_Static_assert(__builtin_offsetof(struct icmphdr, checksum) == 2, "struct icmphdr.checksum offset differs from oracle");

typedef unsigned short slate_oracle_struct_icmphdr_checksum;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct icmphdr *)0)->checksum), slate_oracle_struct_icmphdr_checksum), "struct icmphdr.checksum field type differs from oracle");

#ifndef ICMP_ADDRESS
#error "netinet/ip_icmp.h:ICMP_ADDRESS macro is missing from libc-shim"
#endif

#ifndef ICMP_ADDRESSREPLY
#error "netinet/ip_icmp.h:ICMP_ADDRESSREPLY macro is missing from libc-shim"
#endif

#ifndef ICMP_ADVLEN
#error "netinet/ip_icmp.h:ICMP_ADVLEN macro is missing from libc-shim"
#endif

#ifndef ICMP_ADVLENMIN
#error "netinet/ip_icmp.h:ICMP_ADVLENMIN macro is missing from libc-shim"
#endif

#ifndef ICMP_DEST_UNREACH
#error "netinet/ip_icmp.h:ICMP_DEST_UNREACH macro is missing from libc-shim"
#endif

#ifndef ICMP_ECHO
#error "netinet/ip_icmp.h:ICMP_ECHO macro is missing from libc-shim"
#endif

#ifndef ICMP_ECHOREPLY
#error "netinet/ip_icmp.h:ICMP_ECHOREPLY macro is missing from libc-shim"
#endif

#ifndef ICMP_EXC_FRAGTIME
#error "netinet/ip_icmp.h:ICMP_EXC_FRAGTIME macro is missing from libc-shim"
#endif

#ifndef ICMP_EXC_TTL
#error "netinet/ip_icmp.h:ICMP_EXC_TTL macro is missing from libc-shim"
#endif

#ifndef ICMP_FRAG_NEEDED
#error "netinet/ip_icmp.h:ICMP_FRAG_NEEDED macro is missing from libc-shim"
#endif

#ifndef ICMP_HOST_ANO
#error "netinet/ip_icmp.h:ICMP_HOST_ANO macro is missing from libc-shim"
#endif

#ifndef ICMP_HOST_ISOLATED
#error "netinet/ip_icmp.h:ICMP_HOST_ISOLATED macro is missing from libc-shim"
#endif

#ifndef ICMP_HOST_UNKNOWN
#error "netinet/ip_icmp.h:ICMP_HOST_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef ICMP_HOST_UNREACH
#error "netinet/ip_icmp.h:ICMP_HOST_UNREACH macro is missing from libc-shim"
#endif

#ifndef ICMP_HOST_UNR_TOS
#error "netinet/ip_icmp.h:ICMP_HOST_UNR_TOS macro is missing from libc-shim"
#endif

#ifndef ICMP_INFOTYPE
#error "netinet/ip_icmp.h:ICMP_INFOTYPE macro is missing from libc-shim"
#endif

#ifndef ICMP_INFO_REPLY
#error "netinet/ip_icmp.h:ICMP_INFO_REPLY macro is missing from libc-shim"
#endif

#ifndef ICMP_INFO_REQUEST
#error "netinet/ip_icmp.h:ICMP_INFO_REQUEST macro is missing from libc-shim"
#endif

#ifndef ICMP_IREQ
#error "netinet/ip_icmp.h:ICMP_IREQ macro is missing from libc-shim"
#endif

#ifndef ICMP_IREQREPLY
#error "netinet/ip_icmp.h:ICMP_IREQREPLY macro is missing from libc-shim"
#endif

#ifndef ICMP_MASKLEN
#error "netinet/ip_icmp.h:ICMP_MASKLEN macro is missing from libc-shim"
#endif

#ifndef ICMP_MASKREPLY
#error "netinet/ip_icmp.h:ICMP_MASKREPLY macro is missing from libc-shim"
#endif

#ifndef ICMP_MASKREQ
#error "netinet/ip_icmp.h:ICMP_MASKREQ macro is missing from libc-shim"
#endif

#ifndef ICMP_MAXTYPE
#error "netinet/ip_icmp.h:ICMP_MAXTYPE macro is missing from libc-shim"
#endif

#ifndef ICMP_MINLEN
#error "netinet/ip_icmp.h:ICMP_MINLEN macro is missing from libc-shim"
#endif

#ifndef ICMP_NET_ANO
#error "netinet/ip_icmp.h:ICMP_NET_ANO macro is missing from libc-shim"
#endif

#ifndef ICMP_NET_UNKNOWN
#error "netinet/ip_icmp.h:ICMP_NET_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef ICMP_NET_UNREACH
#error "netinet/ip_icmp.h:ICMP_NET_UNREACH macro is missing from libc-shim"
#endif

#ifndef ICMP_NET_UNR_TOS
#error "netinet/ip_icmp.h:ICMP_NET_UNR_TOS macro is missing from libc-shim"
#endif

#ifndef ICMP_PARAMETERPROB
#error "netinet/ip_icmp.h:ICMP_PARAMETERPROB macro is missing from libc-shim"
#endif

#ifndef ICMP_PARAMPROB
#error "netinet/ip_icmp.h:ICMP_PARAMPROB macro is missing from libc-shim"
#endif

#ifndef ICMP_PARAMPROB_OPTABSENT
#error "netinet/ip_icmp.h:ICMP_PARAMPROB_OPTABSENT macro is missing from libc-shim"
#endif

#ifndef ICMP_PKT_FILTERED
#error "netinet/ip_icmp.h:ICMP_PKT_FILTERED macro is missing from libc-shim"
#endif

#ifndef ICMP_PORT_UNREACH
#error "netinet/ip_icmp.h:ICMP_PORT_UNREACH macro is missing from libc-shim"
#endif

#ifndef ICMP_PREC_CUTOFF
#error "netinet/ip_icmp.h:ICMP_PREC_CUTOFF macro is missing from libc-shim"
#endif

#ifndef ICMP_PREC_VIOLATION
#error "netinet/ip_icmp.h:ICMP_PREC_VIOLATION macro is missing from libc-shim"
#endif

#ifndef ICMP_PROT_UNREACH
#error "netinet/ip_icmp.h:ICMP_PROT_UNREACH macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIRECT
#error "netinet/ip_icmp.h:ICMP_REDIRECT macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIRECT_HOST
#error "netinet/ip_icmp.h:ICMP_REDIRECT_HOST macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIRECT_NET
#error "netinet/ip_icmp.h:ICMP_REDIRECT_NET macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIRECT_TOSHOST
#error "netinet/ip_icmp.h:ICMP_REDIRECT_TOSHOST macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIRECT_TOSNET
#error "netinet/ip_icmp.h:ICMP_REDIRECT_TOSNET macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIR_HOST
#error "netinet/ip_icmp.h:ICMP_REDIR_HOST macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIR_HOSTTOS
#error "netinet/ip_icmp.h:ICMP_REDIR_HOSTTOS macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIR_NET
#error "netinet/ip_icmp.h:ICMP_REDIR_NET macro is missing from libc-shim"
#endif

#ifndef ICMP_REDIR_NETTOS
#error "netinet/ip_icmp.h:ICMP_REDIR_NETTOS macro is missing from libc-shim"
#endif

#ifndef ICMP_ROUTERADVERT
#error "netinet/ip_icmp.h:ICMP_ROUTERADVERT macro is missing from libc-shim"
#endif

#ifndef ICMP_ROUTERSOLICIT
#error "netinet/ip_icmp.h:ICMP_ROUTERSOLICIT macro is missing from libc-shim"
#endif

#ifndef ICMP_SOURCEQUENCH
#error "netinet/ip_icmp.h:ICMP_SOURCEQUENCH macro is missing from libc-shim"
#endif

#ifndef ICMP_SOURCE_QUENCH
#error "netinet/ip_icmp.h:ICMP_SOURCE_QUENCH macro is missing from libc-shim"
#endif

#ifndef ICMP_SR_FAILED
#error "netinet/ip_icmp.h:ICMP_SR_FAILED macro is missing from libc-shim"
#endif

#ifndef ICMP_TIMESTAMP
#error "netinet/ip_icmp.h:ICMP_TIMESTAMP macro is missing from libc-shim"
#endif

#ifndef ICMP_TIMESTAMPREPLY
#error "netinet/ip_icmp.h:ICMP_TIMESTAMPREPLY macro is missing from libc-shim"
#endif

#ifndef ICMP_TIME_EXCEEDED
#error "netinet/ip_icmp.h:ICMP_TIME_EXCEEDED macro is missing from libc-shim"
#endif

#ifndef ICMP_TIMXCEED
#error "netinet/ip_icmp.h:ICMP_TIMXCEED macro is missing from libc-shim"
#endif

#ifndef ICMP_TIMXCEED_INTRANS
#error "netinet/ip_icmp.h:ICMP_TIMXCEED_INTRANS macro is missing from libc-shim"
#endif

#ifndef ICMP_TIMXCEED_REASS
#error "netinet/ip_icmp.h:ICMP_TIMXCEED_REASS macro is missing from libc-shim"
#endif

#ifndef ICMP_TSLEN
#error "netinet/ip_icmp.h:ICMP_TSLEN macro is missing from libc-shim"
#endif

#ifndef ICMP_TSTAMP
#error "netinet/ip_icmp.h:ICMP_TSTAMP macro is missing from libc-shim"
#endif

#ifndef ICMP_TSTAMPREPLY
#error "netinet/ip_icmp.h:ICMP_TSTAMPREPLY macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH
#error "netinet/ip_icmp.h:ICMP_UNREACH macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_FILTER_PROHIB
#error "netinet/ip_icmp.h:ICMP_UNREACH_FILTER_PROHIB macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_HOST
#error "netinet/ip_icmp.h:ICMP_UNREACH_HOST macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_HOST_PRECEDENCE
#error "netinet/ip_icmp.h:ICMP_UNREACH_HOST_PRECEDENCE macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_HOST_PROHIB
#error "netinet/ip_icmp.h:ICMP_UNREACH_HOST_PROHIB macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_HOST_UNKNOWN
#error "netinet/ip_icmp.h:ICMP_UNREACH_HOST_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_ISOLATED
#error "netinet/ip_icmp.h:ICMP_UNREACH_ISOLATED macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_NEEDFRAG
#error "netinet/ip_icmp.h:ICMP_UNREACH_NEEDFRAG macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_NET
#error "netinet/ip_icmp.h:ICMP_UNREACH_NET macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_NET_PROHIB
#error "netinet/ip_icmp.h:ICMP_UNREACH_NET_PROHIB macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_NET_UNKNOWN
#error "netinet/ip_icmp.h:ICMP_UNREACH_NET_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_PORT
#error "netinet/ip_icmp.h:ICMP_UNREACH_PORT macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_PRECEDENCE_CUTOFF
#error "netinet/ip_icmp.h:ICMP_UNREACH_PRECEDENCE_CUTOFF macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_PROTOCOL
#error "netinet/ip_icmp.h:ICMP_UNREACH_PROTOCOL macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_SRCFAIL
#error "netinet/ip_icmp.h:ICMP_UNREACH_SRCFAIL macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_TOSHOST
#error "netinet/ip_icmp.h:ICMP_UNREACH_TOSHOST macro is missing from libc-shim"
#endif

#ifndef ICMP_UNREACH_TOSNET
#error "netinet/ip_icmp.h:ICMP_UNREACH_TOSNET macro is missing from libc-shim"
#endif

#ifndef NR_ICMP_TYPES
#error "netinet/ip_icmp.h:NR_ICMP_TYPES macro is missing from libc-shim"
#endif

#ifndef NR_ICMP_UNREACH
#error "netinet/ip_icmp.h:NR_ICMP_UNREACH macro is missing from libc-shim"
#endif

#ifndef icmp_data
#error "netinet/ip_icmp.h:icmp_data macro is missing from libc-shim"
#endif

#ifndef icmp_gwaddr
#error "netinet/ip_icmp.h:icmp_gwaddr macro is missing from libc-shim"
#endif

#ifndef icmp_id
#error "netinet/ip_icmp.h:icmp_id macro is missing from libc-shim"
#endif

#ifndef icmp_ip
#error "netinet/ip_icmp.h:icmp_ip macro is missing from libc-shim"
#endif

#ifndef icmp_lifetime
#error "netinet/ip_icmp.h:icmp_lifetime macro is missing from libc-shim"
#endif

#ifndef icmp_mask
#error "netinet/ip_icmp.h:icmp_mask macro is missing from libc-shim"
#endif

#ifndef icmp_nextmtu
#error "netinet/ip_icmp.h:icmp_nextmtu macro is missing from libc-shim"
#endif

#ifndef icmp_num_addrs
#error "netinet/ip_icmp.h:icmp_num_addrs macro is missing from libc-shim"
#endif

#ifndef icmp_otime
#error "netinet/ip_icmp.h:icmp_otime macro is missing from libc-shim"
#endif

#ifndef icmp_pmvoid
#error "netinet/ip_icmp.h:icmp_pmvoid macro is missing from libc-shim"
#endif

#ifndef icmp_pptr
#error "netinet/ip_icmp.h:icmp_pptr macro is missing from libc-shim"
#endif

#ifndef icmp_radv
#error "netinet/ip_icmp.h:icmp_radv macro is missing from libc-shim"
#endif

#ifndef icmp_rtime
#error "netinet/ip_icmp.h:icmp_rtime macro is missing from libc-shim"
#endif

#ifndef icmp_seq
#error "netinet/ip_icmp.h:icmp_seq macro is missing from libc-shim"
#endif

#ifndef icmp_ttime
#error "netinet/ip_icmp.h:icmp_ttime macro is missing from libc-shim"
#endif

#ifndef icmp_void
#error "netinet/ip_icmp.h:icmp_void macro is missing from libc-shim"
#endif

#ifndef icmp_wpa
#error "netinet/ip_icmp.h:icmp_wpa macro is missing from libc-shim"
#endif

int main(void) { return 0; }
