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
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn free_sized(_0: *mut core::ffi::c_void, _1: usize);
// COMMON-LOWERING-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn free_aligned_sized(_0: *mut core::ffi::c_void, _1: usize, _2: usize);
// COMMON-LOWERING-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     unsafe { free_sized({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { aligned_alloc({{__v[0-9]+}} as usize, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     unsafe { free_aligned_sized({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { realloc({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { aligned_alloc({{__v[0-9]+}} as usize, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = memalignment({{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn memalignment({{arg[0-9]+}}: *mut core::ffi::c_void) -> u64 {
// COMMON-LOWERING-NEXT:     let mut v: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut align: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     v = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     align = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = v;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = v;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} & {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:             } else {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:             };
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = v;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} >> {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 v = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = align;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                 align = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = v;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = align;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ok\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn free_sized(_0: *mut core::ffi::c_void, _1: usize);
// COMMON-REWRITES-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn free_aligned_sized(_0: *mut core::ffi::c_void, _1: usize, _2: usize);
// COMMON-REWRITES-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         free_sized(
// COMMON-REWRITES-NEXT:             (unsafe { malloc((16 as u64) as usize) }) as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (16 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         free_aligned_sized(
// COMMON-REWRITES-NEXT:             (unsafe { aligned_alloc((16 as u64) as usize, (32 as u64) as usize) })
// COMMON-REWRITES-NEXT:                 as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (16 as u64) as usize,
// COMMON-REWRITES-NEXT:             (32 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         free(
// COMMON-REWRITES-NEXT:             (unsafe { realloc({{__v[0-9]+}} as *mut core::ffi::c_void, (0 as u64) as usize) })
// COMMON-REWRITES-NEXT:                 as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-REWRITES-NEXT:         unsafe { aligned_alloc((64 as u64) as usize, (64 as u64) as usize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = memalignment({{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, ({{__v[0-9]+}} % 64 == 0) as i32) };
// COMMON-REWRITES-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"ok\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn memalignment({{arg[0-9]+}}: *mut core::ffi::c_void) -> u64 {
// COMMON-REWRITES-NEXT:     let mut v: u64 = {{arg[0-9]+}} as u64;
// COMMON-REWRITES-NEXT:     let mut align: u64 = 0;
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = if v != 0 {
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: bool = v & 1 == 0;
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         if !{{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:         v >>= {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         align += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = if v == 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = 1;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} << align;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
