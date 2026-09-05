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
// LOWERING-NEXT: enum Status {
// LOWERING-NEXT:     E_OK = 0,
// LOWERING-NEXT:     E_FAIL = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct CaseData {
// LOWERING-NEXT:     input: i32,
// LOWERING-NEXT:     expected: Status,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [CaseData; 2]> = aligned::Aligned(
// LOWERING-NEXT:         [CaseData {
// LOWERING-NEXT:             input: 0,
// LOWERING-NEXT:             expected: Status::E_OK,
// LOWERING-NEXT:         }; 2],
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let mut actual: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [CaseData; 2] = [
// LOWERING-NEXT:         CaseData {
// LOWERING-NEXT:             input: 1,
// LOWERING-NEXT:             expected: Status::E_OK,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         CaseData {
// LOWERING-NEXT:             input: 2,
// LOWERING-NEXT:             expected: Status::E_FAIL,
// LOWERING-NEXT:         },
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     *cases = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     *actual = Status::E_OK;
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
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u32 = *actual as u32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u32 = cases[({{_v[0-9]+}} as usize)].expected as u32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"mismatch %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"match %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: enum Status {
// REWRITES-NEXT:     E_OK = 0,
// REWRITES-NEXT:     E_FAIL = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct CaseData {
// REWRITES-NEXT:     input: i32,
// REWRITES-NEXT:     expected: Status,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut cases: aligned::Aligned<aligned::A16, [CaseData; 2]> = aligned::Aligned(
// REWRITES-NEXT:         [CaseData {
// REWRITES-NEXT:             input: 0,
// REWRITES-NEXT:             expected: Status::E_OK,
// REWRITES-NEXT:         }; 2],
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let mut actual: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// REWRITES-NEXT:     let {{_v[0-9]+}}: [CaseData; 2] = [
// REWRITES-NEXT:         CaseData {
// REWRITES-NEXT:             input: 1,
// REWRITES-NEXT:             expected: Status::E_OK,
// REWRITES-NEXT:         },
// REWRITES-NEXT:         CaseData {
// REWRITES-NEXT:             input: 2,
// REWRITES-NEXT:             expected: Status::E_FAIL,
// REWRITES-NEXT:         },
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     *cases = {{_v[0-9]+}};
// REWRITES-NEXT:     *actual = Status::E_OK;
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     while i < 2 {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (*actual as u32) != (cases[((i as i64) as usize)].expected as u32);
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             unsafe { printf(c"mismatch %d\n".as_ptr(), i) };
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             unsafe { printf(c"match %d\n".as_ptr(), i) };
// REWRITES-NEXT:         }
// REWRITES-NEXT:         i += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
