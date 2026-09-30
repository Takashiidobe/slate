#include <monetary.h>

extern int slate_oracle_strfmon(char *restrict, unsigned int, const char *restrict, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfmon), __typeof__(strfmon)),
    "monetary.h:strfmon declaration differs from oracle");

static __typeof__(strfmon) *const slate_reference_strfmon = &strfmon;

int main(void) { return 0; }
