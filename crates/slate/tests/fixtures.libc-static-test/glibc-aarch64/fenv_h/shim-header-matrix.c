#include <fenv.h>

extern int slate_oracle_feclearexcept(int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feclearexcept), __typeof__(feclearexcept)),
    "fenv.h:feclearexcept declaration differs from oracle");

static __typeof__(feclearexcept) *const slate_reference_feclearexcept = &feclearexcept;

#ifndef FE_ALL_EXCEPT
#error "fenv.h:FE_ALL_EXCEPT macro is missing from libc-shim"
#endif

#ifndef FE_DFL_ENV
#error "fenv.h:FE_DFL_ENV macro is missing from libc-shim"
#endif

#ifndef FE_DFL_MODE
#error "fenv.h:FE_DFL_MODE macro is missing from libc-shim"
#endif

#ifndef FE_DIVBYZERO
#error "fenv.h:FE_DIVBYZERO macro is missing from libc-shim"
#endif

#ifndef FE_DOWNWARD
#error "fenv.h:FE_DOWNWARD macro is missing from libc-shim"
#endif

#ifndef FE_EXCEPT_SHIFT
#error "fenv.h:FE_EXCEPT_SHIFT macro is missing from libc-shim"
#endif

#ifndef FE_INEXACT
#error "fenv.h:FE_INEXACT macro is missing from libc-shim"
#endif

#ifndef FE_INVALID
#error "fenv.h:FE_INVALID macro is missing from libc-shim"
#endif

#ifndef FE_NOMASK_ENV
#error "fenv.h:FE_NOMASK_ENV macro is missing from libc-shim"
#endif

#ifndef FE_OVERFLOW
#error "fenv.h:FE_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef FE_TONEAREST
#error "fenv.h:FE_TONEAREST macro is missing from libc-shim"
#endif

#ifndef FE_TOWARDZERO
#error "fenv.h:FE_TOWARDZERO macro is missing from libc-shim"
#endif

#ifndef FE_UNDERFLOW
#error "fenv.h:FE_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef FE_UPWARD
#error "fenv.h:FE_UPWARD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
