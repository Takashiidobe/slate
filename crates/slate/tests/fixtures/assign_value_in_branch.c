#include <stdio.h>

unsigned long bump(unsigned long *p, int c) {
  return c ? (*p += 2) : (*p += 1);
}

int main() {
  unsigned long x = 10;
  unsigned long a = bump(&x, 1);
  unsigned long b = bump(&x, 0);
  printf("%lu %lu %lu\n", a, b, x);
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
// LOWERING-NEXT: unsafe fn bump({{arg[0-9]+}}: *mut u64, {{arg[0-9]+}}: i32) -> u64 {
// LOWERING-NEXT:     let mut p: *mut u64 = std::ptr::null_mut();
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut u64 = p;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut u64 = p;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut x: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 10;
// LOWERING-NEXT:     x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { bump(std::ptr::addr_of_mut!(x), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { bump(std::ptr::addr_of_mut!(x), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%lu %lu %lu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: unsafe fn bump(mut p: *mut u64, {{arg[0-9]+}}: i32) -> u64 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = if {{arg[0-9]+}} != 0 {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut u64 = p;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { *{{_v[0-9]+}} }) + 2;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut u64 = p;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { *{{_v[0-9]+}} }) + 1;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut x: u64 = 10;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%lu %lu %lu\n".as_ptr(),
// REWRITES-NEXT:             unsafe { bump(std::ptr::addr_of_mut!(x), 1) },
// REWRITES-NEXT:             unsafe { bump(std::ptr::addr_of_mut!(x), 0) },
// REWRITES-NEXT:             x,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
