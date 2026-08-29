#include <stdio.h>

typedef enum { E_OK = 0, E_FAIL = 1 } Status;

typedef Status (*Handler)(int);

struct Parser {
  Handler processor;
};

static Status handle_ok(int x) {
  (void)x;
  return E_OK;
}

static Status handle_fail(int x) {
  (void)x;
  return E_FAIL;
}

int main(void) {
  struct Parser p;
  for (int i = 0; i < 2; i++) {
    p.processor   = (i == 0) ? handle_ok : handle_fail;
    Status result = p.processor(5);
    if (result == E_OK) {
      printf("ok\n");
    } else {
      printf("fail\n");
    }
  }
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum Status {
// LOWERING-NEXT:     E_OK = 0,
// LOWERING-NEXT:     E_FAIL = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Parser {
// LOWERING-NEXT:     processor: Option<unsafe extern "C" fn(i32) -> u32>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn handle_ok(arg1: i32) -> u32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: u32 = 0;
// LOWERING-NEXT:     x = arg1;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: u32 = Status::E_OK as u32;
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: u32 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn handle_fail(arg0: i32) -> u32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: u32 = 0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: u32 = Status::E_FAIL as u32;
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: u32 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut p: Parser = Parser { processor: None };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = 2;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut result: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// LOWERING-NEXT:                 let _v5: i32 = i;
// LOWERING-NEXT:                 let _v6: i32 = 0;
// LOWERING-NEXT:                 let _v7: bool = _v5 == _v6;
// LOWERING-NEXT:                 let _v8: Option<unsafe extern "C" fn(i32) -> u32> = if _v7 { Some(handle_ok) } else { Some(handle_fail) };
// LOWERING-NEXT:                 p.processor = _v8;
// LOWERING-NEXT:                 let _v9: Option<unsafe extern "C" fn(i32) -> u32> = p.processor;
// LOWERING-NEXT:                 let _v10: i32 = 5;
// LOWERING-NEXT:                 let _v11: u32 = unsafe { _v9.unwrap()(_v10) };
// LOWERING-NEXT:                 *result = unsafe { std::mem::transmute(_v11) };
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v12: u32 = *result as u32;
// LOWERING-NEXT:                     let _v13: u32 = Status::E_OK as u32;
// LOWERING-NEXT:                     let _v14: bool = _v12 == _v13;
// LOWERING-NEXT:                     if _v14 {
// LOWERING-NEXT:                         let _v15: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let _v16: i32 = unsafe { printf(_v15 as *const i8) };
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let _v17: *mut i8 = b"fail\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let _v18: i32 = unsafe { printf(_v17 as *const i8) };
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v19: i32 = i;
// LOWERING-NEXT:             let _v20: i32 = _v19 + 1;
// LOWERING-NEXT:             i = _v20;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
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
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum Status {
// REWRITES-NEXT:     E_OK = 0,
// REWRITES-NEXT:     E_FAIL = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Parser {
// REWRITES-NEXT:     processor: Option<unsafe extern "C" fn(i32) -> u32>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn handle_ok(arg1: i32) -> u32 {
// REWRITES-NEXT: let mut x: i32 = arg1;
// REWRITES-NEXT: let mut __retval: u32 = 0;
// REWRITES-NEXT: let _v0: i32 = x;
// REWRITES-NEXT: let _v1: u32 = Status::E_OK as u32;
// REWRITES-NEXT: __retval = _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn handle_fail(arg0: i32) -> u32 {
// REWRITES-NEXT: let mut x: i32 = arg0;
// REWRITES-NEXT: let mut __retval: u32 = 0;
// REWRITES-NEXT: let _v0: i32 = x;
// REWRITES-NEXT: let _v1: u32 = Status::E_FAIL as u32;
// REWRITES-NEXT: __retval = _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut p: Parser = Parser { processor: None };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v3: i32 = 2;
// REWRITES-NEXT:                     if !(i < _v3) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let mut result: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// REWRITES-NEXT:                                     let _v6: i32 = 0;
// REWRITES-NEXT:                                     let _v7: bool = i == _v6;
// REWRITES-NEXT:                                     let _v8: Option<unsafe extern "C" fn(i32) -> u32> = if _v7 { Some(handle_ok) } else { Some(handle_fail) };
// REWRITES-NEXT:                                     p.processor = _v8;
// REWRITES-NEXT:                                     let _v10: i32 = 5;
// REWRITES-NEXT:                                     let _v11: u32 = unsafe { p.processor.unwrap()(_v10) };
// REWRITES-NEXT:                                     *result = unsafe { std::mem::transmute(_v11) };
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let _v12: u32 = *result as u32;
// REWRITES-NEXT:                                                         let _v13: u32 = Status::E_OK as u32;
// REWRITES-NEXT:                                                         let _v14: bool = _v12 == _v13;
// REWRITES-NEXT:                                                         if _v14 {
// REWRITES-NEXT:                                                                                 let _v15: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                 let _v16: i32 = unsafe { printf(_v15 as *const i8) };
// REWRITES-NEXT:                                                         } else {
// REWRITES-NEXT:                                                                                 let _v17: *mut i8 = b"fail\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                 let _v18: i32 = unsafe { printf(_v17 as *const i8) };
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
