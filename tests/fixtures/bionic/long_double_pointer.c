extern void               store_long_double(long double *);
extern const long double *load_long_double(const long double *);

long double probe(void) {
  long double value;
  store_long_double(&value);
  return *load_long_double(&value);
}

int main(void) { return 0; }
// REWRITES-BIONIC-X86_64-DAG: fn store_long_double(_0: *mut f128);
// REWRITES-BIONIC-X86_64-DAG: fn load_long_double(_0: *const f128) -> *const f128;

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
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn store_long_double(_0: *mut f128);
// COMMON-LOWERING-NEXT:     fn load_long_double(_0: *const f128) -> *const f128;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn probe() -> f128 {
// COMMON-LOWERING-NEXT:     let mut value: f128 = 0.0f128;
// COMMON-LOWERING-NEXT:     unsafe { store_long_double(std::ptr::addr_of_mut!(value) as *mut f128) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut f128 =
// COMMON-LOWERING-NEXT:         (unsafe { load_long_double(std::ptr::addr_of_mut!(value) as *const f128) }) as *mut f128;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f128 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
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
// REWRITES-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// REWRITES-BIONIC-AARCH64-NEXT:     fn store_long_double(_0: *mut f128);
// REWRITES-BIONIC-AARCH64-NEXT:     fn load_long_double(_0: *const f128) -> *const f128;
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn probe() -> f128 {
// REWRITES-BIONIC-AARCH64-NEXT:     let mut value: f128 = 0.0f128;
// REWRITES-BIONIC-AARCH64-NEXT:     unsafe { store_long_double(std::ptr::addr_of_mut!(value) as *mut f128) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{__v[0-9]+}}: *mut f128 =
// REWRITES-BIONIC-AARCH64-NEXT:         (unsafe { load_long_double(std::ptr::addr_of_mut!(value) as *const f128) }) as *mut f128;
// REWRITES-BIONIC-AARCH64-NEXT:     unsafe { *{{__v[0-9]+}} }
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT:     std::process::exit(0 as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64
