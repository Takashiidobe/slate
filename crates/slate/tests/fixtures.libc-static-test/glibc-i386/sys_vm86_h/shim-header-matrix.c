#include <sys/vm86.h>

extern int slate_oracle_vm86(unsigned long, struct vm86plus_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vm86), __typeof__(vm86)),
    "sys/vm86.h:vm86 declaration differs from oracle");

static __typeof__(vm86) *const slate_reference_vm86 = &vm86;

int main(void) { return 0; }
