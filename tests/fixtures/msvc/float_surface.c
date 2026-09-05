#include <float.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(_SW_INVALID == 0x10, "_SW_INVALID");
_Static_assert(_EM_DENORMAL == 0x00080000, "_EM_DENORMAL");
_Static_assert(_MCW_RC == 0x00000300, "_MCW_RC");
_Static_assert(_RC_CHOP == 0x00000300, "_RC_CHOP");
_Static_assert(_MCW_DN == 0x03000000, "_MCW_DN");
_Static_assert(_FPE_MULTIPLE_FAULTS == 0x8e, "_FPE_MULTIPLE_FAULTS");
_Static_assert(_FPCLASS_PINF == 0x0200, "_FPCLASS_PINF");
_Static_assert(_CW_DEFAULT == 0x0008001f, "_CW_DEFAULT");

#ifdef DBL_NORM_MAX
#error "MSVC float.h must not expose C23 DBL_NORM_MAX"
#endif
#ifdef LDBL_SNAN
#error "MSVC float.h must not expose C23 LDBL_SNAN"
#endif

TYPE_IS(&_clearfp, unsigned int (*)(void));
TYPE_IS(&_controlfp, unsigned int (*)(unsigned int, unsigned int));
TYPE_IS(&_set_controlfp, void (*)(unsigned int, unsigned int));
TYPE_IS(&_controlfp_s, errno_t (*)(unsigned int *, unsigned int, unsigned int));
TYPE_IS(&_statusfp, unsigned int (*)(void));
TYPE_IS(&_fpreset, void (*)(void));
TYPE_IS(&_control87, unsigned int (*)(unsigned int, unsigned int));
TYPE_IS(&__fpecode, int *(*)(void));
TYPE_IS(&__fpe_flt_rounds, int (*)(void));
TYPE_IS(&_scalb, double (*)(double, long));
TYPE_IS(&_logb, double (*)(double));
TYPE_IS(&_nextafter, double (*)(double, double));
TYPE_IS(&_finite, int (*)(double));
TYPE_IS(&_isnan, int (*)(double));
TYPE_IS(&_fpclass, int (*)(double));
TYPE_IS(&_scalbf, float (*)(float, long));

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN lowering-msvc
// LOWERING-MSVC: #![allow(
// LOWERING-MSVC-NEXT:     dead_code,
// LOWERING-MSVC-NEXT:     unused,
// LOWERING-MSVC-NEXT:     non_camel_case_types,
// LOWERING-MSVC-NEXT:     non_snake_case,
// LOWERING-MSVC-NEXT:     non_upper_case_globals,
// LOWERING-MSVC-NEXT:     arithmetic_overflow,
// LOWERING-MSVC-NEXT:     unconditional_panic,
// LOWERING-MSVC-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MSVC-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MSVC-NEXT:     unused_comparisons
// LOWERING-MSVC-NEXT: )]
// LOWERING-MSVC-EMPTY:
// LOWERING-MSVC-NEXT: compile_error!("\"MSVC float.h must not expose C23 DBL_NORM_MAX\"");
// LOWERING-MSVC-EMPTY:
// LOWERING-MSVC-NEXT: compile_error!("\"MSVC float.h must not expose C23 LDBL_SNAN\"");
// LOWERING-MSVC-EMPTY:
// LOWERING-MSVC-NEXT: fn main() {
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-MSVC-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-MSVC-NEXT: }
// SLATE-FILECHECK-END lowering-msvc

// SLATE-FILECHECK-BEGIN rewrites-msvc
// REWRITES-MSVC: #![allow(
// REWRITES-MSVC-NEXT:     dead_code,
// REWRITES-MSVC-NEXT:     unused,
// REWRITES-MSVC-NEXT:     non_camel_case_types,
// REWRITES-MSVC-NEXT:     non_snake_case,
// REWRITES-MSVC-NEXT:     non_upper_case_globals,
// REWRITES-MSVC-NEXT:     arithmetic_overflow,
// REWRITES-MSVC-NEXT:     unconditional_panic,
// REWRITES-MSVC-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-MSVC-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-MSVC-NEXT:     unused_comparisons
// REWRITES-MSVC-NEXT: )]
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: compile_error!("\"MSVC float.h must not expose C23 DBL_NORM_MAX\"");
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: compile_error!("\"MSVC float.h must not expose C23 LDBL_SNAN\"");
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: fn main() {
// REWRITES-MSVC-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-MSVC-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-MSVC-NEXT: }
// SLATE-FILECHECK-END rewrites-msvc
