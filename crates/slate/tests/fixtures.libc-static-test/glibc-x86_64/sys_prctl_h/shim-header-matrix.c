#include <sys/prctl.h>

extern int slate_oracle_prctl(int, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_prctl), __typeof__(prctl)),
    "sys/prctl.h:prctl declaration differs from oracle");

static __typeof__(prctl) *const slate_reference_prctl = &prctl;

int main(void) { return 0; }
