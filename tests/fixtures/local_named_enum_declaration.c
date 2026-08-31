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
// LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [Case; 2]> = aligned::Aligned([Case { expectedStatus: XML_Status::XML_STATUS_ERROR }; 2]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = XML_Status::XML_STATUS_OK as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     cases[({{_v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_OK;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = XML_Status::XML_STATUS_ERROR as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     cases[({{_v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_ERROR;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut expectedStatus: aligned::Aligned<aligned::A4, XML_Status> = aligned::Aligned(XML_Status::XML_STATUS_ERROR);
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u32 = cases[({{_v[0-9]+}} as usize)].expectedStatus as u32;
// LOWERING-NEXT:                 *expectedStatus = unsafe { std::mem::transmute({{_v[0-9]+}}) };
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u32 = *expectedStatus as u32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u32 = XML_Status::XML_STATUS_OK as u32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"error\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: let mut cases: aligned::Aligned<aligned::A4, [Case; 2]> = aligned::Aligned([Case { expectedStatus: XML_Status::XML_STATUS_ERROR }; 2]);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = XML_Status::XML_STATUS_OK as u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: cases[({{_v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_OK;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = XML_Status::XML_STATUS_ERROR as u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: cases[({{_v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_ERROR;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:                     if !(i < {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let mut expectedStatus: aligned::Aligned<aligned::A4, XML_Status> = aligned::Aligned(XML_Status::XML_STATUS_ERROR);
// REWRITES-NEXT:                                     *expectedStatus = unsafe { std::mem::transmute(cases[((i as i64) as usize)].expectedStatus as u32) };
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let {{_v[0-9]+}}: u32 = *expectedStatus as u32;
// REWRITES-NEXT:                                                         let {{_v[0-9]+}}: u32 = XML_Status::XML_STATUS_OK as u32;
// REWRITES-NEXT:                                                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:                                                         if {{_v[0-9]+}} {
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:                                                         } else {
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: *mut i8 = b"error\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
