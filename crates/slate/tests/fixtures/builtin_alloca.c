#include <stdio.h>
#include <string.h>

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
// COMMON-LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = const_alloca();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = dyn_alloca({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 12;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = uninitialized_alloca({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn const_alloca() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{__v[0-9]+}} as usize];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 15;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(15) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn dyn_alloca({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{__v[0-9]+}} as usize];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn uninitialized_alloca({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{__v[0-9]+}} as usize];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as i32, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
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
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             const_alloca(),
// COMMON-REWRITES-NEXT:             dyn_alloca(10),
// COMMON-REWRITES-NEXT:             uninitialized_alloca(12),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn const_alloca() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-REWRITES-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; {{__v[0-9]+}} as usize];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (3 as i32) as u8, (16 as u64) as usize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(15) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn dyn_alloca({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; ({{arg[0-9]+}} as u64) as usize];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (7 as i32) as u8, ({{arg[0-9]+}} as u64) as usize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = ({{arg[0-9]+}} - 1) as i64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn uninitialized_alloca({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut bi_alloca: Vec<u8> = vec![0; ({{arg[0-9]+}} as u64) as usize];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = bi_alloca.as_mut_ptr() as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_bytes({{__v[0-9]+}} as *mut u8, (9 as i32) as u8, ({{arg[0-9]+}} as u64) as usize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = ({{arg[0-9]+}} - 1) as i64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + ((unsafe { *{{__v[0-9]+}} }) as i32)
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
