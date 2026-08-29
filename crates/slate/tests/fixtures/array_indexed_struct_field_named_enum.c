#include <stdio.h>

typedef enum { E_OK = 0, E_FAIL = 1 } Status;

int main(void) {
  struct CaseData {
    int    input;
    Status expected;
  };
  struct CaseData cases[2] = {{1, E_OK}, {2, E_FAIL}};
  Status          actual   = E_OK;
  for (int i = 0; i < 2; i++) {
    if (actual != cases[i].expected) {
      printf("mismatch %d\n", i);
    } else {
      printf("match %d\n", i);
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
// LOWERING-NEXT: struct CaseData {
// LOWERING-NEXT:     input: i32,
// LOWERING-NEXT:     expected: Status,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [CaseData; 2]> = aligned::Aligned([CaseData { input: 0, expected: Status::E_OK }; 2]);
// LOWERING-NEXT:     let mut actual: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     *cases = [CaseData { input: 1, expected: Status::E_OK }, CaseData { input: 2, expected: Status::E_FAIL }];
// LOWERING-NEXT:     let _v1: u32 = 0;
// LOWERING-NEXT:     *actual = Status::E_OK;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v2: i32 = 0;
// LOWERING-NEXT:         i = _v2;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v3: i32 = i;
// LOWERING-NEXT:             let _v4: i32 = 2;
// LOWERING-NEXT:             let _v5: bool = _v3 < _v4;
// LOWERING-NEXT:             if !_v5 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v6: u32 = *actual as u32;
// LOWERING-NEXT:                     let _v7: i32 = i;
// LOWERING-NEXT:                     let _v8: i64 = _v7 as i64;
// LOWERING-NEXT:                     let _v9: u32 = cases[(_v8 as usize)].expected as u32;
// LOWERING-NEXT:                     let _v10: bool = _v6 != _v9;
// LOWERING-NEXT:                     if _v10 {
// LOWERING-NEXT:                         let _v11: *mut i8 = b"mismatch %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let _v12: i32 = i;
// LOWERING-NEXT:                         let _v13: i32 = unsafe { printf(_v11 as *const i8, _v12) };
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let _v14: *mut i8 = b"match %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let _v15: i32 = i;
// LOWERING-NEXT:                         let _v16: i32 = unsafe { printf(_v14 as *const i8, _v15) };
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v17: i32 = i;
// LOWERING-NEXT:             let _v18: i32 = _v17 + 1;
// LOWERING-NEXT:             i = _v18;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v19: i32 = 0;
// LOWERING-NEXT:     __retval = _v19;
// LOWERING-NEXT:     let _v20: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v20 as i32);
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
// REWRITES-NEXT: struct CaseData {
// REWRITES-NEXT:     input: i32,
// REWRITES-NEXT:     expected: Status,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut cases: aligned::Aligned<aligned::A16, [CaseData; 2]> = aligned::Aligned([CaseData { input: 0, expected: Status::E_OK }; 2]);
// REWRITES-NEXT: let mut actual: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *cases = [CaseData { input: 1, expected: Status::E_OK }, CaseData { input: 2, expected: Status::E_FAIL }];
// REWRITES-NEXT: let _v1: u32 = 0;
// REWRITES-NEXT: *actual = Status::E_OK;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v4: i32 = 2;
// REWRITES-NEXT:                     if !(i < _v4) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let _v10: bool = (*actual as u32) != (cases[((i as i64) as usize)].expected as u32);
// REWRITES-NEXT:                                                         if _v10 {
// REWRITES-NEXT:                                                                                 let _v11: *mut i8 = b"mismatch %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                 let _v13: i32 = unsafe { printf(_v11 as *const i8, i) };
// REWRITES-NEXT:                                                         } else {
// REWRITES-NEXT:                                                                                 let _v14: *mut i8 = b"match %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                 let _v16: i32 = unsafe { printf(_v14 as *const i8, i) };
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
