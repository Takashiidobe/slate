#include <arpa/inet.h>

extern unsigned int slate_oracle_htonl(unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_htonl), __typeof__(htonl)),
    "arpa/inet.h:htonl declaration differs from oracle");

static __typeof__(htonl) *const slate_reference_htonl = &htonl;

int main(void) { return 0; }
