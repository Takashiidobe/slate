#include <string.h>
#include <stdio.h>

static int const_alloca(void) {
  unsigned char *buf = __builtin_alloca(16);
  memset(buf, 3, 16);
  return buf[0] + buf[15];
}

static int dyn_alloca(int n) {
  unsigned char *buf = __builtin_alloca(n);
  memset(buf, 7, n);
  return buf[0] + buf[n - 1];
}

static int uninitialized_alloca(int n) {
  unsigned char *buf = __builtin_alloca_uninitialized(n);
  memset(buf, 9, n);
  return buf[0] + buf[n - 1];
}

int main(void) {
  printf("%d %d %d\n", const_alloca(), dyn_alloca(10),
         uninitialized_alloca(12));
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
// LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = const_alloca();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = dyn_alloca({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 12;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = uninitialized_alloca({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn const_alloca() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{_v[0-9]+}} as usize];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 15;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(15) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn dyn_alloca({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{_v[0-9]+}} as usize];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn uninitialized_alloca({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{_v[0-9]+}} as usize];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
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
// REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-NEXT:             const_alloca(),
// REWRITES-NEXT:             dyn_alloca(10),
// REWRITES-NEXT:             uninitialized_alloca(12),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn const_alloca() -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{_v[0-9]+}} as usize];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (3 as i32) as u8, (16 as u64) as usize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (unsafe { *{{_v[0-9]+}} }) as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(15) };
// REWRITES-NEXT:     {{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn dyn_alloca({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; ({{arg[0-9]+}} as u64) as usize];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (7 as i32) as u8, ({{arg[0-9]+}} as u64) as usize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (unsafe { *{{_v[0-9]+}} }) as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = ({{arg[0-9]+}} - 1) as i64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// REWRITES-NEXT:     {{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn uninitialized_alloca({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; ({{arg[0-9]+}} as u64) as usize];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (9 as i32) as u8, ({{arg[0-9]+}} as u64) as usize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (unsafe { *{{_v[0-9]+}} }) as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = ({{arg[0-9]+}} - 1) as i64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// REWRITES-NEXT:     {{_v[0-9]+}} + ((unsafe { *{{_v[0-9]+}} }) as i32)
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
