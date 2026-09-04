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
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum XML_Status {
// LOWERING-NEXT:     XML_STATUS_ERROR = 0,
// LOWERING-NEXT:     XML_STATUS_OK = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Case {
// LOWERING-NEXT:     expectedStatus: XML_Status,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [Case; 2]> = aligned::Aligned(
// LOWERING-NEXT:         [Case {
// LOWERING-NEXT:             expectedStatus: XML_Status::XML_STATUS_ERROR,
// LOWERING-NEXT:         }; 2],
// LOWERING-NEXT:     );
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
// LOWERING-NEXT:                 let mut expectedStatus: aligned::Aligned<aligned::A4, XML_Status> =
// LOWERING-NEXT:                     aligned::Aligned(XML_Status::XML_STATUS_ERROR);
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
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"error\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
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
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum XML_Status {
// REWRITES-NEXT:     XML_STATUS_ERROR = 0,
// REWRITES-NEXT:     XML_STATUS_OK = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Case {
// REWRITES-NEXT:     expectedStatus: XML_Status,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [Case; 2]> = aligned::Aligned(
// REWRITES-NEXT:         [Case {
// REWRITES-NEXT:             expectedStatus: XML_Status::XML_STATUS_ERROR,
// REWRITES-NEXT:         }; 2],
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     cases[({{_v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_OK;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     cases[({{_v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_ERROR;
// REWRITES-NEXT:     for i in 0..2 {
// REWRITES-NEXT:         let mut expectedStatus: aligned::Aligned<aligned::A4, XML_Status> =
// REWRITES-NEXT:             aligned::Aligned(XML_Status::XML_STATUS_ERROR);
// REWRITES-NEXT:         *expectedStatus =
// REWRITES-NEXT:             unsafe { std::mem::transmute(cases[((i as i64) as usize)].expectedStatus as u32) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: u32 = *expectedStatus as u32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u32 = XML_Status::XML_STATUS_OK as u32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             unsafe { printf(c"ok\n".as_ptr()) };
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             unsafe { printf(c"error\n".as_ptr()) };
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
