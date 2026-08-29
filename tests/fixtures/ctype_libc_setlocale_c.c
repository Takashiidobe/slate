#include <ctype.h>
#include <locale.h>
#include <stdio.h>

int main(void) {
  setlocale(LC_ALL, "C");
  int lower = 'q';
  int upper = 'Q';
  printf("%d %d\n", toupper(lower), tolower(upper));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn setlocale(_0: i32, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn toupper(_0: i32) -> i32;
// LOWERING-NEXT:     fn tolower(_0: i32) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut lower: i32 = 0;
// LOWERING-NEXT:     let mut upper: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 6;
// LOWERING-NEXT:     let _v2: *mut i8 = b"C\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut i8 = unsafe { setlocale(_v1 as i32, _v2 as *const i8) };
// LOWERING-NEXT:     let _v4: i32 = 113;
// LOWERING-NEXT:     lower = _v4;
// LOWERING-NEXT:     let _v5: i32 = 81;
// LOWERING-NEXT:     upper = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = lower;
// LOWERING-NEXT:     let _v8: i32 = unsafe { toupper(_v7 as i32) };
// LOWERING-NEXT:     let _v9: i32 = upper;
// LOWERING-NEXT:     let _v10: i32 = unsafe { tolower(_v9 as i32) };
// LOWERING-NEXT:     let _v11: i32 = unsafe { printf(_v6 as *const i8, _v8, _v10) };
// LOWERING-NEXT:     let _v12: i32 = 0;
// LOWERING-NEXT:     __retval = _v12;
// LOWERING-NEXT:     let _v13: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v13 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn setlocale(_0: i32, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn toupper(_0: i32) -> i32;
// REWRITES-NEXT:     fn tolower(_0: i32) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut lower: i32 = 0;
// REWRITES-NEXT: let mut upper: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i32 = 6;
// REWRITES-NEXT: let _v2: *mut i8 = b"C\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: *mut i8 = unsafe { setlocale(_v1 as i32, _v2 as *const i8) };
// REWRITES-NEXT: lower = 113;
// REWRITES-NEXT: upper = 81;
// REWRITES-NEXT: let _v6: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { toupper(lower as i32) };
// REWRITES-NEXT: let _v10: i32 = unsafe { tolower(upper as i32) };
// REWRITES-NEXT: let _v11: i32 = unsafe { printf(_v6 as *const i8, _v8, _v10) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
