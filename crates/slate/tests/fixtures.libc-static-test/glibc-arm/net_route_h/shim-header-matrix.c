#include <net/route.h>

_Static_assert(__builtin_offsetof(struct rtentry, rt_pad1) == 0, "struct rtentry.rt_pad1 offset differs from oracle");

typedef unsigned long slate_oracle_struct_rtentry_rt_pad1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_pad1), slate_oracle_struct_rtentry_rt_pad1), "struct rtentry.rt_pad1 field type differs from oracle");

typedef struct sockaddr slate_oracle_struct_rtentry_rt_dst;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_dst), slate_oracle_struct_rtentry_rt_dst), "struct rtentry.rt_dst field type differs from oracle");

typedef struct sockaddr slate_oracle_struct_rtentry_rt_gateway;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_gateway), slate_oracle_struct_rtentry_rt_gateway), "struct rtentry.rt_gateway field type differs from oracle");

typedef struct sockaddr slate_oracle_struct_rtentry_rt_genmask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_genmask), slate_oracle_struct_rtentry_rt_genmask), "struct rtentry.rt_genmask field type differs from oracle");

typedef unsigned short slate_oracle_struct_rtentry_rt_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_flags), slate_oracle_struct_rtentry_rt_flags), "struct rtentry.rt_flags field type differs from oracle");

typedef short slate_oracle_struct_rtentry_rt_pad2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_pad2), slate_oracle_struct_rtentry_rt_pad2), "struct rtentry.rt_pad2 field type differs from oracle");

typedef unsigned long slate_oracle_struct_rtentry_rt_pad3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_pad3), slate_oracle_struct_rtentry_rt_pad3), "struct rtentry.rt_pad3 field type differs from oracle");

typedef unsigned char slate_oracle_struct_rtentry_rt_tos;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_tos), slate_oracle_struct_rtentry_rt_tos), "struct rtentry.rt_tos field type differs from oracle");

typedef unsigned char slate_oracle_struct_rtentry_rt_class;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_class), slate_oracle_struct_rtentry_rt_class), "struct rtentry.rt_class field type differs from oracle");

typedef short slate_oracle_struct_rtentry_rt_pad4;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_pad4), slate_oracle_struct_rtentry_rt_pad4), "struct rtentry.rt_pad4 field type differs from oracle");

typedef short slate_oracle_struct_rtentry_rt_metric;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_metric), slate_oracle_struct_rtentry_rt_metric), "struct rtentry.rt_metric field type differs from oracle");

typedef char * slate_oracle_struct_rtentry_rt_dev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_dev), slate_oracle_struct_rtentry_rt_dev), "struct rtentry.rt_dev field type differs from oracle");

typedef unsigned long slate_oracle_struct_rtentry_rt_mtu;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_mtu), slate_oracle_struct_rtentry_rt_mtu), "struct rtentry.rt_mtu field type differs from oracle");

typedef unsigned long slate_oracle_struct_rtentry_rt_window;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_window), slate_oracle_struct_rtentry_rt_window), "struct rtentry.rt_window field type differs from oracle");

typedef unsigned short slate_oracle_struct_rtentry_rt_irtt;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rtentry *)0)->rt_irtt), slate_oracle_struct_rtentry_rt_irtt), "struct rtentry.rt_irtt field type differs from oracle");

#ifndef RTCF_DIRECTSRC
#error "net/route.h:RTCF_DIRECTSRC macro is missing from libc-shim"
#endif

#ifndef RTCF_DOREDIRECT
#error "net/route.h:RTCF_DOREDIRECT macro is missing from libc-shim"
#endif

#ifndef RTCF_LOG
#error "net/route.h:RTCF_LOG macro is missing from libc-shim"
#endif

#ifndef RTCF_MASQ
#error "net/route.h:RTCF_MASQ macro is missing from libc-shim"
#endif

#ifndef RTCF_NAT
#error "net/route.h:RTCF_NAT macro is missing from libc-shim"
#endif

#ifndef RTCF_VALVE
#error "net/route.h:RTCF_VALVE macro is missing from libc-shim"
#endif

#ifndef RTF_ADDRCLASSMASK
#error "net/route.h:RTF_ADDRCLASSMASK macro is missing from libc-shim"
#endif

#ifndef RTF_ADDRCONF
#error "net/route.h:RTF_ADDRCONF macro is missing from libc-shim"
#endif

#ifndef RTF_ALLONLINK
#error "net/route.h:RTF_ALLONLINK macro is missing from libc-shim"
#endif

#ifndef RTF_BROADCAST
#error "net/route.h:RTF_BROADCAST macro is missing from libc-shim"
#endif

#ifndef RTF_CACHE
#error "net/route.h:RTF_CACHE macro is missing from libc-shim"
#endif

#ifndef RTF_DEFAULT
#error "net/route.h:RTF_DEFAULT macro is missing from libc-shim"
#endif

#ifndef RTF_DYNAMIC
#error "net/route.h:RTF_DYNAMIC macro is missing from libc-shim"
#endif

#ifndef RTF_FLOW
#error "net/route.h:RTF_FLOW macro is missing from libc-shim"
#endif

#ifndef RTF_GATEWAY
#error "net/route.h:RTF_GATEWAY macro is missing from libc-shim"
#endif

