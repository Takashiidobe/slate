#include <netinet/ether.h>

extern char * slate_oracle_ether_ntoa(const struct ether_addr *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ether_ntoa), __typeof__(ether_ntoa)),
    "netinet/ether.h:ether_ntoa declaration differs from oracle");

static __typeof__(ether_ntoa) *const slate_reference_ether_ntoa = &ether_ntoa;

int main(void) { return 0; }
