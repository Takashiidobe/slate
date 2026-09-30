#include <ucontext.h>

extern int slate_oracle_getcontext(ucontext_t *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getcontext), __typeof__(getcontext)),
    "ucontext.h:getcontext declaration differs from oracle");

static __typeof__(getcontext) *const slate_reference_getcontext = &getcontext;

typedef unsigned long slate_oracle_typedef_greg_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_greg_t, greg_t), "typedef greg_t differs from oracle");

int main(void) { return 0; }
