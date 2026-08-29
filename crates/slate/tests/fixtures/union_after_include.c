#include <stdio.h>
enum FuzzEnum {
  FuzzZero,
  FuzzOne,
  FuzzFive = 5,
  FuzzSix,
  FuzzNegative = -2,
  FuzzNegativeNext
};
union FuzzPair {
  int left;
  int right;
};
static int helper(int a, int b) {
  int c = a + b;
  return c;
}
static int loop_sum(int n) {
  int total = 0;
  for (int i = 1; i <= n; i++) {
    total += i;
  }
  return total;
}
static int array_pick(int index) {
  int values[3];
  values[0] = 6;
  values[1] = 1;
  values[2] = values[0] + values[1];
  return values[index];
}
static int union_pick(int a, int b) {
  union FuzzPair p;
  p.left  = a;
  p.right = b;
  return p.left;
}
int main(void) {
  printf("%d\n", helper(7, 2));
  printf("%d\n", loop_sum(2));
  printf("%d\n", array_pick(1));
  printf("%d\n", union_pick(8, 9));
  printf("%d\n", FuzzSix);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum FuzzEnum {
// LOWERING-NEXT:     FuzzZero = 0,
// LOWERING-NEXT:     FuzzOne = 1,
// LOWERING-NEXT:     FuzzFive = 5,
// LOWERING-NEXT:     FuzzSix = 6,
// LOWERING-NEXT:     FuzzNegative = -2,
// LOWERING-NEXT:     FuzzNegativeNext = -1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union FuzzPair {
// LOWERING-NEXT:     left: i32,
// LOWERING-NEXT:     right: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn helper(arg4: i32, arg5: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut c: i32 = 0;
// LOWERING-NEXT:     a = arg4;
// LOWERING-NEXT:     b = arg5;
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
// LOWERING-NEXT: fn loop_sum(arg3: i32) -> i32 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     n = arg3;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 1;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = n;
// LOWERING-NEXT:             let _v4: bool = _v2 <= _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: i32 = i;
// LOWERING-NEXT:                 let _v6: i32 = total;
// LOWERING-NEXT:                 let _v7: i32 = _v6 + _v5;
// LOWERING-NEXT:                 total = _v7;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v8: i32 = i;
// LOWERING-NEXT:             let _v9: i32 = _v8 + 1;
// LOWERING-NEXT:             i = _v9;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v10: i32 = total;
// LOWERING-NEXT:     __retval = _v10;
// LOWERING-NEXT:     let _v11: i32 = __retval;
// LOWERING-NEXT:     return _v11;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn array_pick(arg2: i32) -> i32 {
// LOWERING-NEXT:     let mut index: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: [i32; 3] = [0; 3];
// LOWERING-NEXT:     index = arg2;
// LOWERING-NEXT:     let _v0: i32 = 6;
// LOWERING-NEXT:     let _v1: i64 = 0;
// LOWERING-NEXT:     values[(_v1 as usize)] = _v0;
// LOWERING-NEXT:     let _v2: i32 = 1;
// LOWERING-NEXT:     let _v3: i64 = 1;
// LOWERING-NEXT:     values[(_v3 as usize)] = _v2;
// LOWERING-NEXT:     let _v4: i64 = 0;
// LOWERING-NEXT:     let _v5: i32 = values[(_v4 as usize)];
// LOWERING-NEXT:     let _v6: i64 = 1;
// LOWERING-NEXT:     let _v7: i32 = values[(_v6 as usize)];
// LOWERING-NEXT:     let _v8: i32 = _v5 + _v7;
// LOWERING-NEXT:     let _v9: i64 = 2;
// LOWERING-NEXT:     values[(_v9 as usize)] = _v8;
// LOWERING-NEXT:     let _v10: i32 = index;
// LOWERING-NEXT:     let _v11: i64 = _v10 as i64;
// LOWERING-NEXT:     let _v12: i32 = values[(_v11 as usize)];
// LOWERING-NEXT:     __retval = _v12;
// LOWERING-NEXT:     let _v13: i32 = __retval;
// LOWERING-NEXT:     return _v13;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn union_pick(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut p: FuzzPair = FuzzPair { left: 0 };
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: i32 = a;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         p.left = _v0;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v1: i32 = b;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         p.right = _v1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v2: i32 = unsafe { p.left };
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 7;
// LOWERING-NEXT:     let _v3: i32 = 2;
// LOWERING-NEXT:     let _v4: i32 = helper(_v2, _v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = 2;
// LOWERING-NEXT:     let _v8: i32 = loop_sum(_v7);
// LOWERING-NEXT:     let _v9: i32 = unsafe { printf(_v6 as *const i8, _v8) };
// LOWERING-NEXT:     let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i32 = 1;
// LOWERING-NEXT:     let _v12: i32 = array_pick(_v11);
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v10 as *const i8, _v12) };
// LOWERING-NEXT:     let _v14: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v15: i32 = 8;
// LOWERING-NEXT:     let _v16: i32 = 9;
// LOWERING-NEXT:     let _v17: i32 = union_pick(_v15, _v16);
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v14 as *const i8, _v17) };
// LOWERING-NEXT:     let _v19: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: i32 = FuzzEnum::FuzzSix as i32;
// LOWERING-NEXT:     let _v21: i32 = unsafe { printf(_v19 as *const i8, _v20) };
// LOWERING-NEXT:     let _v22: i32 = 0;
// LOWERING-NEXT:     __retval = _v22;
// LOWERING-NEXT:     let _v23: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v23 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum FuzzEnum {
// REWRITES-NEXT:     FuzzZero = 0,
// REWRITES-NEXT:     FuzzOne = 1,
// REWRITES-NEXT:     FuzzFive = 5,
// REWRITES-NEXT:     FuzzSix = 6,
// REWRITES-NEXT:     FuzzNegative = -2,
// REWRITES-NEXT:     FuzzNegativeNext = -1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union FuzzPair {
// REWRITES-NEXT:     left: i32,
// REWRITES-NEXT:     right: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn helper(arg4: i32, arg5: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg4;
// REWRITES-NEXT: let mut b: i32 = arg5;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut c: i32 = 0;
// REWRITES-NEXT: c = a + b;
// REWRITES-NEXT: __retval = c;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn loop_sum(arg3: i32) -> i32 {
// REWRITES-NEXT: let mut n: i32 = arg3;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 1;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i <= n) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     total = total + i;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn array_pick(arg2: i32) -> i32 {
// REWRITES-NEXT: let mut index: i32 = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut values: [i32; 3] = [0; 3];
// REWRITES-NEXT: let _v1: i64 = 0;
// REWRITES-NEXT: values[(_v1 as usize)] = 6;
// REWRITES-NEXT: let _v3: i64 = 1;
// REWRITES-NEXT: values[(_v3 as usize)] = 1;
// REWRITES-NEXT: let _v4: i64 = 0;
// REWRITES-NEXT: let _v6: i64 = 1;
// REWRITES-NEXT: let _v9: i64 = 2;
// REWRITES-NEXT: values[(_v9 as usize)] = values[(_v4 as usize)] + values[(_v6 as usize)];
// REWRITES-NEXT: __retval = values[((index as i64) as usize)];
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn union_pick(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut a: i32 = arg0;
// REWRITES-NEXT: let mut b: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut p: FuzzPair = FuzzPair { left: 0 };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         p.left = a;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         p.right = b;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = unsafe { p.left };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 7;
// REWRITES-NEXT: let _v3: i32 = 2;
// REWRITES-NEXT: let _v4: i32 = helper(_v2, _v3);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v1 as *const i8, _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = 2;
// REWRITES-NEXT: let _v8: i32 = loop_sum(_v7);
// REWRITES-NEXT: let _v9: i32 = unsafe { printf(_v6 as *const i8, _v8) };
// REWRITES-NEXT: let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i32 = 1;
// REWRITES-NEXT: let _v12: i32 = array_pick(_v11);
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v10 as *const i8, _v12) };
// REWRITES-NEXT: let _v14: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = 8;
// REWRITES-NEXT: let _v16: i32 = 9;
// REWRITES-NEXT: let _v17: i32 = union_pick(_v15, _v16);
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v14 as *const i8, _v17) };
// REWRITES-NEXT: let _v19: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v20: i32 = FuzzEnum::FuzzSix as i32;
// REWRITES-NEXT: let _v21: i32 = unsafe { printf(_v19 as *const i8, _v20) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
