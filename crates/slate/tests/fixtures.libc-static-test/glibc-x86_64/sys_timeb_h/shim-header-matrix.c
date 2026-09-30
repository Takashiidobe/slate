#include <sys/timeb.h>

extern int slate_oracle_ftime(struct timeb *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ftime), __typeof__(ftime)),
    "sys/timeb.h:ftime declaration differs from oracle");

static __typeof__(ftime) *const slate_reference_ftime = &ftime;

int main(void) { return 0; }
