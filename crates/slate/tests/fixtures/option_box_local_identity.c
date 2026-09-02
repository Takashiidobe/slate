#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  int *q = NULL;
  if (flag) {
    p = malloc(sizeof(int));
    q = malloc(sizeof(int));
  }
  if (p == q) {
    printf("same\n");
  } else {
    printf("diff\n");
  }
  if (p) {
    *p = 1;
    printf("%d\n", *p);
    free(p);
  }
  if (q) {
    *q = 2;
    printf("%d\n", *q);
    free(q);
  }
  return 0;
}

int main(void) {
  compute(1);
  compute(0);
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
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn compute({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut flag: i32 = 0;
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut q: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     flag = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     q = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = flag;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:             p = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:             q = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = q;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"same\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"diff\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:             unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = q;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = q;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = q;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = q;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:             unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = compute({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = compute({{_v[0-9]+}});
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
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn compute(mut {{_v[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:     let mut q: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc((4 as u64) as usize) };
// REWRITES-NEXT:         p = {{_v[0-9]+}} as *mut i32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc((4 as u64) as usize) };
// REWRITES-NEXT:         q = {{_v[0-9]+}} as *mut i32;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = p == q;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { printf(c"same\n".as_ptr()) };
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         unsafe { printf(c"diff\n".as_ptr()) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = p != std::ptr::null_mut();
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *p = 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         unsafe { printf(c"%d\n".as_ptr(), unsafe { *p }) };
// REWRITES-NEXT:         unsafe { free(p as *mut core::ffi::c_void) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = q != std::ptr::null_mut();
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *q = 2;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         unsafe { printf(c"%d\n".as_ptr(), unsafe { *q }) };
// REWRITES-NEXT:         unsafe { free(q as *mut core::ffi::c_void) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return 0;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     compute(1);
// REWRITES-NEXT:     compute(0);
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
