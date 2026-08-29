#include <stdio.h>

int main(void) {
  int rc = remove("slate_perror_call_guard_missing.tmp");
  if (rc < 0) {
    perror("remove failed");
    return 1;
  }
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn remove(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn perror(_0: *const i8);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut rc: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"slate_perror_call_guard_missing.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { remove(_v1 as *const i8) };
// LOWERING-NEXT:     rc = _v2;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v3: i32 = rc;
// LOWERING-NEXT:         let _v4: i32 = 0;
// LOWERING-NEXT:         let _v5: bool = _v3 < _v4;
// LOWERING-NEXT:         if _v5 {
// LOWERING-NEXT:             let _v6: *mut i8 = b"remove failed\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             unsafe { perror(_v6 as *const i8) };
// LOWERING-NEXT:             let _v7: i32 = 1;
// LOWERING-NEXT:             __retval = _v7;
// LOWERING-NEXT:             let _v8: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v8 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v9: i32 = 0;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn remove(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn perror(_0: *const i8);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut rc: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"slate_perror_call_guard_missing.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: rc = unsafe { remove(_v1 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v4: i32 = 0;
// REWRITES-NEXT:         let _v5: bool = rc < _v4;
// REWRITES-NEXT:         if _v5 {
// REWRITES-NEXT:                     let _v6: *mut i8 = b"remove failed\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     unsafe { perror(_v6 as *const i8) };
// REWRITES-NEXT:                     __retval = 1;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
