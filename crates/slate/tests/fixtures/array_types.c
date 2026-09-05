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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
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
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum_char_array();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = pick_double_array({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn sum_char_array() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pick_double_array({{arg[0-9]+}}: i32) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.25;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     values[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{arg[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.2f\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut values: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 5;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [f64; 3]> = aligned::Aligned([0.0; 3]);
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.2f\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 5;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = values[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let mut values: [f64; 3] = [0.0; 3];
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), sum_char_array()) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%.2f\n".as_ptr(), pick_double_array(2)) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn sum_char_array() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 2;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 5;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-REWRITES-NEXT:     values[2] as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pick_double_array({{arg[0-9]+}}: i32) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 1.25;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = 2.5;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-REWRITES-NEXT:     values[({{__v[0-9]+}} as usize)] = values[0] + values[1];
// COMMON-REWRITES-NEXT:     values[(({{arg[0-9]+}} as i64) as usize)]
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut values: [i8; 3] = [0; 3];
// REWRITES-X86_64-GNU-NEXT:     values[({{__v[0-9]+}} as usize)] = ((values[0] as i32) + (values[1] as i32)) as i8;
// REWRITES-X86_64-GNU-NEXT:     let mut values: aligned::Aligned<aligned::A16, [f64; 3]> = aligned::Aligned([0.0; 3]);
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [u8; 3] = [0; 3];
// REWRITES-AARCH64-GNU-NEXT:     values[({{__v[0-9]+}} as usize)] = ((values[0] as i32) + (values[1] as i32)) as u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut values: [f64; 3] = [0.0; 3];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
