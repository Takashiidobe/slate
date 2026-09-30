#include <sys/io.h>

extern int slate_oracle_iopl(int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_iopl), __typeof__(iopl)),
    "sys/io.h:iopl declaration differs from oracle");

static __typeof__(iopl) *const slate_reference_iopl = &iopl;

int main(void) { return 0; }
