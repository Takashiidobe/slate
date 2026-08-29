#include <stdio.h>

typedef unsigned char my_char_t;

struct buffer {
  my_char_t *start;
  my_char_t *end;
};

int main(void) {
  my_char_t     data[4] = {65, 66, 67, 0};
  struct buffer b;
  b.start = data;
  b.end   = data + 3;
  printf("%c%c%c\n", b.start[0], b.start[1], b.start[2]);
  printf("%d\n", (int)(b.end - b.start));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct buffer {
// LOWERING-NEXT:     start: *mut u8,
// LOWERING-NEXT:     end: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut data: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut b: buffer = buffer { start: std::ptr::null_mut(), end: std::ptr::null_mut() };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     data = [65, 66, 67, 0];
// LOWERING-NEXT:     let _v1: *mut u8 = data.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     b.start = _v1;
// LOWERING-NEXT:     let _v2: *mut u8 = data.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v3: i32 = 3;
// LOWERING-NEXT:     let _v4: *mut u8 = unsafe { _v2.add(3) };
// LOWERING-NEXT:     b.end = _v4;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%c%c%c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i64 = 0;
// LOWERING-NEXT:     let _v7: *mut u8 = b.start;
// LOWERING-NEXT:     let _v8: *mut u8 = unsafe { _v7.add(0) };
// LOWERING-NEXT:     let _v9: u8 = unsafe { *_v8 };
// LOWERING-NEXT:     let _v10: i32 = _v9 as i32;
// LOWERING-NEXT:     let _v11: i64 = 1;
// LOWERING-NEXT:     let _v12: *mut u8 = b.start;
// LOWERING-NEXT:     let _v13: *mut u8 = unsafe { _v12.add(1) };
// LOWERING-NEXT:     let _v14: u8 = unsafe { *_v13 };
// LOWERING-NEXT:     let _v15: i32 = _v14 as i32;
// LOWERING-NEXT:     let _v16: i64 = 2;
// LOWERING-NEXT:     let _v17: *mut u8 = b.start;
// LOWERING-NEXT:     let _v18: *mut u8 = unsafe { _v17.add(2) };
// LOWERING-NEXT:     let _v19: u8 = unsafe { *_v18 };
// LOWERING-NEXT:     let _v20: i32 = _v19 as i32;
// LOWERING-NEXT:     let _v21: i32 = unsafe { printf(_v5 as *const i8, _v10, _v15, _v20) };
// LOWERING-NEXT:     let _v22: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v23: *mut u8 = b.end;
// LOWERING-NEXT:     let _v24: *mut u8 = b.start;
// LOWERING-NEXT:     let _v25: i64 = unsafe { _v23.offset_from(_v24) as i64 };
// LOWERING-NEXT:     let _v26: i32 = _v25 as i32;
// LOWERING-NEXT:     let _v27: i32 = unsafe { printf(_v22 as *const i8, _v26) };
// LOWERING-NEXT:     let _v28: i32 = 0;
// LOWERING-NEXT:     __retval = _v28;
// LOWERING-NEXT:     let _v29: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v29 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct buffer {
// REWRITES-NEXT:     start: *mut u8,
// REWRITES-NEXT:     end: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut data: [u8; 4] = [0; 4];
// REWRITES-NEXT: let mut b: buffer = buffer { start: std::ptr::null_mut(), end: std::ptr::null_mut() };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: data = [65, 66, 67, 0];
// REWRITES-NEXT: let _v1: *mut u8 = data.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: b.start = _v1;
// REWRITES-NEXT: let _v2: *mut u8 = data.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v3: i32 = 3;
// REWRITES-NEXT: let _v4: *mut u8 = unsafe { _v2.add(3) };
// REWRITES-NEXT: b.end = _v4;
// REWRITES-NEXT: let _v5: *mut i8 = b"%c%c%c\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i64 = 0;
// REWRITES-NEXT: let _v7: *mut u8 = b.start;
// REWRITES-NEXT: let _v8: *mut u8 = unsafe { _v7.add(0) };
// REWRITES-NEXT: let _v10: i32 = (unsafe { *_v8 }) as i32;
// REWRITES-NEXT: let _v11: i64 = 1;
// REWRITES-NEXT: let _v12: *mut u8 = b.start;
// REWRITES-NEXT: let _v13: *mut u8 = unsafe { _v12.add(1) };
// REWRITES-NEXT: let _v15: i32 = (unsafe { *_v13 }) as i32;
// REWRITES-NEXT: let _v16: i64 = 2;
// REWRITES-NEXT: let _v17: *mut u8 = b.start;
// REWRITES-NEXT: let _v18: *mut u8 = unsafe { _v17.add(2) };
// REWRITES-NEXT: let _v21: i32 = unsafe { printf(_v5 as *const i8, _v10, _v15, (unsafe { *_v18 }) as i32) };
// REWRITES-NEXT: let _v22: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: *mut u8 = b.end;
// REWRITES-NEXT: let _v24: *mut u8 = b.start;
// REWRITES-NEXT: let _v25: i64 = unsafe { _v23.offset_from(_v24) as i64 };
// REWRITES-NEXT: let _v27: i32 = unsafe { printf(_v22 as *const i8, _v25 as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
