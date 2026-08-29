#include <stdio.h>

struct Inner {
  int x;
  int y;
};

struct Outer {
  struct Inner a;
  int          z;
};

int main(void) {
  struct Outer o;
  o.a.x = 3;
  o.a.y = 4;
  o.z   = 5;
  printf("%d\n", o.a.x + o.a.y + o.z);

  struct Outer init = {{1, 2}, 3};
  printf("%d\n", init.a.x + init.a.y + init.z);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Inner {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Outer {
// LOWERING-NEXT:     a: Inner,
// LOWERING-NEXT:     z: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut o: Outer = Outer { a: Inner { x: 0, y: 0 }, z: 0 };
// LOWERING-NEXT:     let mut init: Outer = Outer { a: Inner { x: 0, y: 0 }, z: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 3;
// LOWERING-NEXT:     o.a.x = _v1;
// LOWERING-NEXT:     let _v2: i32 = 4;
// LOWERING-NEXT:     o.a.y = _v2;
// LOWERING-NEXT:     let _v3: i32 = 5;
// LOWERING-NEXT:     o.z = _v3;
// LOWERING-NEXT:     let _v4: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = o.a.x;
// LOWERING-NEXT:     let _v6: i32 = o.a.y;
// LOWERING-NEXT:     let _v7: i32 = _v5 + _v6;
// LOWERING-NEXT:     let _v8: i32 = o.z;
// LOWERING-NEXT:     let _v9: i32 = _v7 + _v8;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v4 as *const i8, _v9) };
// LOWERING-NEXT:     init = Outer { a: Inner { x: 1, y: 2 }, z: 3 };
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: i32 = init.a.x;
// LOWERING-NEXT:     let _v13: i32 = init.a.y;
// LOWERING-NEXT:     let _v14: i32 = _v12 + _v13;
// LOWERING-NEXT:     let _v15: i32 = init.z;
// LOWERING-NEXT:     let _v16: i32 = _v14 + _v15;
// LOWERING-NEXT:     let _v17: i32 = unsafe { printf(_v11 as *const i8, _v16) };
// LOWERING-NEXT:     let _v18: i32 = 0;
// LOWERING-NEXT:     __retval = _v18;
// LOWERING-NEXT:     let _v19: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v19 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Inner {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Outer {
// REWRITES-NEXT:     a: Inner,
// REWRITES-NEXT:     z: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut o: Outer = Outer { a: Inner { x: 0, y: 0 }, z: 0 };
// REWRITES-NEXT: let mut init: Outer = Outer { a: Inner { x: 0, y: 0 }, z: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: o.a.x = 3;
// REWRITES-NEXT: o.a.y = 4;
// REWRITES-NEXT: o.z = 5;
// REWRITES-NEXT: let _v4: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v4 as *const i8, o.a.x + o.a.y + o.z) };
// REWRITES-NEXT: init = Outer { a: Inner { x: 1, y: 2 }, z: 3 };
// REWRITES-NEXT: let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: i32 = unsafe { printf(_v11 as *const i8, init.a.x + init.a.y + init.z) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
