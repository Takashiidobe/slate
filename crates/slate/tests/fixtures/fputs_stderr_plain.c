#include <stdio.h>

int main(void) {
  fputs("bye\n", stderr);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut stderr: *mut libc::FILE;
// LOWERING-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"bye\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut libc::FILE = unsafe { stderr };
// LOWERING-NEXT:     let _v3: i32 = unsafe { fputs(_v1 as *const i8, _v2 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v4: i32 = 0;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v5 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     static mut stderr: *mut libc::FILE;
// REWRITES-NEXT:     fn fputs(_0: *const i8, _1: *mut libc::FILE) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"bye\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = unsafe { fputs(_v1 as *const i8, (unsafe { stderr }) as *mut libc::FILE) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
