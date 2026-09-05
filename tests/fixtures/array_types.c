#include <stdio.h>

static int sum_char_array(void) {
  char values[3];
  values[0] = 2;
  values[1] = 5;
  values[2] = values[0] + values[1];
  return values[2];
}

static double pick_double_array(int index) {
  double values[3];
  values[0] = 1.25;
  values[1] = 2.50;
  values[2] = values[0] + values[1];
  return values[index];
}

int main(void) {
  printf("%d\n", sum_char_array());
  printf("%.2f\n", pick_double_array(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum_char_array();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.2f\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.2f\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = pick_double_array({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum_char_array() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut values: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 2;
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 5;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pick_double_array({{arg[0-9]+}}: i32) -> f64 {
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [f64; 3]> = aligned::Aligned([0.0; 3]);
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [f64; 3] = [0.0; 3];
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.25;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{arg[0-9]+}} as i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = values[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), sum_char_array()) };
// REWRITES-NEXT:     unsafe { printf(c"%.2f\n".as_ptr(), pick_double_array(2)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_char_array() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut values: [i8; 3] = [0; 3];
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [u8; 3] = [0; 3];
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 2;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 5;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// REWRITES-X86_64-GNU-NEXT:     values[({{__v[0-9]+}} as usize)] = ((values[0] as i32) + (values[1] as i32)) as i8;
// REWRITES-AARCH64-GNU-NEXT:     values[({{__v[0-9]+}} as usize)] = ((values[0] as i32) + (values[1] as i32)) as u8;
// REWRITES-NEXT:     values[2] as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pick_double_array({{arg[0-9]+}}: i32) -> f64 {
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [f64; 3]> = aligned::Aligned([0.0; 3]);
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [f64; 3] = [0.0; 3];
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 1.25;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 2.5;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = values[0] + values[1];
// REWRITES-NEXT:     values[(({{arg[0-9]+}} as i64) as usize)]
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
