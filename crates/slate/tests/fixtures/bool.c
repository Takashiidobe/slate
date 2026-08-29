#include <stdbool.h>
#include <stdio.h>

static bool from_int(int x) {
  bool b = x;
  return b;
}

static _Bool from_compare(int x, int y) {
  _Bool b = x < y;
  return b;
}

static int use_bool(_Bool flag) { return flag; }

int main(void) {
  printf("%d\n", from_int(0));
  printf("%d\n", from_int(42));
  printf("%d\n", from_compare(2, 5));
  printf("%d\n", from_compare(9, 5));
  printf("%d\n", use_bool(2));
  printf("%d\n", use_bool(0));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn from_int(arg3: i32) -> bool {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: bool = false;
// LOWERING-NEXT:     let mut b: bool = false;
// LOWERING-NEXT:     x = arg3;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: bool = _v0 != 0;
// LOWERING-NEXT:     b = _v1;
// LOWERING-NEXT:     let _v2: bool = b;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: bool = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn from_compare(arg1: i32, arg2: i32) -> bool {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut y: i32 = 0;
// LOWERING-NEXT:     let mut __retval: bool = false;
// LOWERING-NEXT:     let mut b: bool = false;
// LOWERING-NEXT:     x = arg1;
// LOWERING-NEXT:     y = arg2;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = y;
// LOWERING-NEXT:     let _v2: bool = _v0 < _v1;
// LOWERING-NEXT:     b = _v2;
// LOWERING-NEXT:     let _v3: bool = b;
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: bool = __retval;
// LOWERING-NEXT:     return _v4;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn use_bool(arg0: bool) -> i32 {
// LOWERING-NEXT:     let mut flag: bool = false;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     flag = arg0;
// LOWERING-NEXT:     let _v0: bool = flag;
// LOWERING-NEXT:     let _v1: i32 = _v0 as i32;
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: i32 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     let _v3: bool = from_int(_v2);
// LOWERING-NEXT:     let _v4: i32 = _v3 as i32;
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = 42;
// LOWERING-NEXT:     let _v8: bool = from_int(_v7);
// LOWERING-NEXT:     let _v9: i32 = _v8 as i32;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: i32 = 2;
// LOWERING-NEXT:     let _v13: i32 = 5;
// LOWERING-NEXT:     let _v14: bool = from_compare(_v12, _v13);
// LOWERING-NEXT:     let _v15: i32 = _v14 as i32;
// LOWERING-NEXT:     let _v16: i32 = unsafe { printf(_v11 as *const i8, _v15) };
// LOWERING-NEXT:     let _v17: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v18: i32 = 9;
// LOWERING-NEXT:     let _v19: i32 = 5;
// LOWERING-NEXT:     let _v20: bool = from_compare(_v18, _v19);
// LOWERING-NEXT:     let _v21: i32 = _v20 as i32;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v17 as *const i8, _v21) };
// LOWERING-NEXT:     let _v23: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: i32 = 2;
// LOWERING-NEXT:     let _v25: bool = _v24 != 0;
// LOWERING-NEXT:     let _v26: i32 = use_bool(_v25);
// LOWERING-NEXT:     let _v27: i32 = unsafe { printf(_v23 as *const i8, _v26) };
// LOWERING-NEXT:     let _v28: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v29: i32 = 0;
// LOWERING-NEXT:     let _v30: bool = _v29 != 0;
// LOWERING-NEXT:     let _v31: i32 = use_bool(_v30);
// LOWERING-NEXT:     let _v32: i32 = unsafe { printf(_v28 as *const i8, _v31) };
// LOWERING-NEXT:     let _v33: i32 = 0;
// LOWERING-NEXT:     __retval = _v33;
// LOWERING-NEXT:     let _v34: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v34 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn from_int(arg3: i32) -> bool {
// REWRITES-NEXT: let mut x: i32 = arg3;
// REWRITES-NEXT: let mut __retval: bool = false;
// REWRITES-NEXT: let mut b: bool = false;
// REWRITES-NEXT: b = x != 0;
// REWRITES-NEXT: __retval = b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn from_compare(arg1: i32, arg2: i32) -> bool {
// REWRITES-NEXT: let mut x: i32 = arg1;
// REWRITES-NEXT: let mut y: i32 = arg2;
// REWRITES-NEXT: let mut __retval: bool = false;
// REWRITES-NEXT: let mut b: bool = false;
// REWRITES-NEXT: b = x < y;
// REWRITES-NEXT: __retval = b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn use_bool(arg0: bool) -> i32 {
// REWRITES-NEXT: let mut flag: bool = false;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: flag = arg0;
// REWRITES-NEXT: __retval = flag as i32;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 0;
// REWRITES-NEXT: let _v3: bool = from_int(_v2);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v3 as i32) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = 42;
// REWRITES-NEXT: let _v8: bool = from_int(_v7);
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v6 as *const i8, _v8 as i32) };
// REWRITES-NEXT: let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = 2;
// REWRITES-NEXT: let _v13: i32 = 5;
// REWRITES-NEXT: let _v14: bool = from_compare(_v12, _v13);
// REWRITES-NEXT: let _v16: i32 = unsafe { printf(_v11 as *const i8, _v14 as i32) };
// REWRITES-NEXT: let _v17: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: i32 = 9;
// REWRITES-NEXT: let _v19: i32 = 5;
// REWRITES-NEXT: let _v20: bool = from_compare(_v18, _v19);
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v17 as *const i8, _v20 as i32) };
// REWRITES-NEXT: let _v23: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: i32 = 2;
// REWRITES-NEXT: let _v25: bool = _v24 != 0;
// REWRITES-NEXT: let _v26: i32 = use_bool(_v25);
// REWRITES-NEXT: let _v27: i32 = unsafe { printf(_v23 as *const i8, _v26) };
// REWRITES-NEXT: let _v28: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v29: i32 = 0;
// REWRITES-NEXT: let _v30: bool = _v29 != 0;
// REWRITES-NEXT: let _v31: i32 = use_bool(_v30);
// REWRITES-NEXT: let _v32: i32 = unsafe { printf(_v28 as *const i8, _v31) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
