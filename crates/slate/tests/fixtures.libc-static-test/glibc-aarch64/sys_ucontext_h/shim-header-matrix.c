#include <sys/ucontext.h>

typedef unsigned long slate_oracle_typedef_greg_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_greg_t, greg_t), "typedef greg_t differs from oracle");

int main(void) { return 0; }
