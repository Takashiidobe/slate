#include <sys/auxv.h>

extern unsigned long slate_oracle_getauxval(unsigned long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getauxval), __typeof__(getauxval)),
    "sys/auxv.h:getauxval declaration differs from oracle");

static __typeof__(getauxval) *const slate_reference_getauxval = &getauxval;

int main(void) { return 0; }
