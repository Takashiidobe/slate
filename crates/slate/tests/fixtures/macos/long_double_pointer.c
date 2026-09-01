extern void               store_long_double(long double *);
extern const long double *load_long_double(const long double *);

long double probe(void) {
  long double value;
  store_long_double(&value);
  return *load_long_double(&value);
}

int main(void) { return 0; }
// REWRITES-MACOS-DAG: fn store_long_double(_0: *mut f64);
// REWRITES-MACOS-DAG: fn load_long_double(_0: *const f64) -> *const f64;

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS: #![allow(
// LOWERING-MACOS-NEXT:     dead_code,
// LOWERING-MACOS-NEXT:     unused,
// LOWERING-MACOS-NEXT:     non_camel_case_types,
// LOWERING-MACOS-NEXT:     non_snake_case,
// LOWERING-MACOS-NEXT:     non_upper_case_globals,
// LOWERING-MACOS-NEXT:     arithmetic_overflow,
// LOWERING-MACOS-NEXT:     unconditional_panic,
// LOWERING-MACOS-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MACOS-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MACOS-NEXT:     unused_comparisons
// LOWERING-MACOS-NEXT: )]
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: unsafe extern "C" {
// LOWERING-MACOS-NEXT:     fn store_long_double(_0: *mut f64);
// LOWERING-MACOS-NEXT:     fn load_long_double(_0: *const f64) -> *const f64;
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn probe() -> f64 {
// LOWERING-MACOS-NEXT:     let mut value: f64 = 0.0;
// LOWERING-MACOS-NEXT:     unsafe { store_long_double(std::ptr::addr_of_mut!(value) as *mut f64) };
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: *mut f64 =
// LOWERING-MACOS-NEXT:         (unsafe { load_long_double(std::ptr::addr_of_mut!(value) as *const f64) }) as *mut f64;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-MACOS-NEXT:     return {{_v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn main() {
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MACOS-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-MACOS-NEXT: }
// SLATE-FILECHECK-END lowering-macos
