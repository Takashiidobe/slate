#include <complex.h>

extern double slate_oracle_cabs(_Complex double);
extern _Complex double slate_oracle_cacos(_Complex double);
extern _Complex double slate_oracle_cacosh(_Complex double);
extern double slate_oracle_carg(_Complex double);
extern _Complex double slate_oracle_casin(_Complex double);
extern _Complex double slate_oracle_casinh(_Complex double);
extern _Complex double slate_oracle_catan(_Complex double);
extern _Complex double slate_oracle_catanh(_Complex double);
extern _Complex double slate_oracle_ccos(_Complex double);
extern _Complex double slate_oracle_ccosh(_Complex double);
extern _Complex double slate_oracle_cexp(_Complex double);
extern double slate_oracle_cimag(_Complex double);
extern _Complex double slate_oracle_clog(_Complex double);
extern _Complex double slate_oracle_clog10(_Complex double);
extern _Complex double slate_oracle_conj(_Complex double);
extern _Complex double slate_oracle_cpow(_Complex double, _Complex double);
extern _Complex double slate_oracle_cproj(_Complex double);
extern double slate_oracle_creal(_Complex double);
extern _Complex double slate_oracle_csin(_Complex double);
extern _Complex double slate_oracle_csinh(_Complex double);
extern _Complex double slate_oracle_csqrt(_Complex double);
extern _Complex double slate_oracle_ctan(_Complex double);
extern _Complex double slate_oracle_ctanh(_Complex double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cabs), __typeof__(cabs)),
    "complex.h:cabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cacos), __typeof__(cacos)),
    "complex.h:cacos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cacosh), __typeof__(cacosh)),
    "complex.h:cacosh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_carg), __typeof__(carg)),
    "complex.h:carg declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_casin), __typeof__(casin)),
    "complex.h:casin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_casinh), __typeof__(casinh)),
    "complex.h:casinh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_catan), __typeof__(catan)),
    "complex.h:catan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_catanh), __typeof__(catanh)),
    "complex.h:catanh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ccos), __typeof__(ccos)),
    "complex.h:ccos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ccosh), __typeof__(ccosh)),
    "complex.h:ccosh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cexp), __typeof__(cexp)),
    "complex.h:cexp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cimag), __typeof__(cimag)),
    "complex.h:cimag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clog), __typeof__(clog)),
    "complex.h:clog declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clog10), __typeof__(clog10)),
    "complex.h:clog10 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_conj), __typeof__(conj)),
    "complex.h:conj declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cpow), __typeof__(cpow)),
    "complex.h:cpow declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cproj), __typeof__(cproj)),
    "complex.h:cproj declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_creal), __typeof__(creal)),
    "complex.h:creal declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_csin), __typeof__(csin)),
    "complex.h:csin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_csinh), __typeof__(csinh)),
    "complex.h:csinh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_csqrt), __typeof__(csqrt)),
    "complex.h:csqrt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ctan), __typeof__(ctan)),
    "complex.h:ctan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ctanh), __typeof__(ctanh)),
    "complex.h:ctanh declaration differs from oracle");

static __typeof__(cabs) *const slate_reference_cabs = &cabs;
static __typeof__(cacos) *const slate_reference_cacos = &cacos;
static __typeof__(cacosh) *const slate_reference_cacosh = &cacosh;
static __typeof__(carg) *const slate_reference_carg = &carg;
static __typeof__(casin) *const slate_reference_casin = &casin;
static __typeof__(casinh) *const slate_reference_casinh = &casinh;
static __typeof__(catan) *const slate_reference_catan = &catan;
static __typeof__(catanh) *const slate_reference_catanh = &catanh;
static __typeof__(ccos) *const slate_reference_ccos = &ccos;
static __typeof__(ccosh) *const slate_reference_ccosh = &ccosh;
static __typeof__(cexp) *const slate_reference_cexp = &cexp;
static __typeof__(cimag) *const slate_reference_cimag = &cimag;
static __typeof__(clog) *const slate_reference_clog = &clog;
static __typeof__(clog10) *const slate_reference_clog10 = &clog10;
static __typeof__(conj) *const slate_reference_conj = &conj;
static __typeof__(cpow) *const slate_reference_cpow = &cpow;
static __typeof__(cproj) *const slate_reference_cproj = &cproj;
static __typeof__(creal) *const slate_reference_creal = &creal;
static __typeof__(csin) *const slate_reference_csin = &csin;
static __typeof__(csinh) *const slate_reference_csinh = &csinh;
static __typeof__(csqrt) *const slate_reference_csqrt = &csqrt;
static __typeof__(ctan) *const slate_reference_ctan = &ctan;
static __typeof__(ctanh) *const slate_reference_ctanh = &ctanh;

#ifndef CMPLX
#error "complex.h:CMPLX macro is missing from libc-shim"
#endif

#ifndef CMPLXF
#error "complex.h:CMPLXF macro is missing from libc-shim"
#endif

#ifndef CMPLXF32
#error "complex.h:CMPLXF32 macro is missing from libc-shim"
#endif

#ifndef CMPLXF32X
#error "complex.h:CMPLXF32X macro is missing from libc-shim"
#endif

#ifndef CMPLXF64
#error "complex.h:CMPLXF64 macro is missing from libc-shim"
#endif

#ifndef CMPLXL
#error "complex.h:CMPLXL macro is missing from libc-shim"
#endif

#ifndef I
#error "complex.h:I macro is missing from libc-shim"
#endif

#ifndef complex
#error "complex.h:complex macro is missing from libc-shim"
#endif

int main(void) { return 0; }
