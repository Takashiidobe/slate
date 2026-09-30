#include <execinfo.h>

extern int slate_oracle_backtrace(void **, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_backtrace), __typeof__(backtrace)),
    "execinfo.h:backtrace declaration differs from oracle");

static __typeof__(backtrace) *const slate_reference_backtrace = &backtrace;

int main(void) { return 0; }
