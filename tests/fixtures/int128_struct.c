#include <stdio.h>

struct Wide {
  int               tag;
  __int128          value;
  unsigned __int128 uvalue;
};

int main(void) {
  struct Wide w;
  w.tag     = 7;
  w.value   = -1234567890123456789;
  w.uvalue  = 12345678901234567890ULL;
  w.value  += 1;
  w.uvalue *= 2;

  printf("%d\n", w.tag);
  printf("%llu\n", (unsigned long long)(w.value >> 64));
  printf("%llu\n", (unsigned long long)w.value);
  printf("%llu\n", (unsigned long long)(w.uvalue >> 64));
  printf("%llu\n", (unsigned long long)w.uvalue);
  printf("%zu\n", sizeof(struct Wide));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Wide {
// LOWERING-NEXT:     tag: i32,
// LOWERING-NEXT:     value: i128,
// LOWERING-NEXT:     uvalue: u128,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut w: Wide = Wide { tag: 0, value: 0, uvalue: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 7;
// LOWERING-NEXT:     w.tag = _v1;
// LOWERING-NEXT:     let _v2: i128 = -1234567890123456789i128;
// LOWERING-NEXT:     w.value = _v2;
// LOWERING-NEXT:     let _v3: u128 = 12345678901234567890u128;
// LOWERING-NEXT:     w.uvalue = _v3;
// LOWERING-NEXT:     let _v4: i128 = 1;
// LOWERING-NEXT:     let _v5: i128 = w.value;
// LOWERING-NEXT:     let _v6: i128 = _v5 + _v4;
// LOWERING-NEXT:     w.value = _v6;
// LOWERING-NEXT:     let _v7: u128 = 2;
// LOWERING-NEXT:     let _v8: u128 = w.uvalue;
// LOWERING-NEXT:     let _v9: u128 = _v8 * _v7;
// LOWERING-NEXT:     w.uvalue = _v9;
// LOWERING-NEXT:     let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i32 = w.tag;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v10 as *const i8, _v11) };
// LOWERING-NEXT:     let _v13: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: i128 = w.value;
// LOWERING-NEXT:     let _v15: i32 = 64;
// LOWERING-NEXT:     let _v16: i128 = _v14 >> _v15;
// LOWERING-NEXT:     let _v17: u64 = _v16 as u64;
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v13 as *const i8, _v17) };
// LOWERING-NEXT:     let _v19: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: i128 = w.value;
// LOWERING-NEXT:     let _v21: u64 = _v20 as u64;
// LOWERING-NEXT:     let _v22: i32 = unsafe { printf(_v19 as *const i8, _v21) };
// LOWERING-NEXT:     let _v23: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: u128 = w.uvalue;
// LOWERING-NEXT:     let _v25: i32 = 64;
// LOWERING-NEXT:     let _v26: u128 = _v24 >> _v25;
// LOWERING-NEXT:     let _v27: u64 = _v26 as u64;
// LOWERING-NEXT:     let _v28: i32 = unsafe { printf(_v23 as *const i8, _v27) };
// LOWERING-NEXT:     let _v29: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v30: u128 = w.uvalue;
// LOWERING-NEXT:     let _v31: u64 = _v30 as u64;
// LOWERING-NEXT:     let _v32: i32 = unsafe { printf(_v29 as *const i8, _v31) };
// LOWERING-NEXT:     let _v33: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v34: u64 = std::mem::size_of::<Wide>() as u64;
// LOWERING-NEXT:     let _v35: i32 = unsafe { printf(_v33 as *const i8, _v34) };
// LOWERING-NEXT:     let _v36: i32 = 0;
// LOWERING-NEXT:     __retval = _v36;
// LOWERING-NEXT:     let _v37: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v37 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Wide {
// REWRITES-NEXT:     tag: i32,
// REWRITES-NEXT:     value: i128,
// REWRITES-NEXT:     uvalue: u128,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut w: Wide = Wide { tag: 0, value: 0, uvalue: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: w.tag = 7;
// REWRITES-NEXT: w.value = -1234567890123456789i128;
// REWRITES-NEXT: w.uvalue = 12345678901234567890u128;
// REWRITES-NEXT: let _v4: i128 = 1;
// REWRITES-NEXT: w.value = w.value + _v4;
// REWRITES-NEXT: let _v7: u128 = 2;
// REWRITES-NEXT: w.uvalue = w.uvalue * _v7;
// REWRITES-NEXT: let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i32 = w.tag;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v10 as *const i8, _v11) };
// REWRITES-NEXT: let _v13: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = 64;
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v13 as *const i8, (w.value >> _v15) as u64) };
// REWRITES-NEXT: let _v19: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: i32 = unsafe { printf(_v19 as *const i8, w.value as u64) };
// REWRITES-NEXT: let _v23: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v25: i32 = 64;
// REWRITES-NEXT: let _v28: i32 = unsafe { printf(_v23 as *const i8, (w.uvalue >> _v25) as u64) };
// REWRITES-NEXT: let _v29: *mut i8 = b"%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v32: i32 = unsafe { printf(_v29 as *const i8, w.uvalue as u64) };
// REWRITES-NEXT: let _v33: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v34: u64 = std::mem::size_of::<Wide>() as u64;
// REWRITES-NEXT: let _v35: i32 = unsafe { printf(_v33 as *const i8, _v34) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
