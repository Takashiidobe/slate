#include <float.h>

long double f(void) { return LDBL_TRUE_MIN; }

int main(void) { return f() == 0.0L; }
// REWRITES-BIONIC-X86_64-DAG: #![feature(f128)]
// REWRITES-BIONIC-X86_64-NOT: struct LongDouble

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(f128)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn f() -> f128 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f128 = 6.475180e-4966f128;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f128 = f();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN rewrites-bionic-aarch64
// REWRITES-BIONIC-AARCH64: #![feature(f128)]
// REWRITES-BIONIC-AARCH64-NEXT: #![allow(
// REWRITES-BIONIC-AARCH64-NEXT:     dead_code,
// REWRITES-BIONIC-AARCH64-NEXT:     unused,
// REWRITES-BIONIC-AARCH64-NEXT:     non_camel_case_types,
// REWRITES-BIONIC-AARCH64-NEXT:     non_snake_case,
// REWRITES-BIONIC-AARCH64-NEXT:     non_upper_case_globals,
// REWRITES-BIONIC-AARCH64-NEXT:     arithmetic_overflow,
// REWRITES-BIONIC-AARCH64-NEXT:     unconditional_panic,
// REWRITES-BIONIC-AARCH64-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-BIONIC-AARCH64-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-BIONIC-AARCH64-NEXT:     unused_comparisons
// REWRITES-BIONIC-AARCH64-NEXT: )]
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn f() -> f128 {
// REWRITES-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: f128 = 6.475180e-4966f128;
// REWRITES-BIONIC-AARCH64-NEXT:     {{__v[0-9]+}}
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: f128 = f();
// REWRITES-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-BIONIC-AARCH64-NEXT:     std::process::exit(({{__v[0-9]+}} == {{__v[0-9]+}}) as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64
