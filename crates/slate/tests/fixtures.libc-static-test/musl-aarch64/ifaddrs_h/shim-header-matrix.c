#include <ifaddrs.h>

typedef struct ifaddrs * slate_oracle_struct_ifaddrs_ifa_next;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ifaddrs *)0)->ifa_next), slate_oracle_struct_ifaddrs_ifa_next), "struct ifaddrs.ifa_next field type differs from oracle");

typedef char * slate_oracle_struct_ifaddrs_ifa_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ifaddrs *)0)->ifa_name), slate_oracle_struct_ifaddrs_ifa_name), "struct ifaddrs.ifa_name field type differs from oracle");

typedef unsigned int slate_oracle_struct_ifaddrs_ifa_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ifaddrs *)0)->ifa_flags), slate_oracle_struct_ifaddrs_ifa_flags), "struct ifaddrs.ifa_flags field type differs from oracle");

typedef struct sockaddr * slate_oracle_struct_ifaddrs_ifa_addr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ifaddrs *)0)->ifa_addr), slate_oracle_struct_ifaddrs_ifa_addr), "struct ifaddrs.ifa_addr field type differs from oracle");

typedef struct sockaddr * slate_oracle_struct_ifaddrs_ifa_netmask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ifaddrs *)0)->ifa_netmask), slate_oracle_struct_ifaddrs_ifa_netmask), "struct ifaddrs.ifa_netmask field type differs from oracle");

typedef void * slate_oracle_struct_ifaddrs_ifa_data;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ifaddrs *)0)->ifa_data), slate_oracle_struct_ifaddrs_ifa_data), "struct ifaddrs.ifa_data field type differs from oracle");

#ifndef ifa_broadaddr
#error "ifaddrs.h:ifa_broadaddr macro is missing from libc-shim"
#endif

#ifndef ifa_dstaddr
#error "ifaddrs.h:ifa_dstaddr macro is missing from libc-shim"
#endif

int main(void) { return 0; }
