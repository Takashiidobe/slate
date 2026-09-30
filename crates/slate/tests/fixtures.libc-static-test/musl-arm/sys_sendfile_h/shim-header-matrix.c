#include <sys/sendfile.h>

extern int slate_oracle_sendfile(int, int, long long *, unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sendfile), __typeof__(sendfile)),
    "sys/sendfile.h:sendfile declaration differs from oracle");

static __typeof__(sendfile) *const slate_reference_sendfile = &sendfile;

int main(void) { return 0; }