#ifndef RTF_HOST
#error "net/route.h:RTF_HOST macro is missing from libc-shim"
#endif

#ifndef RTF_INTERFACE
#error "net/route.h:RTF_INTERFACE macro is missing from libc-shim"
#endif

#ifndef RTF_IRTT
#error "net/route.h:RTF_IRTT macro is missing from libc-shim"
#endif

#ifndef RTF_LINKRT
#error "net/route.h:RTF_LINKRT macro is missing from libc-shim"
#endif

#ifndef RTF_LOCAL
#error "net/route.h:RTF_LOCAL macro is missing from libc-shim"
#endif

#ifndef RTF_MODIFIED
#error "net/route.h:RTF_MODIFIED macro is missing from libc-shim"
#endif

#ifndef RTF_MSS
#error "net/route.h:RTF_MSS macro is missing from libc-shim"
#endif

#ifndef RTF_MTU
#error "net/route.h:RTF_MTU macro is missing from libc-shim"
#endif

#ifndef RTF_MULTICAST
#error "net/route.h:RTF_MULTICAST macro is missing from libc-shim"
#endif

#ifndef RTF_NAT
#error "net/route.h:RTF_NAT macro is missing from libc-shim"
#endif

#ifndef RTF_NOFORWARD
#error "net/route.h:RTF_NOFORWARD macro is missing from libc-shim"
#endif

#ifndef RTF_NONEXTHOP
#error "net/route.h:RTF_NONEXTHOP macro is missing from libc-shim"
#endif

#ifndef RTF_NOPMTUDISC
#error "net/route.h:RTF_NOPMTUDISC macro is missing from libc-shim"
#endif

#ifndef RTF_POLICY
#error "net/route.h:RTF_POLICY macro is missing from libc-shim"
#endif

#ifndef RTF_REINSTATE
#error "net/route.h:RTF_REINSTATE macro is missing from libc-shim"
#endif

#ifndef RTF_REJECT
#error "net/route.h:RTF_REJECT macro is missing from libc-shim"
#endif

#ifndef RTF_STATIC
#error "net/route.h:RTF_STATIC macro is missing from libc-shim"
#endif

#ifndef RTF_THROW
#error "net/route.h:RTF_THROW macro is missing from libc-shim"
#endif

#ifndef RTF_UP
#error "net/route.h:RTF_UP macro is missing from libc-shim"
#endif

#ifndef RTF_WINDOW
#error "net/route.h:RTF_WINDOW macro is missing from libc-shim"
#endif

#ifndef RTF_XRESOLVE
#error "net/route.h:RTF_XRESOLVE macro is missing from libc-shim"
#endif

#ifndef RTMSG_ACK
#error "net/route.h:RTMSG_ACK macro is missing from libc-shim"
#endif

#ifndef RTMSG_AR_FAILED
#error "net/route.h:RTMSG_AR_FAILED macro is missing from libc-shim"
#endif

#ifndef RTMSG_CONTROL
#error "net/route.h:RTMSG_CONTROL macro is missing from libc-shim"
#endif

#ifndef RTMSG_DELDEVICE
#error "net/route.h:RTMSG_DELDEVICE macro is missing from libc-shim"
#endif

#ifndef RTMSG_DELROUTE
#error "net/route.h:RTMSG_DELROUTE macro is missing from libc-shim"
#endif

#ifndef RTMSG_DELRULE
#error "net/route.h:RTMSG_DELRULE macro is missing from libc-shim"
#endif

#ifndef RTMSG_NEWDEVICE
#error "net/route.h:RTMSG_NEWDEVICE macro is missing from libc-shim"
#endif

#ifndef RTMSG_NEWROUTE
#error "net/route.h:RTMSG_NEWROUTE macro is missing from libc-shim"
#endif

#ifndef RTMSG_NEWRULE
#error "net/route.h:RTMSG_NEWRULE macro is missing from libc-shim"
#endif

#ifndef RTMSG_OVERRUN
#error "net/route.h:RTMSG_OVERRUN macro is missing from libc-shim"
#endif

#ifndef RT_ADDRCLASS
#error "net/route.h:RT_ADDRCLASS macro is missing from libc-shim"
#endif

#ifndef RT_CLASS_DEFAULT
#error "net/route.h:RT_CLASS_DEFAULT macro is missing from libc-shim"
#endif

#ifndef RT_CLASS_LOCAL
#error "net/route.h:RT_CLASS_LOCAL macro is missing from libc-shim"
#endif

#ifndef RT_CLASS_MAIN
#error "net/route.h:RT_CLASS_MAIN macro is missing from libc-shim"
#endif

#ifndef RT_CLASS_MAX
#error "net/route.h:RT_CLASS_MAX macro is missing from libc-shim"
#endif

#ifndef RT_CLASS_UNSPEC
#error "net/route.h:RT_CLASS_UNSPEC macro is missing from libc-shim"
#endif

#ifndef RT_LOCALADDR
#error "net/route.h:RT_LOCALADDR macro is missing from libc-shim"
#endif

#ifndef RT_TOS
#error "net/route.h:RT_TOS macro is missing from libc-shim"
#endif

#ifndef rt_mss
#error "net/route.h:rt_mss macro is missing from libc-shim"
#endif

int main(void) { return 0; }
