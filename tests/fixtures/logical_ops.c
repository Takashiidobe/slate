#include <stdio.h>

static int hits = 0;

static int mark(int value) {
  hits += 1;
  return value;
}

// REWRITES-DAG: println!("{} {}",
// REWRITES-DAG: println!("{} {} {}",

static int logical_and(int a, int b) { return a && mark(b); }

static int logical_or(int a, int b) { return a || mark(b); }

int main(void) {
  hits = 0;
  printf("%d %d\n", logical_and(0, 1), hits);
  hits = 0;
  printf("%d %d\n", logical_and(2, 3), hits);
  hits = 0;
  printf("%d %d\n", logical_or(5, 0), hits);
  hits = 0;
  printf("%d %d\n", logical_or(0, 7), hits);
  printf("%d %d %d\n", !0, !4, !!9);
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
// LOWERING-NEXT: static mut hits: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         hits = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = logical_and({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         hits = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = logical_and({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         hits = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = logical_or({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         hits = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = logical_or({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// REWRITES-DAG: println!("{} {}",
// LOWERING-NEXT: /// REWRITES-DAG: println!("{} {} {}",
// LOWERING-NEXT: fn logical_and({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = b;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = mark({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn logical_or({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = true;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = b;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = mark({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mark({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         hits = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{arg[0-9]+}};
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
// REWRITES-NEXT: static mut hits: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         hits = 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), logical_and(0, 1), unsafe { hits }) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         hits = 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), logical_and(2, 3), unsafe { hits }) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         hits = 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), logical_or(5, 0), unsafe { hits }) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         hits = 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), logical_or(0, 7), unsafe { hits }) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}} as i32,
// REWRITES-NEXT:             (9 != 0) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// REWRITES-DAG: println!("{} {}",
// REWRITES-NEXT: /// REWRITES-DAG: println!("{} {} {}",
// REWRITES-NEXT: fn logical_and({{arg[0-9]+}}: i32, mut b: i32) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = mark(b);
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}} as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn logical_or({{arg[0-9]+}}: i32, mut b: i32) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = mark(b);
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}} as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mark({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         hits = (unsafe { hits }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{arg[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
