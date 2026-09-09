#include <arpa/inet.h>

extern unsigned int slate_oracle_inet_addr(const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_inet_addr), __typeof__(inet_addr)),
    "arpa/inet.h:inet_addr declaration differs from oracle");

static __typeof__(inet_addr) *const slate_reference_inet_addr = &inet_addr;

int main(void) { return 0; }
