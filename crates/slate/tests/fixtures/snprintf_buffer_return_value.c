#include <stdio.h>

int main(void) {
  char buf[64];
  int  n = snprintf(buf, sizeof(buf), "%d-%d", 3, 4);
  printf("n=%d buf=%s\n", n, buf);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn snprintf(_0: *mut i8, _1: usize, _2: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: u64 = 64;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%d-%d\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: i32 = 3;
// LOWERING-NEXT:     let _v5: i32 = 4;
// LOWERING-NEXT:     let _v6: i32 = unsafe { snprintf(_v1 as *mut i8, _v2 as usize, _v3 as *const i8, _v4, _v5) };
// LOWERING-NEXT:     n = _v6;
// LOWERING-NEXT:     let _v7: *mut i8 = b"n=%d buf=%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: i32 = n;
// LOWERING-NEXT:     let _v9: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v7 as *const i8, _v8, _v9) };
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v12 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn snprintf(_0: *mut i8, _1: usize, _2: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut buf: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT: let mut n: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: u64 = 64;
// REWRITES-NEXT: let _v3: *mut i8 = b"%d-%d\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = 3;
// REWRITES-NEXT: let _v5: i32 = 4;
// REWRITES-NEXT: n = unsafe { snprintf(_v1 as *mut i8, _v2 as usize, _v3 as *const i8, _v4, _v5) };
// REWRITES-NEXT: let _v7: *mut i8 = b"n=%d buf=%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = n;
// REWRITES-NEXT: let _v9: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v7 as *const i8, _v8, _v9) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
