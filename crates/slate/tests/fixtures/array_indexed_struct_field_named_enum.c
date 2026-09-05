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
// COMMON-LOWERING-NEXT: enum Status {
// COMMON-LOWERING-NEXT:     E_OK = 0,
// COMMON-LOWERING-NEXT:     E_FAIL = 1,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct CaseData {
// COMMON-LOWERING-NEXT:     input: i32,
// COMMON-LOWERING-NEXT:     expected: Status,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:         [CaseData {
// COMMON-LOWERING-NEXT:             input: 0,
// COMMON-LOWERING-NEXT:             expected: Status::E_OK,
// COMMON-LOWERING-NEXT:         }; 2],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let mut actual: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [CaseData; 2] = [
// COMMON-LOWERING-NEXT:         CaseData {
// COMMON-LOWERING-NEXT:             input: 1,
// COMMON-LOWERING-NEXT:             expected: Status::E_OK,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         CaseData {
// COMMON-LOWERING-NEXT:             input: 2,
// COMMON-LOWERING-NEXT:             expected: Status::E_FAIL,
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     *cases = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     *actual = Status::E_OK;
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
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u32 = *actual as u32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u32 = cases[({{__v[0-9]+}} as usize)].expected as u32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                             unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                     } else {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                             unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
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
// LOWERING-X86_64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [CaseData; 2]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = b"mismatch %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut i8 = b"match %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [CaseData; 2]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = b"mismatch %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                         let {{__v[0-9]+}}: *mut u8 = b"match %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: enum Status {
// COMMON-REWRITES-NEXT:     E_OK = 0,
// COMMON-REWRITES-NEXT:     E_FAIL = 1,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct CaseData {
// COMMON-REWRITES-NEXT:     input: i32,
// COMMON-REWRITES-NEXT:     expected: Status,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:         [CaseData {
// COMMON-REWRITES-NEXT:             input: 0,
// COMMON-REWRITES-NEXT:             expected: Status::E_OK,
// COMMON-REWRITES-NEXT:         }; 2],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let mut actual: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: [CaseData; 2] = [
// COMMON-REWRITES-NEXT:         CaseData {
// COMMON-REWRITES-NEXT:             input: 1,
// COMMON-REWRITES-NEXT:             expected: Status::E_OK,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         CaseData {
// COMMON-REWRITES-NEXT:             input: 2,
// COMMON-REWRITES-NEXT:             expected: Status::E_FAIL,
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     *cases = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     *actual = Status::E_OK;
// COMMON-REWRITES-NEXT:     let mut i: i32 = 0;
// COMMON-REWRITES-NEXT:     while i < 2 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (*actual as u32) != (cases[((i as i64) as usize)].expected as u32);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             unsafe { printf(c"mismatch %d\n".as_ptr(), i) };
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             unsafe { printf(c"match %d\n".as_ptr(), i) };
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         i += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [CaseData; 2]> = aligned::Aligned(
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut cases: aligned::Aligned<aligned::A4, [CaseData; 2]> = aligned::Aligned(
// SLATE-FILECHECK-END rewrites-aarch64-gnu
