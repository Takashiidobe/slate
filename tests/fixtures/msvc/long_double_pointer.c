extern void               store_long_double(long double *);
extern const long double *load_long_double(const long double *);

long double probe(void) {
  long double value;
  store_long_double(&value);
  return *load_long_double(&value);
}

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
// LOWERING-MSVC-NEXT: unsafe extern "C" {
// LOWERING-MSVC-NEXT:     fn store_long_double(_0: *mut f64);
// LOWERING-MSVC-NEXT:     fn load_long_double(_0: *const f64) -> *const f64;
// LOWERING-MSVC-NEXT: }
// LOWERING-MSVC-EMPTY:
// LOWERING-MSVC-NEXT: fn probe() -> f64 {
// LOWERING-MSVC-NEXT:     let mut value: f64 = 0.0;
// LOWERING-MSVC-NEXT:     unsafe { store_long_double(std::ptr::addr_of_mut!(value) as *mut f64) };
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: *mut f64 =
// LOWERING-MSVC-NEXT:         (unsafe { load_long_double(std::ptr::addr_of_mut!(value) as *const f64) }) as *mut f64;
// LOWERING-MSVC-NEXT:     let {{__v[0-9]+}}: f64 = unsafe { *{{__v[0-9]+}} };
// LOWERING-MSVC-NEXT:     return {{__v[0-9]+}};
// LOWERING-MSVC-NEXT: }
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
// REWRITES-MSVC-NEXT: unsafe extern "C" {
// REWRITES-MSVC-NEXT:     fn store_long_double(_0: *mut f64);
// REWRITES-MSVC-NEXT:     fn load_long_double(_0: *const f64) -> *const f64;
// REWRITES-MSVC-NEXT: }
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: fn probe() -> f64 {
// REWRITES-MSVC-NEXT:     let mut value: f64 = 0.0;
// REWRITES-MSVC-NEXT:     unsafe { store_long_double(std::ptr::addr_of_mut!(value) as *mut f64) };
// REWRITES-MSVC-NEXT:     let {{__v[0-9]+}}: *mut f64 =
// REWRITES-MSVC-NEXT:         (unsafe { load_long_double(std::ptr::addr_of_mut!(value) as *const f64) }) as *mut f64;
// REWRITES-MSVC-NEXT:     unsafe { *{{__v[0-9]+}} }
// REWRITES-MSVC-NEXT: }
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: fn main() {
// REWRITES-MSVC-NEXT:     std::process::exit(0 as i32);
// REWRITES-MSVC-NEXT: }
// SLATE-FILECHECK-END rewrites-msvc
