#include <err.h>

extern void slate_oracle_warn(const char *, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_warn), __typeof__(warn)),
    "err.h:warn declaration differs from oracle");

static __typeof__(warn) *const slate_reference_warn = &warn;

int main(void) { return 0; }
