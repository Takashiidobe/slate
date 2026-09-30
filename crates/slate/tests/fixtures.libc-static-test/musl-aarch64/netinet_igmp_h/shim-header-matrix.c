#include <netinet/igmp.h>

_Static_assert(__builtin_offsetof(struct igmp, igmp_type) == 0, "struct igmp.igmp_type offset differs from oracle");

typedef unsigned char slate_oracle_struct_igmp_igmp_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct igmp *)0)->igmp_type), slate_oracle_struct_igmp_igmp_type), "struct igmp.igmp_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct igmp, igmp_code) == 1, "struct igmp.igmp_code offset differs from oracle");

typedef unsigned char slate_oracle_struct_igmp_igmp_code;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct igmp *)0)->igmp_code), slate_oracle_struct_igmp_igmp_code), "struct igmp.igmp_code field type differs from oracle");

_Static_assert(__builtin_offsetof(struct igmp, igmp_cksum) == 2, "struct igmp.igmp_cksum offset differs from oracle");

typedef unsigned short slate_oracle_struct_igmp_igmp_cksum;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct igmp *)0)->igmp_cksum), slate_oracle_struct_igmp_igmp_cksum), "struct igmp.igmp_cksum field type differs from oracle");

typedef struct in_addr slate_oracle_struct_igmp_igmp_group;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct igmp *)0)->igmp_group), slate_oracle_struct_igmp_igmp_group), "struct igmp.igmp_group field type differs from oracle");

#ifndef IGMP_AWAKENING_MEMBER
#error "netinet/igmp.h:IGMP_AWAKENING_MEMBER macro is missing from libc-shim"
#endif

#ifndef IGMP_DELAYING_MEMBER
#error "netinet/igmp.h:IGMP_DELAYING_MEMBER macro is missing from libc-shim"
#endif

#ifndef IGMP_DVMRP
#error "netinet/igmp.h:IGMP_DVMRP macro is missing from libc-shim"
#endif

#ifndef IGMP_HOST_LEAVE_MESSAGE
#error "netinet/igmp.h:IGMP_HOST_LEAVE_MESSAGE macro is missing from libc-shim"
#endif

#ifndef IGMP_HOST_MEMBERSHIP_QUERY
#error "netinet/igmp.h:IGMP_HOST_MEMBERSHIP_QUERY macro is missing from libc-shim"
#endif

#ifndef IGMP_HOST_MEMBERSHIP_REPORT
#error "netinet/igmp.h:IGMP_HOST_MEMBERSHIP_REPORT macro is missing from libc-shim"
#endif

#ifndef IGMP_HOST_NEW_MEMBERSHIP_REPORT
#error "netinet/igmp.h:IGMP_HOST_NEW_MEMBERSHIP_REPORT macro is missing from libc-shim"
#endif

#ifndef IGMP_IDLE_MEMBER
#error "netinet/igmp.h:IGMP_IDLE_MEMBER macro is missing from libc-shim"
#endif

#ifndef IGMP_LAZY_MEMBER
#error "netinet/igmp.h:IGMP_LAZY_MEMBER macro is missing from libc-shim"
#endif

#ifndef IGMP_MAX_HOST_REPORT_DELAY
#error "netinet/igmp.h:IGMP_MAX_HOST_REPORT_DELAY macro is missing from libc-shim"
#endif

#ifndef IGMP_MEMBERSHIP_QUERY
#error "netinet/igmp.h:IGMP_MEMBERSHIP_QUERY macro is missing from libc-shim"
#endif

#ifndef IGMP_MINLEN
#error "netinet/igmp.h:IGMP_MINLEN macro is missing from libc-shim"
#endif

#ifndef IGMP_MTRACE
#error "netinet/igmp.h:IGMP_MTRACE macro is missing from libc-shim"
#endif

#ifndef IGMP_MTRACE_RESP
#error "netinet/igmp.h:IGMP_MTRACE_RESP macro is missing from libc-shim"
#endif

#ifndef IGMP_PIM
#error "netinet/igmp.h:IGMP_PIM macro is missing from libc-shim"
#endif

#ifndef IGMP_SLEEPING_MEMBER
#error "netinet/igmp.h:IGMP_SLEEPING_MEMBER macro is missing from libc-shim"
#endif

#ifndef IGMP_TIMER_SCALE
#error "netinet/igmp.h:IGMP_TIMER_SCALE macro is missing from libc-shim"
#endif

#ifndef IGMP_TRACE
#error "netinet/igmp.h:IGMP_TRACE macro is missing from libc-shim"
#endif

#ifndef IGMP_V1_MEMBERSHIP_REPORT
#error "netinet/igmp.h:IGMP_V1_MEMBERSHIP_REPORT macro is missing from libc-shim"
#endif

#ifndef IGMP_V2_LEAVE_GROUP
#error "netinet/igmp.h:IGMP_V2_LEAVE_GROUP macro is missing from libc-shim"
#endif

#ifndef IGMP_V2_MEMBERSHIP_REPORT
#error "netinet/igmp.h:IGMP_V2_MEMBERSHIP_REPORT macro is missing from libc-shim"
#endif

#ifndef IGMP_v1_ROUTER
#error "netinet/igmp.h:IGMP_v1_ROUTER macro is missing from libc-shim"
#endif

#ifndef IGMP_v2_ROUTER
#error "netinet/igmp.h:IGMP_v2_ROUTER macro is missing from libc-shim"
#endif

int main(void) { return 0; }
