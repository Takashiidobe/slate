#include <stdio.h>

static char add_char(char a, char b) {
  char c = a + b;
  return c;
}

int main(void) {
  char          x      = 10;
  unsigned char y      = 200;
  signed char   z      = -5;
  char          letter = 'A';
  printf("%d\n", add_char(x, z));
  printf("%d\n", y);
  printf("%c\n", letter);
  printf("%c\n", letter + 1);
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
// LOWERING-NEXT: fn add_char(arg0: i8, arg1: i8) -> i8 {
// LOWERING-NEXT:     let mut a: i8 = 0;
// LOWERING-NEXT:     let mut b: i8 = 0;
// LOWERING-NEXT:     let mut __retval: i8 = 0;
// LOWERING-NEXT:     let mut c: i8 = 0;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: i8 = a;
// LOWERING-NEXT:     let _v1: i32 = _v0 as i32;
// LOWERING-NEXT:     let _v2: i8 = b;
// LOWERING-NEXT:     let _v3: i32 = _v2 as i32;
// LOWERING-NEXT:     let _v4: i32 = _v1 + _v3;
// LOWERING-NEXT:     let _v5: i8 = _v4 as i8;
// LOWERING-NEXT:     c = _v5;
// LOWERING-NEXT:     let _v6: i8 = c;
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: i8 = __retval;
// LOWERING-NEXT:     return _v7;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: i8 = 0;
// LOWERING-NEXT:     let mut y: u8 = 0;
// LOWERING-NEXT:     let mut z: i8 = 0;
// LOWERING-NEXT:     let mut letter: i8 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i8 = 10;
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: u8 = 200;
// LOWERING-NEXT:     y = _v2;
// LOWERING-NEXT:     let _v3: i8 = -5;
// LOWERING-NEXT:     z = _v3;
// LOWERING-NEXT:     let _v4: i8 = 65;
// LOWERING-NEXT:     letter = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i8 = x;
// LOWERING-NEXT:     let _v7: i8 = z;
// LOWERING-NEXT:     let _v8: i8 = add_char(_v6, _v7);
// LOWERING-NEXT:     let _v9: i32 = _v8 as i32;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v5 as *const i8, _v9) };
// LOWERING-NEXT:     let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: u8 = y;
// LOWERING-NEXT:     let _v13: i32 = _v12 as i32;
// LOWERING-NEXT:     let _v14: i32 = unsafe { printf(_v11 as *const i8, _v13) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: i8 = letter;
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v15 as *const i8, _v17) };
// LOWERING-NEXT:     let _v19: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: i8 = letter;
// LOWERING-NEXT:     let _v21: i32 = _v20 as i32;
// LOWERING-NEXT:     let _v22: i32 = 1;
// LOWERING-NEXT:     let _v23: i32 = _v21 + _v22;
// LOWERING-NEXT:     let _v24: i32 = unsafe { printf(_v19 as *const i8, _v23) };
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
// REWRITES-NEXT: fn add_char(arg0: i8, arg1: i8) -> i8 {
// REWRITES-NEXT: let mut a: i8 = arg0;
// REWRITES-NEXT: let mut b: i8 = arg1;
// REWRITES-NEXT: let mut __retval: i8 = 0;
// REWRITES-NEXT: let mut c: i8 = 0;
// REWRITES-NEXT: c = ((a as i32) + (b as i32)) as i8;
// REWRITES-NEXT: __retval = c;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: i8 = 0;
// REWRITES-NEXT: let mut y: u8 = 0;
// REWRITES-NEXT: let mut z: i8 = 0;
// REWRITES-NEXT: let mut letter: i8 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = 10;
// REWRITES-NEXT: y = 200;
// REWRITES-NEXT: z = -5;
// REWRITES-NEXT: letter = 65;
// REWRITES-NEXT: let _v5: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i8 = add_char(x, z);
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v5 as *const i8, _v8 as i32) };
// REWRITES-NEXT: let _v11: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: i32 = unsafe { printf(_v11 as *const i8, y as i32) };
// REWRITES-NEXT: let _v15: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v15 as *const i8, letter as i32) };
// REWRITES-NEXT: let _v19: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: i32 = 1;
// REWRITES-NEXT: let _v24: i32 = unsafe { printf(_v19 as *const i8, (letter as i32) + _v22) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
