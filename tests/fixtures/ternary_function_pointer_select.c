#include <stdio.h>

typedef int (*BinaryOp)(int, int);

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

static int apply(int useAdd, int a, int b) {
  return (useAdd ? add : sub)(a, b);
}

int main(void) {
  int      useAdd = 1;
  BinaryOp op     = useAdd ? add : sub;
  printf("%d %d %d %d\n", op(10, 3), apply(0, 10, 3), apply(1, 4, 4),
         (useAdd ? sub : add)(9, 2));
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
// LOWERING-NEXT: extern "C" fn add(arg5: i32, arg6: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     a = arg5;
// LOWERING-NEXT:     b = arg6;
// LOWERING-NEXT:     let _v0: i32 = a;
// LOWERING-NEXT:     let _v1: i32 = b;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn sub(arg3: i32, arg4: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     a = arg3;
// LOWERING-NEXT:     b = arg4;
// LOWERING-NEXT:     let _v0: i32 = a;
// LOWERING-NEXT:     let _v1: i32 = b;
// LOWERING-NEXT:     let _v2: i32 = _v0 - _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn apply(arg0: i32, arg1: i32, arg2: i32) -> i32 {
// LOWERING-NEXT:     let mut useAdd: i32 = 0;
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     useAdd = arg0;
// LOWERING-NEXT:     a = arg1;
// LOWERING-NEXT:     b = arg2;
// LOWERING-NEXT:     let _v0: i32 = useAdd;
// LOWERING-NEXT:     let _v1: bool = _v0 != 0;
// LOWERING-NEXT:     let _v2: Option<unsafe extern "C" fn(i32, i32) -> i32> = if _v1 { Some(add) } else { Some(sub) };
// LOWERING-NEXT:     let _v3: i32 = a;
// LOWERING-NEXT:     let _v4: i32 = b;
// LOWERING-NEXT:     let _v5: i32 = unsafe { _v2.unwrap()(_v3, _v4) };
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut useAdd: i32 = 0;
// LOWERING-NEXT:     let mut op: Option<unsafe extern "C" fn(i32, i32) -> i32> = None;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     useAdd = _v1;
// LOWERING-NEXT:     let _v2: i32 = useAdd;
// LOWERING-NEXT:     let _v3: bool = _v2 != 0;
// LOWERING-NEXT:     let _v4: Option<unsafe extern "C" fn(i32, i32) -> i32> = if _v3 { Some(add) } else { Some(sub) };
// LOWERING-NEXT:     op = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: Option<unsafe extern "C" fn(i32, i32) -> i32> = op;
// LOWERING-NEXT:     let _v7: i32 = 10;
// LOWERING-NEXT:     let _v8: i32 = 3;
// LOWERING-NEXT:     let _v9: i32 = unsafe { _v6.unwrap()(_v7, _v8) };
// LOWERING-NEXT:     let _v10: i32 = 0;
// LOWERING-NEXT:     let _v11: i32 = 10;
// LOWERING-NEXT:     let _v12: i32 = 3;
// LOWERING-NEXT:     let _v13: i32 = apply(_v10, _v11, _v12);
// LOWERING-NEXT:     let _v14: i32 = 1;
// LOWERING-NEXT:     let _v15: i32 = 4;
// LOWERING-NEXT:     let _v16: i32 = 4;
// LOWERING-NEXT:     let _v17: i32 = apply(_v14, _v15, _v16);
// LOWERING-NEXT:     let _v18: i32 = useAdd;
// LOWERING-NEXT:     let _v19: bool = _v18 != 0;
// LOWERING-NEXT:     let _v20: Option<unsafe extern "C" fn(i32, i32) -> i32> = if _v19 { Some(sub) } else { Some(add) };
// LOWERING-NEXT:     let _v21: i32 = 9;
// LOWERING-NEXT:     let _v22: i32 = 2;
// LOWERING-NEXT:     let _v23: i32 = unsafe { _v20.unwrap()(_v21, _v22) };
// LOWERING-NEXT:     let _v24: i32 = unsafe { printf(_v5 as *const i8, _v9, _v13, _v17, _v23) };
// LOWERING-NEXT:     let _v25: i32 = 0;
// LOWERING-NEXT:     __retval = _v25;
// LOWERING-NEXT:     let _v26: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v26 as i32);
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
// REWRITES-NEXT: extern "C" fn add(arg5: i32, arg6: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg5;
// REWRITES-NEXT: let mut b: i32 = arg6;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = a + b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn sub(arg3: i32, arg4: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg3;
// REWRITES-NEXT: let mut b: i32 = arg4;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = a - b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn apply(arg0: i32, arg1: i32, arg2: i32) -> i32 {
// REWRITES-NEXT: let mut useAdd: i32 = arg0;
// REWRITES-NEXT: let mut a: i32 = arg1;
// REWRITES-NEXT: let mut b: i32 = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: bool = useAdd != 0;
// REWRITES-NEXT: let _v2: Option<unsafe extern "C" fn(i32, i32) -> i32> = if _v1 { Some(add) } else { Some(sub) };
// REWRITES-NEXT: __retval = unsafe { _v2.unwrap()(a, b) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut useAdd: i32 = 0;
// REWRITES-NEXT: let mut op: Option<unsafe extern "C" fn(i32, i32) -> i32> = None;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: useAdd = 1;
// REWRITES-NEXT: let _v3: bool = useAdd != 0;
// REWRITES-NEXT: op = if _v3 { Some(add) } else { Some(sub) };
// REWRITES-NEXT: let _v5: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = 10;
// REWRITES-NEXT: let _v8: i32 = 3;
// REWRITES-NEXT: let _v9: i32 = unsafe { op.unwrap()(_v7, _v8) };
// REWRITES-NEXT: let _v10: i32 = 0;
// REWRITES-NEXT: let _v11: i32 = 10;
// REWRITES-NEXT: let _v12: i32 = 3;
// REWRITES-NEXT: let _v13: i32 = apply(_v10, _v11, _v12);
// REWRITES-NEXT: let _v14: i32 = 1;
// REWRITES-NEXT: let _v15: i32 = 4;
// REWRITES-NEXT: let _v16: i32 = 4;
// REWRITES-NEXT: let _v17: i32 = apply(_v14, _v15, _v16);
// REWRITES-NEXT: let _v19: bool = useAdd != 0;
// REWRITES-NEXT: let _v20: Option<unsafe extern "C" fn(i32, i32) -> i32> = if _v19 { Some(sub) } else { Some(add) };
// REWRITES-NEXT: let _v21: i32 = 9;
// REWRITES-NEXT: let _v22: i32 = 2;
// REWRITES-NEXT: let _v23: i32 = unsafe { _v20.unwrap()(_v21, _v22) };
// REWRITES-NEXT: let _v24: i32 = unsafe { printf(_v5 as *const i8, _v9, _v13, _v17, _v23) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
