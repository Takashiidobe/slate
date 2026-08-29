#include <stdio.h>

int main(void) {
  int x = 0;
  x     = 42;

  int y = 0;
  int z = getchar();
  y     = z + 1;

  printf("%d %d %d\n", x, y, z);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn getchar() -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut y: i32 = 0;
// LOWERING-NEXT:     let mut z: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 0;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: i32 = 42;
// LOWERING-NEXT:     x = _v2;
// LOWERING-NEXT:     let _v3: i32 = 0;
// LOWERING-NEXT:     y = _v3;
// LOWERING-NEXT:     let _v4: i32 = unsafe { getchar() };
// LOWERING-NEXT:     z = _v4;
// LOWERING-NEXT:     let _v5: i32 = z;
// LOWERING-NEXT:     let _v6: i32 = 1;
// LOWERING-NEXT:     let _v7: i32 = _v5 + _v6;
// LOWERING-NEXT:     y = _v7;
// LOWERING-NEXT:     let _v8: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: i32 = x;
// LOWERING-NEXT:     let _v10: i32 = y;
// LOWERING-NEXT:     let _v11: i32 = z;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v8 as *const i8, _v9, _v10, _v11) };
// LOWERING-NEXT:     let _v13: i32 = 0;
// LOWERING-NEXT:     __retval = _v13;
// LOWERING-NEXT:     let _v14: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v14 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn getchar() -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: let mut y: i32 = 0;
// REWRITES-NEXT: let mut z: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 0;
// REWRITES-NEXT: x = 42;
// REWRITES-NEXT: y = 0;
// REWRITES-NEXT: z = unsafe { getchar() };
// REWRITES-NEXT: let _v6: i32 = 1;
// REWRITES-NEXT: y = z + _v6;
// REWRITES-NEXT: let _v8: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v8 as *const i8, x, y, z) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
