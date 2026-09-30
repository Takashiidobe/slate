#include <monetary.h>

extern long slate_oracle_strfmon_l(char *restrict, unsigned long, struct __locale_struct *, const char *restrict, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfmon_l), __typeof__(strfmon_l)),
    "monetary.h:strfmon_l declaration differs from oracle");

static __typeof__(strfmon_l) *const slate_reference_strfmon_l = &strfmon_l;

typedef long slate_oracle_typedef_ssize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ssize_t, ssize_t), "typedef ssize_t differs from oracle");

int main(void) { return 0; }
