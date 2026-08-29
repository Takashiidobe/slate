#include <ctype.h>
#include <stdio.h>

static int next_lower(void) {
  static int c = 'a';
  return c++;
}

int main(void) {
  int upper = 'Q';
  int lower = 'q';
  int digit = '5';
  int punct = '!';
  int eof   = EOF;

  printf("%d %d %d %d %d\n", toupper(lower), toupper(digit), toupper(punct),
         toupper(upper), toupper(eof));
  printf("%d %d %d %d %d\n", tolower(upper), tolower(digit), tolower(punct),
         tolower(lower), tolower(eof));
  printf("%d %d\n", toupper(next_lower()), tolower(next_lower()));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut next_lower_c: i32 = 97;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn toupper(_0: i32) -> i32;
// LOWERING-NEXT:     fn tolower(_0: i32) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn next_lower() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = unsafe { next_lower_c };
// LOWERING-NEXT:     let _v1: i32 = _v0 + 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         next_lower_c = _v1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v2: i32 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut upper: i32 = 0;
// LOWERING-NEXT:     let mut lower: i32 = 0;
// LOWERING-NEXT:     let mut digit: i32 = 0;
// LOWERING-NEXT:     let mut punct: i32 = 0;
// LOWERING-NEXT:     let mut eof: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 81;
// LOWERING-NEXT:     upper = _v1;
// LOWERING-NEXT:     let _v2: i32 = 113;
// LOWERING-NEXT:     lower = _v2;
// LOWERING-NEXT:     let _v3: i32 = 53;
// LOWERING-NEXT:     digit = _v3;
// LOWERING-NEXT:     let _v4: i32 = 33;
// LOWERING-NEXT:     punct = _v4;
// LOWERING-NEXT:     let _v5: i32 = -1;
// LOWERING-NEXT:     eof = _v5;
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = lower;
// LOWERING-NEXT:     let _v8: i32 = unsafe { toupper(_v7 as i32) };
// LOWERING-NEXT:     let _v9: i32 = digit;
// LOWERING-NEXT:     let _v10: i32 = unsafe { toupper(_v9 as i32) };
// LOWERING-NEXT:     let _v11: i32 = punct;
// LOWERING-NEXT:     let _v12: i32 = unsafe { toupper(_v11 as i32) };
// LOWERING-NEXT:     let _v13: i32 = upper;
// LOWERING-NEXT:     let _v14: i32 = unsafe { toupper(_v13 as i32) };
// LOWERING-NEXT:     let _v15: i32 = eof;
// LOWERING-NEXT:     let _v16: i32 = unsafe { toupper(_v15 as i32) };
// LOWERING-NEXT:     let _v17: i32 = unsafe { printf(_v6 as *const i8, _v8, _v10, _v12, _v14, _v16) };
// LOWERING-NEXT:     let _v18: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: i32 = upper;
// LOWERING-NEXT:     let _v20: i32 = unsafe { tolower(_v19 as i32) };
// LOWERING-NEXT:     let _v21: i32 = digit;
// LOWERING-NEXT:     let _v22: i32 = unsafe { tolower(_v21 as i32) };
// LOWERING-NEXT:     let _v23: i32 = punct;
// LOWERING-NEXT:     let _v24: i32 = unsafe { tolower(_v23 as i32) };
// LOWERING-NEXT:     let _v25: i32 = lower;
// LOWERING-NEXT:     let _v26: i32 = unsafe { tolower(_v25 as i32) };
// LOWERING-NEXT:     let _v27: i32 = eof;
// LOWERING-NEXT:     let _v28: i32 = unsafe { tolower(_v27 as i32) };
// LOWERING-NEXT:     let _v29: i32 = unsafe { printf(_v18 as *const i8, _v20, _v22, _v24, _v26, _v28) };
// LOWERING-NEXT:     let _v30: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v31: i32 = next_lower();
// LOWERING-NEXT:     let _v32: i32 = unsafe { toupper(_v31 as i32) };
// LOWERING-NEXT:     let _v33: i32 = next_lower();
// LOWERING-NEXT:     let _v34: i32 = unsafe { tolower(_v33 as i32) };
// LOWERING-NEXT:     let _v35: i32 = unsafe { printf(_v30 as *const i8, _v32, _v34) };
// LOWERING-NEXT:     let _v36: i32 = 0;
// LOWERING-NEXT:     __retval = _v36;
// LOWERING-NEXT:     let _v37: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v37 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut next_lower_c: i32 = 97;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn toupper(_0: i32) -> i32;
// REWRITES-NEXT:     fn tolower(_0: i32) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn next_lower() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v0: i32 = unsafe { next_lower_c };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         next_lower_c = _v0 + 1;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = _v0;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut upper: i32 = 0;
// REWRITES-NEXT: let mut lower: i32 = 0;
// REWRITES-NEXT: let mut digit: i32 = 0;
// REWRITES-NEXT: let mut punct: i32 = 0;
// REWRITES-NEXT: let mut eof: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: upper = 81;
// REWRITES-NEXT: lower = 113;
// REWRITES-NEXT: digit = 53;
// REWRITES-NEXT: punct = 33;
// REWRITES-NEXT: eof = -1;
// REWRITES-NEXT: let _v6: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { toupper(lower as i32) };
// REWRITES-NEXT: let _v10: i32 = unsafe { toupper(digit as i32) };
// REWRITES-NEXT: let _v12: i32 = unsafe { toupper(punct as i32) };
// REWRITES-NEXT: let _v14: i32 = unsafe { toupper(upper as i32) };
// REWRITES-NEXT: let _v16: i32 = unsafe { toupper(eof as i32) };
// REWRITES-NEXT: let _v17: i32 = unsafe { printf(_v6 as *const i8, _v8, _v10, _v12, _v14, _v16) };
// REWRITES-NEXT: let _v18: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: i32 = unsafe { tolower(upper as i32) };
// REWRITES-NEXT: let _v22: i32 = unsafe { tolower(digit as i32) };
// REWRITES-NEXT: let _v24: i32 = unsafe { tolower(punct as i32) };
// REWRITES-NEXT: let _v26: i32 = unsafe { tolower(lower as i32) };
// REWRITES-NEXT: let _v28: i32 = unsafe { tolower(eof as i32) };
// REWRITES-NEXT: let _v29: i32 = unsafe { printf(_v18 as *const i8, _v20, _v22, _v24, _v26, _v28) };
// REWRITES-NEXT: let _v30: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v31: i32 = next_lower();
// REWRITES-NEXT: let _v32: i32 = unsafe { toupper(_v31 as i32) };
// REWRITES-NEXT: let _v33: i32 = next_lower();
// REWRITES-NEXT: let _v34: i32 = unsafe { tolower(_v33 as i32) };
// REWRITES-NEXT: let _v35: i32 = unsafe { printf(_v30 as *const i8, _v32, _v34) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
