#include <strings.h>

extern int slate_oracle_bcmp(const void *, const void *, __size_t);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bcmp), __typeof__(bcmp)),
    "strings.h:bcmp declaration differs from oracle");

static __typeof__(bcmp) *const slate_reference_bcmp = &bcmp;

int main(void) { return 0; }
