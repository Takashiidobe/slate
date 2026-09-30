#include <sys/perm.h>

extern int slate_oracle_ioperm(unsigned long, unsigned long, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ioperm), __typeof__(ioperm)),
    "sys/perm.h:ioperm declaration differs from oracle");

static __typeof__(ioperm) *const slate_reference_ioperm = &ioperm;

int main(void) { return 0; }
