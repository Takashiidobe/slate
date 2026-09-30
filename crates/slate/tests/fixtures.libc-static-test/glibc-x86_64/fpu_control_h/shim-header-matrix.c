#include <fpu_control.h>

typedef unsigned short slate_oracle_typedef_fpu_control_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fpu_control_t, fpu_control_t), "typedef fpu_control_t differs from oracle");

int main(void) { return 0; }
