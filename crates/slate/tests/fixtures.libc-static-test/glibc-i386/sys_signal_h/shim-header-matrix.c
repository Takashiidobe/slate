#include <sys/signal.h>

extern int slate_oracle_tgkill(int, int, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgkill), __typeof__(tgkill)),
    "sys/signal.h:tgkill declaration differs from oracle");

static __typeof__(tgkill) *const slate_reference_tgkill = &tgkill;

int main(void) { return 0; }
