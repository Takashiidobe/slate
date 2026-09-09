#include <complex.h>

extern _Complex double slate_oracle_cacos(_Complex double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cacos), __typeof__(cacos)),
    "complex.h:cacos declaration differs from oracle");

static __typeof__(cacos) *const slate_reference_cacos = &cacos;

#ifndef CMPLX
#error "complex.h:CMPLX macro is missing from libc-shim"
#endif

#ifndef CMPLXF
#error "complex.h:CMPLXF macro is missing from libc-shim"
#endif

#ifndef CMPLXL
#error "complex.h:CMPLXL macro is missing from libc-shim"
#endif

#ifndef I
#error "complex.h:I macro is missing from libc-shim"
#endif

#ifndef cimag
#error "complex.h:cimag macro is missing from libc-shim"
#endif

#ifndef cimagf
#error "complex.h:cimagf macro is missing from libc-shim"
#endif

#ifndef cimagl
#error "complex.h:cimagl macro is missing from libc-shim"
#endif

#ifndef complex
#error "complex.h:complex macro is missing from libc-shim"
#endif

#ifndef creal
#error "complex.h:creal macro is missing from libc-shim"
#endif

#ifndef crealf
#error "complex.h:crealf macro is missing from libc-shim"
#endif

#ifndef creall
#error "complex.h:creall macro is missing from libc-shim"
#endif

int main(void) { return 0; }
