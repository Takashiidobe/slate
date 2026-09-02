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
// LOWERING-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// LOWERING-BIONIC-AARCH64-NEXT:     fn store_long_double(_0: *mut f128);
// LOWERING-BIONIC-AARCH64-NEXT:     fn load_long_double(_0: *const f128) -> *const f128;
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn probe() -> f128 {
// LOWERING-BIONIC-AARCH64-NEXT:     let mut value: f128 = 0.0f128;
// LOWERING-BIONIC-AARCH64-NEXT:     unsafe { store_long_double(std::ptr::addr_of_mut!(value) as *mut f128) };
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut f128 =
// LOWERING-BIONIC-AARCH64-NEXT:         (unsafe { load_long_double(std::ptr::addr_of_mut!(value) as *const f128) }) as *mut f128;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: f128 = unsafe { *{{_v[0-9]+}} };
// LOWERING-BIONIC-AARCH64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-AARCH64-NEXT: }
// LOWERING-BIONIC-AARCH64-EMPTY:
// LOWERING-BIONIC-AARCH64-NEXT: fn main() {
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
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
// LOWERING-BIONIC-X86_64-NEXT: unsafe extern "C" {
// LOWERING-BIONIC-X86_64-NEXT:     fn store_long_double(_0: *mut f128);
// LOWERING-BIONIC-X86_64-NEXT:     fn load_long_double(_0: *const f128) -> *const f128;
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn probe() -> f128 {
// LOWERING-BIONIC-X86_64-NEXT:     let mut value: f128 = 0.0f128;
// LOWERING-BIONIC-X86_64-NEXT:     unsafe { store_long_double(std::ptr::addr_of_mut!(value) as *mut f128) };
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: *mut f128 =
// LOWERING-BIONIC-X86_64-NEXT:         (unsafe { load_long_double(std::ptr::addr_of_mut!(value) as *const f128) }) as *mut f128;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: f128 = unsafe { *{{_v[0-9]+}} };
// LOWERING-BIONIC-X86_64-NEXT:     return {{_v[0-9]+}};
// LOWERING-BIONIC-X86_64-NEXT: }
// LOWERING-BIONIC-X86_64-EMPTY:
// LOWERING-BIONIC-X86_64-NEXT: fn main() {
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-BIONIC-X86_64-NEXT:     let {{_v[0-9]+}}: i32 = 0;
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
// REWRITES-BIONIC-AARCH64-NEXT: unsafe extern "C" {
// REWRITES-BIONIC-AARCH64-NEXT:     fn store_long_double(_0: *mut f128);
// REWRITES-BIONIC-AARCH64-NEXT:     fn load_long_double(_0: *const f128) -> *const f128;
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn probe() -> f128 {
// REWRITES-BIONIC-AARCH64-NEXT:     let mut value: f128 = 0.0f128;
// REWRITES-BIONIC-AARCH64-NEXT:     unsafe { store_long_double(std::ptr::addr_of_mut!(value) as *mut f128) };
// REWRITES-BIONIC-AARCH64-NEXT:     let {{_v[0-9]+}}: *mut f128 =
// REWRITES-BIONIC-AARCH64-NEXT:         (unsafe { load_long_double(std::ptr::addr_of_mut!(value) as *const f128) }) as *mut f128;
// REWRITES-BIONIC-AARCH64-NEXT:     unsafe { *{{_v[0-9]+}} }
// REWRITES-BIONIC-AARCH64-NEXT: }
// REWRITES-BIONIC-AARCH64-EMPTY:
// REWRITES-BIONIC-AARCH64-NEXT: fn main() {
// REWRITES-BIONIC-AARCH64-NEXT:     std::process::exit(0 as i32);
// REWRITES-BIONIC-AARCH64-NEXT: }
// SLATE-FILECHECK-END rewrites-bionic-aarch64
