#include <stdio.h>

typedef int (*Callback)(int);

static int add_one(int x) { return x + 1; }

static void *store_fn(void *fn) { return fn; }

int main(void) {
  void    *slot = (void *)add_one;
  Callback cb   = (Callback)slot;

  void    *slot2 = store_fn((void *)add_one);
  Callback cb2   = (Callback)slot2;

  printf("%d %d\n", cb(41), cb2(99));
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
// LOWERING-NEXT: extern "C" fn add_one({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn store_fn({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<Option<unsafe extern "C" fn(i32) -> i32>, *mut core::ffi::c_void>(
// LOWERING-NEXT:             Some(add_one),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>({{_v[0-9]+}} as usize)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<Option<unsafe extern "C" fn(i32) -> i32>, *mut core::ffi::c_void>(
// LOWERING-NEXT:             Some(add_one),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = store_fn({{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>({{_v[0-9]+}} as usize)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
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
// REWRITES-NEXT: extern "C" fn add_one({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + 1
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn store_fn({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// REWRITES-NEXT:     {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<Option<unsafe extern "C" fn(i32) -> i32>, *mut core::ffi::c_void>(
// REWRITES-NEXT:             Some(add_one),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>({{_v[0-9]+}} as usize)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<Option<unsafe extern "C" fn(i32) -> i32>, *mut core::ffi::c_void>(
// REWRITES-NEXT:             Some(add_one),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = store_fn({{_v[0-9]+}} as *mut core::ffi::c_void);
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<usize, Option<unsafe extern "C" fn(i32) -> i32>>({{_v[0-9]+}} as usize)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// REWRITES-NEXT:             unsafe { {{_v[0-9]+}}.unwrap()(41 as i32) },
// REWRITES-NEXT:             unsafe { {{_v[0-9]+}}.unwrap()(99 as i32) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
