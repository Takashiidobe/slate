#include <stdio.h>

static int add(int a, int b) {
  int c = a + b;
  return c;
}

int main(void) {
  printf("%d\n", add(2, 3));
  printf("%d\n", add(-10, 4));
  printf("%d\n", add(0, 0));
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
// LOWERING-NEXT: fn add(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut c: i32 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: i32 = a;
// LOWERING-NEXT:     let _v1: i32 = b;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     c = _v2;
// LOWERING-NEXT:     let _v3: i32 = c;
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: i32 = __retval;
// LOWERING-NEXT:     return _v4;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 2;
// LOWERING-NEXT:     let _v3: i32 = 3;
// LOWERING-NEXT:     let _v4: i32 = add(_v2, _v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = -10;
// LOWERING-NEXT:     let _v8: i32 = 4;
// LOWERING-NEXT:     let _v9: i32 = add(_v7, _v8);
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: i32 = 0;
// LOWERING-NEXT:     let _v13: i32 = 0;
// LOWERING-NEXT:     let _v14: i32 = add(_v12, _v13);
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v11 as *const i8, _v14) };
// LOWERING-NEXT:     let _v16: i32 = 0;
// LOWERING-NEXT:     __retval = _v16;
// LOWERING-NEXT:     let _v17: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v17 as i32);
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
// REWRITES-NEXT: fn add(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg0;
// REWRITES-NEXT: let mut b: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut c: i32 = 0;
// REWRITES-NEXT: c = a + b;
// REWRITES-NEXT: __retval = c;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 2;
// REWRITES-NEXT: let _v3: i32 = 3;
// REWRITES-NEXT: let _v4: i32 = add(_v2, _v3);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = -10;
// REWRITES-NEXT: let _v8: i32 = 4;
// REWRITES-NEXT: let _v9: i32 = add(_v7, _v8);
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v6 as *const i8, _v9) };
// REWRITES-NEXT: let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = 0;
// REWRITES-NEXT: let _v13: i32 = 0;
// REWRITES-NEXT: let _v14: i32 = add(_v12, _v13);
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v11 as *const i8, _v14) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites

// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-NOT: let _v1: i32 = 2;
// REWRITES-NOT: std::process::exit(0 as i32);
// REWRITES: {{^}}}
