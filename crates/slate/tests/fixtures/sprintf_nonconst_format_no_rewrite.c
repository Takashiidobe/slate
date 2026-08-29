#include <stdio.h>

int main(void) {
  char        buf[64];
  const char *fmt = "%d-%d";
  sprintf(buf, fmt, 3, 4);
  puts(buf);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn sprintf(_0: *mut i8, _1: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn puts(_0: *const i8) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-NEXT:     let mut fmt: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d-%d\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     fmt = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut i8 = fmt;
// LOWERING-NEXT:     let _v4: i32 = 3;
// LOWERING-NEXT:     let _v5: i32 = 4;
// LOWERING-NEXT:     let _v6: i32 = unsafe { sprintf(_v2 as *mut i8, _v3 as *const i8, _v4, _v5) };
// LOWERING-NEXT:     let _v7: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: i32 = unsafe { puts(_v7 as *const i8) };
// LOWERING-NEXT:     let _v9: i32 = 0;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn sprintf(_0: *mut i8, _1: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn puts(_0: *const i8) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut buf: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT: let mut fmt: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: fmt = b"%d-%d\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = 3;
// REWRITES-NEXT: let _v5: i32 = 4;
// REWRITES-NEXT: let _v6: i32 = unsafe { sprintf(_v2 as *mut i8, fmt as *const i8, _v4, _v5) };
// REWRITES-NEXT: let _v7: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { puts(_v7 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
