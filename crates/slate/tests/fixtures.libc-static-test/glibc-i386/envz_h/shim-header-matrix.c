#include <envz.h>

extern char * slate_oracle_envz_entry(const char *restrict, unsigned int, const char *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_envz_entry), __typeof__(envz_entry)),
    "envz.h:envz_entry declaration differs from oracle");

static __typeof__(envz_entry) *const slate_reference_envz_entry = &envz_entry;

int main(void) { return 0; }
