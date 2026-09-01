#include <stdio.h>

int main(void) {
  volatile double x = 0x1.fffffffffffffp+0;
  double y = x;
  double z = -(x * x);
  double contracted;
  double uncontracted;

  {
#pragma STDC FP_CONTRACT ON
    contracted = x * y + z;
  }

  {
#pragma STDC FP_CONTRACT OFF
    uncontracted = x * y + z;
  }

  printf("%.20e %.20e\n", contracted, uncontracted);
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
// LOWERING-NEXT:     let mut x: f64 = 0.0;
// LOWERING-NEXT:     let mut y: f64 = 0.0;
// LOWERING-NEXT:     let mut z: f64 = 0.0;
// LOWERING-NEXT:     let mut contracted: f64 = 0.0;
// LOWERING-NEXT:     let mut uncontracted: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.9999999999999998;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     y = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = -{{_v[0-9]+}};
// LOWERING-NEXT:     z = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = y;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = z;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         contracted = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = y;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = z;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         uncontracted = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.20e %.20e\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = contracted;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = uncontracted;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT:     let mut x: f64 = 0.0;
// REWRITES-NEXT:     let mut y: f64 = 0.0;
// REWRITES-NEXT:     let mut z: f64 = 0.0;
// REWRITES-NEXT:     let mut contracted: f64 = 0.0;
// REWRITES-NEXT:     let mut uncontracted: f64 = 0.0;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), 1.9999999999999998 as f64) };
// REWRITES-NEXT:     y = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:     z = -({{_v[0-9]+}} * {{_v[0-9]+}});
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:         contracted = {{_v[0-9]+}} * y + z;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-NEXT:         uncontracted = {{_v[0-9]+}} * y + z;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%.20e %.20e\n".as_ptr(), contracted, uncontracted) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
