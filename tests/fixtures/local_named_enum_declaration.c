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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum XML_Status {
// COMMON-LOWERING-NEXT:     XML_STATUS_ERROR = 0,
// COMMON-LOWERING-NEXT:     XML_STATUS_OK = 1,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Case {
// COMMON-LOWERING-NEXT:     expectedStatus: XML_Status,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [Case; 2]> = aligned::Aligned(
// COMMON-LOWERING-NEXT:         [Case {
// COMMON-LOWERING-NEXT:             expectedStatus: XML_Status::XML_STATUS_ERROR,
// COMMON-LOWERING-NEXT:         }; 2],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = XML_Status::XML_STATUS_OK as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     cases[({{__v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_OK;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = XML_Status::XML_STATUS_ERROR as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     cases[({{__v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_ERROR;
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let mut expectedStatus: aligned::Aligned<aligned::A4, XML_Status> =
// COMMON-LOWERING-NEXT:                     aligned::Aligned(XML_Status::XML_STATUS_ERROR);
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u32 = cases[({{__v[0-9]+}} as usize)].expectedStatus as u32;
// COMMON-LOWERING-NEXT:                 *expectedStatus = unsafe { std::mem::transmute({{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u32 = *expectedStatus as u32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u32 = XML_Status::XML_STATUS_OK as u32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:                     } else {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = b"error\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = b"ok\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = b"error\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum XML_Status {
// COMMON-REWRITES-NEXT:     XML_STATUS_ERROR = 0,
// COMMON-REWRITES-NEXT:     XML_STATUS_OK = 1,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Case {
// COMMON-REWRITES-NEXT:     expectedStatus: XML_Status,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [Case; 2]> = aligned::Aligned(
// COMMON-REWRITES-NEXT:         [Case {
// COMMON-REWRITES-NEXT:             expectedStatus: XML_Status::XML_STATUS_ERROR,
// COMMON-REWRITES-NEXT:         }; 2],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:     cases[({{__v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_OK;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-NEXT:     cases[({{__v[0-9]+}} as usize)].expectedStatus = XML_Status::XML_STATUS_ERROR;
// COMMON-REWRITES-NEXT:     for i in 0..2 {
// COMMON-REWRITES-NEXT:         let mut expectedStatus: aligned::Aligned<aligned::A4, XML_Status> =
// COMMON-REWRITES-NEXT:             aligned::Aligned(XML_Status::XML_STATUS_ERROR);
// COMMON-REWRITES-NEXT:         *expectedStatus =
// COMMON-REWRITES-NEXT:             unsafe { std::mem::transmute(cases[((i as i64) as usize)].expectedStatus as u32) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u32 = *expectedStatus as u32;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u32 = XML_Status::XML_STATUS_OK as u32;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             unsafe { printf(c"ok\n".as_ptr()) };
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             unsafe { printf(c"error\n".as_ptr()) };
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
