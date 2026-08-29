#include <stdio.h>

typedef enum { XML_STATUS_ERROR = 0, XML_STATUS_OK = 1 } XML_Status;

struct Case {
  XML_Status expectedStatus;
};

int main(void) {
  struct Case cases[2];
  cases[0].expectedStatus = XML_STATUS_OK;
  cases[1].expectedStatus = XML_STATUS_ERROR;

  for (int i = 0; i < 2; i++) {
    const XML_Status expectedStatus = cases[i].expectedStatus;
    if (expectedStatus == XML_STATUS_OK) {
      printf("ok\n");
    } else {
      printf("error\n");
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
// LOWERING-NEXT: enum XML_Status {
// LOWERING-NEXT:     XML_STATUS_ERROR = 0,
// LOWERING-NEXT:     XML_STATUS_OK = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Case {
// LOWERING-NEXT:     expectedStatus: XML_Status,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [Case; 2]> = aligned::Aligned([Case { expectedStatus: XML_Status::XML_STATUS_ERROR }; 2]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u32 = XML_Status::XML_STATUS_OK as u32;
// LOWERING-NEXT:     let _v2: i64 = 0;
// LOWERING-NEXT:     cases[(_v2 as usize)].expectedStatus = XML_Status::XML_STATUS_OK;
// LOWERING-NEXT:     let _v3: u32 = XML_Status::XML_STATUS_ERROR as u32;
// LOWERING-NEXT:     let _v4: i64 = 1;
// LOWERING-NEXT:     cases[(_v4 as usize)].expectedStatus = XML_Status::XML_STATUS_ERROR;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v5: i32 = 0;
// LOWERING-NEXT:         i = _v5;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v6: i32 = i;
// LOWERING-NEXT:             let _v7: i32 = 2;
// LOWERING-NEXT:             let _v8: bool = _v6 < _v7;
// LOWERING-NEXT:             if !_v8 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut expectedStatus: aligned::Aligned<aligned::A4, XML_Status> = aligned::Aligned(XML_Status::XML_STATUS_ERROR);
// LOWERING-NEXT:                 let _v9: i32 = i;
// LOWERING-NEXT:                 let _v10: i64 = _v9 as i64;
// LOWERING-NEXT:                 let _v11: u32 = cases[(_v10 as usize)].expectedStatus as u32;
// LOWERING-NEXT:                 *expectedStatus = unsafe { std::mem::transmute(_v11) };
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v12: u32 = *expectedStatus as u32;
// LOWERING-NEXT:                     let _v13: u32 = XML_Status::XML_STATUS_OK as u32;
// LOWERING-NEXT:                     let _v14: bool = _v12 == _v13;
// LOWERING-NEXT:                     if _v14 {
// LOWERING-NEXT:                         let _v15: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let _v16: i32 = unsafe { printf(_v15 as *const i8) };
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let _v17: *mut i8 = b"error\n\0".as_ptr() as *mut i8;
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
// REWRITES-NEXT: enum XML_Status {
// REWRITES-NEXT:     XML_STATUS_ERROR = 0,
// REWRITES-NEXT:     XML_STATUS_OK = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Case {
// REWRITES-NEXT:     expectedStatus: XML_Status,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut cases: aligned::Aligned<aligned::A4, [Case; 2]> = aligned::Aligned([Case { expectedStatus: XML_Status::XML_STATUS_ERROR }; 2]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: u32 = XML_Status::XML_STATUS_OK as u32;
// REWRITES-NEXT: let _v2: i64 = 0;
// REWRITES-NEXT: cases[(_v2 as usize)].expectedStatus = XML_Status::XML_STATUS_OK;
// REWRITES-NEXT: let _v3: u32 = XML_Status::XML_STATUS_ERROR as u32;
// REWRITES-NEXT: let _v4: i64 = 1;
// REWRITES-NEXT: cases[(_v4 as usize)].expectedStatus = XML_Status::XML_STATUS_ERROR;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v7: i32 = 2;
// REWRITES-NEXT:                     if !(i < _v7) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let mut expectedStatus: aligned::Aligned<aligned::A4, XML_Status> = aligned::Aligned(XML_Status::XML_STATUS_ERROR);
// REWRITES-NEXT:                                     *expectedStatus = unsafe { std::mem::transmute(cases[((i as i64) as usize)].expectedStatus as u32) };
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let _v12: u32 = *expectedStatus as u32;
// REWRITES-NEXT:                                                         let _v13: u32 = XML_Status::XML_STATUS_OK as u32;
// REWRITES-NEXT:                                                         let _v14: bool = _v12 == _v13;
// REWRITES-NEXT:                                                         if _v14 {
// REWRITES-NEXT:                                                                                 let _v15: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                 let _v16: i32 = unsafe { printf(_v15 as *const i8) };
// REWRITES-NEXT:                                                         } else {
// REWRITES-NEXT:                                                                                 let _v17: *mut i8 = b"error\n\0".as_ptr() as *mut i8;
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
