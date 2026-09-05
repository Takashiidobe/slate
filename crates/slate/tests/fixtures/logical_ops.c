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
// COMMON-LOWERING-NEXT: static mut hits: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         hits = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = logical_and({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         hits = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = logical_and({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         hits = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = logical_or({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         hits = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = logical_or({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: /// REWRITES-DAG: println!("{} {}",
// COMMON-LOWERING-NEXT: /// REWRITES-DAG: println!("{} {} {}",
// COMMON-LOWERING-NEXT: fn logical_and({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut b: i32 = 0;
// COMMON-LOWERING-NEXT:     b = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = b;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = mark({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn logical_or({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut b: i32 = 0;
// COMMON-LOWERING-NEXT:     b = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = b;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = mark({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn mark({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { hits };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         hits = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return {{arg[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: static mut hits: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         hits = 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), logical_and(0, 1), unsafe { hits }) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         hits = 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), logical_and(2, 3), unsafe { hits }) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         hits = 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), logical_or(5, 0), unsafe { hits }) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         hits = 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), logical_or(0, 7), unsafe { hits }) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-REWRITES-NEXT:             (9 != 0) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: /// REWRITES-DAG: println!("{} {}",
// COMMON-REWRITES-NEXT: /// REWRITES-DAG: println!("{} {} {}",
// COMMON-REWRITES-NEXT: fn logical_and({{arg[0-9]+}}: i32, mut b: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = mark(b);
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn logical_or({{arg[0-9]+}}: i32, mut b: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = mark(b);
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn mark({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         hits = (unsafe { hits }) + 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
