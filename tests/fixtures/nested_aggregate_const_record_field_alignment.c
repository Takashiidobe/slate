#include <stdio.h>

typedef int (*fnptr)(int);

int add1(int x) { return x + 1; }
int add2(int x) { return x + 2; }
int add3(int x) { return x + 3; }
int mul5(int x) { return x * 5; }
int mul7(int x) { return x * 7; }

struct Inner {
  fnptr scanners[3];
  int   tag;
};

struct Outer {
  struct Inner  inner;
  unsigned char bytes[8];
  fnptr         trailing1;
  fnptr         trailing2;
  int           trailing3;
};

struct Outer g = {
    {{add1, add2, add3}, 42}, {1, 2, 3, 4, 5, 6, 7, 8}, mul5, mul7, 99,
};

int main(void) {
  printf("%d %d %d %d\n", g.inner.scanners[0](10), g.inner.scanners[1](10),
         g.inner.scanners[2](10), g.inner.tag);
  for (int i = 0; i < 8; i++) {
    printf("%d ", g.bytes[i]);
  }
  printf("\n");
  printf("%d %d %d\n", g.trailing1(10), g.trailing2(10), g.trailing3);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Inner {
// LOWERING-NEXT:     scanners: [Option<unsafe extern "C" fn(i32) -> i32>; 3],
// LOWERING-NEXT:     tag: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Outer {
// LOWERING-NEXT:     inner: Inner,
// LOWERING-NEXT:     bytes: [u8; 8],
// LOWERING-NEXT:     trailing1: Option<unsafe extern "C" fn(i32) -> i32>,
// LOWERING-NEXT:     trailing2: Option<unsafe extern "C" fn(i32) -> i32>,
// LOWERING-NEXT:     trailing3: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut g: Outer = Outer { inner: Inner { scanners: [unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(add1 as *const ()) }, unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(add2 as *const ()) }, unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(add3 as *const ()) }], tag: 42 }, bytes: [1, 2, 3, 4, 5, 6, 7, 8], trailing1: unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(mul5 as *const ()) }, trailing2: unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(mul7 as *const ()) }, trailing3: 99 };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add1(arg4: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg4;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add2(arg3: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg3;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 2;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add3(arg2: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg2;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 3;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn mul5(arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg1;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 5;
// LOWERING-NEXT:     let _v2: i32 = _v0 * _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn mul7(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 7;
// LOWERING-NEXT:     let _v2: i32 = _v0 * _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i64 = 0;
// LOWERING-NEXT:     let _v3: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.inner.scanners[(_v2 as usize)] };
// LOWERING-NEXT:     let _v4: i32 = 10;
// LOWERING-NEXT:     let _v5: i32 = unsafe { _v3.unwrap()(_v4) };
// LOWERING-NEXT:     let _v6: i64 = 1;
// LOWERING-NEXT:     let _v7: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.inner.scanners[(_v6 as usize)] };
// LOWERING-NEXT:     let _v8: i32 = 10;
// LOWERING-NEXT:     let _v9: i32 = unsafe { _v7.unwrap()(_v8) };
// LOWERING-NEXT:     let _v10: i64 = 2;
// LOWERING-NEXT:     let _v11: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.inner.scanners[(_v10 as usize)] };
// LOWERING-NEXT:     let _v12: i32 = 10;
// LOWERING-NEXT:     let _v13: i32 = unsafe { _v11.unwrap()(_v12) };
// LOWERING-NEXT:     let _v14: i32 = unsafe { g.inner.tag };
// LOWERING-NEXT:     let _v15: i32 = unsafe { printf(_v1 as *const i8, _v5, _v9, _v13, _v14) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v16: i32 = 0;
// LOWERING-NEXT:         i = _v16;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v17: i32 = i;
// LOWERING-NEXT:             let _v18: i32 = 8;
// LOWERING-NEXT:             let _v19: bool = _v17 < _v18;
// LOWERING-NEXT:             if !_v19 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v20: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v21: i32 = i;
// LOWERING-NEXT:                 let _v22: i64 = _v21 as i64;
// LOWERING-NEXT:                 let _v23: u8 = unsafe { g.bytes[(_v22 as usize)] };
// LOWERING-NEXT:                 let _v24: i32 = _v23 as i32;
// LOWERING-NEXT:                 let _v25: i32 = unsafe { printf(_v20 as *const i8, _v24) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v26: i32 = i;
// LOWERING-NEXT:             let _v27: i32 = _v26 + 1;
// LOWERING-NEXT:             i = _v27;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v28: *mut i8 = b"\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v29: i32 = unsafe { printf(_v28 as *const i8) };
// LOWERING-NEXT:     let _v30: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v31: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.trailing1 };
// LOWERING-NEXT:     let _v32: i32 = 10;
// LOWERING-NEXT:     let _v33: i32 = unsafe { _v31.unwrap()(_v32) };
// LOWERING-NEXT:     let _v34: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.trailing2 };
// LOWERING-NEXT:     let _v35: i32 = 10;
// LOWERING-NEXT:     let _v36: i32 = unsafe { _v34.unwrap()(_v35) };
// LOWERING-NEXT:     let _v37: i32 = unsafe { g.trailing3 };
// LOWERING-NEXT:     let _v38: i32 = unsafe { printf(_v30 as *const i8, _v33, _v36, _v37) };
// LOWERING-NEXT:     let _v39: i32 = 0;
// LOWERING-NEXT:     __retval = _v39;
// LOWERING-NEXT:     let _v40: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v40 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Inner {
// REWRITES-NEXT:     scanners: [Option<unsafe extern "C" fn(i32) -> i32>; 3],
// REWRITES-NEXT:     tag: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Outer {
// REWRITES-NEXT:     inner: Inner,
// REWRITES-NEXT:     bytes: [u8; 8],
// REWRITES-NEXT:     trailing1: Option<unsafe extern "C" fn(i32) -> i32>,
// REWRITES-NEXT:     trailing2: Option<unsafe extern "C" fn(i32) -> i32>,
// REWRITES-NEXT:     trailing3: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut g: Outer = Outer { inner: Inner { scanners: [unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(add1 as *const ()) }, unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(add2 as *const ()) }, unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(add3 as *const ()) }], tag: 42 }, bytes: [1, 2, 3, 4, 5, 6, 7, 8], trailing1: unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(mul5 as *const ()) }, trailing2: unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(mul7 as *const ()) }, trailing3: 99 };
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add1(arg4: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg4;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 1;
// REWRITES-NEXT: __retval = x + _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add2(arg3: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg3;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 2;
// REWRITES-NEXT: __retval = x + _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add3(arg2: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 3;
// REWRITES-NEXT: __retval = x + _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn mul5(arg1: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 5;
// REWRITES-NEXT: __retval = x * _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn mul7(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 7;
// REWRITES-NEXT: __retval = x * _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i64 = 0;
// REWRITES-NEXT: let _v4: i32 = 10;
// REWRITES-NEXT: let _v5: i32 = unsafe { unsafe { g.inner.scanners[(_v2 as usize)] }.unwrap()(_v4) };
// REWRITES-NEXT: let _v6: i64 = 1;
// REWRITES-NEXT: let _v8: i32 = 10;
// REWRITES-NEXT: let _v9: i32 = unsafe { unsafe { g.inner.scanners[(_v6 as usize)] }.unwrap()(_v8) };
// REWRITES-NEXT: let _v10: i64 = 2;
// REWRITES-NEXT: let _v12: i32 = 10;
// REWRITES-NEXT: let _v13: i32 = unsafe { unsafe { g.inner.scanners[(_v10 as usize)] }.unwrap()(_v12) };
// REWRITES-NEXT: let _v14: i32 = unsafe { g.inner.tag };
// REWRITES-NEXT: let _v15: i32 = unsafe { printf(_v1 as *const i8, _v5, _v9, _v13, _v14) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v18: i32 = 8;
// REWRITES-NEXT:                     if !(i < _v18) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v20: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                     let _v25: i32 = unsafe { printf(_v20 as *const i8, (unsafe { g.bytes[((i as i64) as usize)] }) as i32) };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v28: *mut i8 = b"\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v29: i32 = unsafe { printf(_v28 as *const i8) };
// REWRITES-NEXT: let _v30: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v32: i32 = 10;
// REWRITES-NEXT: let _v33: i32 = unsafe { unsafe { g.trailing1 }.unwrap()(_v32) };
// REWRITES-NEXT: let _v35: i32 = 10;
// REWRITES-NEXT: let _v36: i32 = unsafe { unsafe { g.trailing2 }.unwrap()(_v35) };
// REWRITES-NEXT: let _v37: i32 = unsafe { g.trailing3 };
// REWRITES-NEXT: let _v38: i32 = unsafe { printf(_v30 as *const i8, _v33, _v36, _v37) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
