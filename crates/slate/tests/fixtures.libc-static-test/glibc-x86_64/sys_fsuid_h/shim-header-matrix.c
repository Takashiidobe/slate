#include <sys/fsuid.h>

extern int slate_oracle_setfsuid(unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setfsuid), __typeof__(setfsuid)),
    "sys/fsuid.h:setfsuid declaration differs from oracle");

static __typeof__(setfsuid) *const slate_reference_setfsuid = &setfsuid;

int main(void) { return 0; }
