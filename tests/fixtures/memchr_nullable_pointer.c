#include <stdio.h>

int main(void) {
  unsigned char  buf[4] = {10, 20, 30, 40};
  unsigned char *hit    = (unsigned char *)__builtin_memchr(buf, 30, 4);
  unsigned char *miss   = (unsigned char *)__builtin_memchr(buf, 99, 4);
  printf("%ld %d\n", hit - buf, miss == 0);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: fn __slate_memchr(s: *const core::ffi::c_void, c: i32, n: usize) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let b: u8 = c as u8;
// LOWERING-NEXT:     let bytes: *const u8 = s as *const u8;
// LOWERING-NEXT:     let mut i: usize = 0;
// LOWERING-NEXT:     while i < n {
// LOWERING-NEXT:         if (unsafe { *bytes.add(i) }) == b {
// LOWERING-NEXT:             return (unsafe { bytes.add(i) }) as *mut core::ffi::c_void;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         i += 1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return std::ptr::null_mut();
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut buf: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     buf = [10, 20, 30, 40];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 30;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{_v[0-9]+}}, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{_v[0-9]+}}, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%ld %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: fn __slate_memchr(s: *const core::ffi::c_void, c: i32, n: usize) -> *mut core::ffi::c_void {
// REWRITES-NEXT: let b: u8 = c as u8;
// REWRITES-NEXT: let bytes: *const u8 = s as *const u8;
// REWRITES-NEXT: let mut i: usize = 0;
// REWRITES-NEXT: while i < n {
// REWRITES-NEXT:         if (unsafe { *bytes.add(i) }) == b {
// REWRITES-NEXT:                     return (unsafe { bytes.add(i) }) as *mut core::ffi::c_void;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         i += 1;
// REWRITES-NEXT: }
// REWRITES-NEXT: return std::ptr::null_mut();
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut buf: [u8; 4] = [10, 20, 30, 40];
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 30;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 99;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = __slate_memchr({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = {{_v[0-9]+}} as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%ld %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, ({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
