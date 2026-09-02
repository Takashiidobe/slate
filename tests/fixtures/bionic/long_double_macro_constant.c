#include <float.h>

long double f(void) { return LDBL_TRUE_MIN; }

int main(void) { return f() == 0.0L; }
// REWRITES-BIONIC-X86_64-DAG: #![feature(f128)]
// REWRITES-BIONIC-X86_64-NOT: struct LongDouble

// SLATE-FILECHECK-BEGIN lowering-bionic-aarch64
// LOWERING-BIONIC-AARCH64: #![feature(f128)]
// LOWERING-BIONIC-AARCH64-NEXT: #![allow(
// LOWERING-BIONIC-AARCH64-NEXT:     dead_code,
// LOWERING-BIONIC-AARCH64-NEXT:     unused,
// LOWERING-BIONIC-AARCH64-NEXT:     non_camel_case_types,
// LOWERING-BIONIC-AARCH64-NEXT:     non_snake_case,
// LOWERING-BIONIC-AARCH64-NEXT:     non_upper_case_globals,
// LOWERING-BIONIC-AARCH64-NEXT:     arithmetic_overflow,
// LOWERING-BIONIC-AARCH64-NEXT:     unconditional_panic,
// LOWERING-BIONIC-AARCH64-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-BIONIC-AARCH64-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-BIONIC-AARCH64-NEXT:     unused_comparisons
// LOWERING-BIONIC-AARCH64-NEXT: )]
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn f() -> f128 {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: f128 = 6.475180e-4966f128;
// LOWERING-BIONIC-AARCH64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn main() {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: f128 = f();
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-BIONIC-AARCH64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-aarch64

// SLATE-FILECHECK-BEGIN lowering-bionic-x86_64
// LOWERING-BIONIC-X86_64: #![feature(f128)]
// LOWERING-BIONIC-X86_64-NEXT: #![allow(
// LOWERING-BIONIC-X86_64-NEXT:     dead_code,
// LOWERING-BIONIC-X86_64-NEXT:     unused,
// LOWERING-BIONIC-X86_64-NEXT:     non_camel_case_types,
// LOWERING-BIONIC-X86_64-NEXT:     non_snake_case,
// LOWERING-BIONIC-X86_64-NEXT:     non_upper_case_globals,
// LOWERING-BIONIC-X86_64-NEXT:     arithmetic_overflow,
// LOWERING-BIONIC-X86_64-NEXT:     unconditional_panic,
// LOWERING-BIONIC-X86_64-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-BIONIC-X86_64-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-BIONIC-X86_64-NEXT:     unused_comparisons
// LOWERING-BIONIC-X86_64-NEXT: )]
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn f() -> f128 {
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: f128 = 6.475180e-4966f128;
// LOWERING-BIONIC-X86_64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn main() {
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: f128 = f();
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-BIONIC-X86_64-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-BIONIC-X86_64-NEXT: }
// SLATE-FILECHECK-END lowering-bionic-x86_64

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
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: f128 = 6.475180e-4966f128;
// REWRITES-BIONIC-AARCH64-NEXT:     {{_v[0-9]+}}
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: f128 = f();
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-BIONIC-AARCH64-NEXT:     std::process::exit(({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64
