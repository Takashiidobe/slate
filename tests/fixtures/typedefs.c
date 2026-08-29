#include <stdio.h>

typedef int           my_int;
typedef unsigned char byte;
typedef long long     wide;

struct Box {
  my_int value;
  byte   tag;
};

static my_int add_alias(my_int a, my_int b) {
  my_int c = a + b;
  return c;
}

int main(void) {
  my_int     x = 40;
  byte       y = 200;
  wide       z = 9000000000LL;
  struct Box bx;
  bx.value = add_alias(x, 2);
  bx.tag   = y;
  printf("%d\n", bx.value);
  printf("%d\n", bx.tag);
  printf("%lld\n", z);
  printf("%d\n", (int)sizeof(my_int));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Box {
// LOWERING-NEXT:     value: i32,
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_alias(arg0: i32, arg1: i32) -> i32 {
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
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut y: u8 = 0;
// LOWERING-NEXT:     let mut z: i64 = 0;
// LOWERING-NEXT:     let mut bx: Box = Box { value: 0, tag: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 40;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: u8 = 200;
// LOWERING-NEXT:     y = _v2;
// LOWERING-NEXT:     let _v3: i64 = 9000000000i64;
// LOWERING-NEXT:     z = _v3;
// LOWERING-NEXT:     let _v4: i32 = x;
// LOWERING-NEXT:     let _v5: i32 = 2;
// LOWERING-NEXT:     let _v6: i32 = add_alias(_v4, _v5);
// LOWERING-NEXT:     bx.value = _v6;
// LOWERING-NEXT:     let _v7: u8 = y;
// LOWERING-NEXT:     bx.tag = _v7;
// LOWERING-NEXT:     let _v8: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: i32 = bx.value;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v8 as *const i8, _v9) };
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: u8 = bx.tag;
// LOWERING-NEXT:     let _v13: i32 = _v12 as i32;
// LOWERING-NEXT:     let _v14: i32 = unsafe { printf(_v11 as *const i8, _v13) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"%lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: i64 = z;
// LOWERING-NEXT:     let _v17: i32 = unsafe { printf(_v15 as *const i8, _v16) };
// LOWERING-NEXT:     let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: i32 = 4;
// LOWERING-NEXT:     let _v20: i32 = unsafe { printf(_v18 as *const i8, _v19) };
// LOWERING-NEXT:     let _v21: i32 = 0;
// LOWERING-NEXT:     __retval = _v21;
// LOWERING-NEXT:     let _v22: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v22 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Box {
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_alias(arg0: i32, arg1: i32) -> i32 {
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
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: let mut y: u8 = 0;
// REWRITES-NEXT: let mut z: i64 = 0;
// REWRITES-NEXT: let mut bx: Box = Box { value: 0, tag: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 40;
// REWRITES-NEXT: y = 200;
// REWRITES-NEXT: z = 9000000000i64;
// REWRITES-NEXT: let _v5: i32 = 2;
// REWRITES-NEXT: let _v6: i32 = add_alias(x, _v5);
// REWRITES-NEXT: bx.value = _v6;
// REWRITES-NEXT: bx.tag = y;
// REWRITES-NEXT: let _v8: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v9: i32 = bx.value;
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v8 as *const i8, _v9) };
// REWRITES-NEXT: let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: i32 = unsafe { printf(_v11 as *const i8, bx.tag as i32) };
// REWRITES-NEXT: let _v15: *mut i8 = b"%lld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v17: i32 = unsafe { printf(_v15 as *const i8, z) };
// REWRITES-NEXT: let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i32 = 4;
// REWRITES-NEXT: let _v20: i32 = unsafe { printf(_v18 as *const i8, _v19) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
