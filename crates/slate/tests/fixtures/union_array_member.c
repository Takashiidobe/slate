#include <stdio.h>

union Word {
  int           value;
  unsigned char bytes[4];
};

int main(void) {
  union Word word = {0};
  word.bytes[0]   = 65;
  word.bytes[1]   = 66;
  printf("%d %d\n", word.bytes[0], word.bytes[1]);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union Word {
// LOWERING-NEXT:     value: i32,
// LOWERING-NEXT:     bytes: [u8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut word: Word = Word { value: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     word = Word { value: 0 };
// LOWERING-NEXT:     let _v1: u8 = 65;
// LOWERING-NEXT:     let _v2: i64 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         word.bytes[(_v2 as usize)] = _v1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v3: u8 = 66;
// LOWERING-NEXT:     let _v4: i64 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         word.bytes[(_v4 as usize)] = _v3;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i64 = 0;
// LOWERING-NEXT:     let _v7: u8 = unsafe { word.bytes[(_v6 as usize)] };
// LOWERING-NEXT:     let _v8: i32 = _v7 as i32;
// LOWERING-NEXT:     let _v9: i64 = 1;
// LOWERING-NEXT:     let _v10: u8 = unsafe { word.bytes[(_v9 as usize)] };
// LOWERING-NEXT:     let _v11: i32 = _v10 as i32;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v5 as *const i8, _v8, _v11) };
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union Word {
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT:     bytes: [u8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut word: Word = Word { value: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: word = Word { value: 0 };
// REWRITES-NEXT: let _v2: i64 = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         word.bytes[(_v2 as usize)] = 65;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v4: i64 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         word.bytes[(_v4 as usize)] = 66;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v5: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i64 = 0;
// REWRITES-NEXT: let _v9: i64 = 1;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v5 as *const i8, (unsafe { word.bytes[(_v6 as usize)] }) as i32, (unsafe { word.bytes[(_v9 as usize)] }) as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
