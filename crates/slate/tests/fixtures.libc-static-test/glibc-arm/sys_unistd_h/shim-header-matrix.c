#include <sys/unistd.h>

extern int slate_oracle_gettid(void);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gettid), __typeof__(gettid)),
    "sys/unistd.h:gettid declaration differs from oracle");

static __typeof__(gettid) *const slate_reference_gettid = &gettid;

int main(void) { return 0; }
