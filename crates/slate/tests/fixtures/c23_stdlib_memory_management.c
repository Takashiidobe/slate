#include <stdio.h>
#include <stdlib.h>

int main(void) {
  void *p = malloc(16);
  free_sized(p, 16);

  void *q = aligned_alloc(16, 32);
  free_aligned_sized(q, 16, 32);

  void *r = realloc(NULL, 0);
  free(r);

  void *a = aligned_alloc(64, 64);
  printf("%d\n", memalignment(a) % 64 == 0);
  free(a);

  printf("ok\n");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free_sized(_0: *mut core::ffi::c_void, _1: usize);
// LOWERING-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free_aligned_sized(_0: *mut core::ffi::c_void, _1: usize, _2: usize);
// LOWERING-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn memalignment({{arg[0-9]+}}: *mut core::ffi::c_void) -> u64 {
// LOWERING-NEXT:     let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: u64 = 0;
// LOWERING-NEXT:     let mut v: u64 = 0;
// LOWERING-NEXT:     let mut align: u64 = 0;
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     v = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     align = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = v;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = v;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                 {{_v[0-9]+}}
// LOWERING-NEXT:             } else {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:                 {{_v[0-9]+}}
// LOWERING-NEXT:             };
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = v;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} >> {{_v[0-9]+}};
// LOWERING-NEXT:                 v = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = align;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                 align = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = v;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = align;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut q: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut r: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut a: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     unsafe { free_sized({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { aligned_alloc({{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     q = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = q;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     unsafe { free_aligned_sized({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { realloc({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     r = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = r;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { aligned_alloc({{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = memalignment({{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = a;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free_sized(_0: *mut core::ffi::c_void, _1: usize);
// REWRITES-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free_aligned_sized(_0: *mut core::ffi::c_void, _1: usize, _2: usize);
// REWRITES-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn memalignment({{arg[0-9]+}}: *mut core::ffi::c_void) -> u64 {
// REWRITES-NEXT: let mut p: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: u64 = 0;
// REWRITES-NEXT: let mut v: u64 = 0;
// REWRITES-NEXT: let mut align: u64 = 0;
// REWRITES-NEXT: v = p as u64;
// REWRITES-NEXT: align = 0;
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = if v != {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: bool = v & {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:             {{_v[0-9]+}}
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:             {{_v[0-9]+}}
// REWRITES-NEXT:         };
// REWRITES-NEXT:         if !{{_v[0-9]+}} {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:                     v = v >> {{_v[0-9]+}};
// REWRITES-NEXT:                     align = align + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = if v == {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} << align;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: __retval = {{_v[0-9]+}};
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut q: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut r: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut a: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: p = unsafe { malloc({{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: unsafe { free_sized(p as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 32;
// REWRITES-NEXT: q = unsafe { aligned_alloc({{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 32;
// REWRITES-NEXT: unsafe { free_aligned_sized(q as *mut core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT: r = unsafe { realloc({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: unsafe { free(r as *mut core::ffi::c_void) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 64;
// REWRITES-NEXT: a = unsafe { aligned_alloc({{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = memalignment(a as *mut core::ffi::c_void);
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, ({{_v[0-9]+}} % {{_v[0-9]+}} == {{_v[0-9]+}}) as i32) };
// REWRITES-NEXT: unsafe { free(a as *mut core::ffi::c_void) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
