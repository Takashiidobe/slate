#include <error.h>

extern void slate_oracle_error(int, int, const char *, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_error), __typeof__(error)),
    "error.h:error declaration differs from oracle");

static __typeof__(error) *const slate_reference_error = &error;

int main(void) { return 0; }
