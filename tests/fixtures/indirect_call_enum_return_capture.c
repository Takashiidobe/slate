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
// LOWERING-NEXT: struct Parser {
// LOWERING-NEXT:     processor: Option<unsafe extern "C-unwind" fn(i32) -> u32>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn handle_ok({{arg[0-9]+}}: i32) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = Status::E_OK as u32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn handle_fail({{arg[0-9]+}}: i32) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = Status::E_FAIL as u32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut p: Parser = Parser { processor: None };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
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
// LOWERING-NEXT:                 let mut result: aligned::Aligned<aligned::A4, Status> =
// LOWERING-NEXT:                     aligned::Aligned(Status::E_OK);
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> u32> = if {{_v[0-9]+}} {
// LOWERING-NEXT:                     Some(handle_ok)
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     Some(handle_fail)
// LOWERING-NEXT:                 };
// LOWERING-NEXT:                 p.processor = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> u32> = p.processor;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:                 *result = unsafe { std::mem::transmute({{_v[0-9]+}}) };
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u32 = *result as u32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u32 = Status::E_OK as u32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:                     } else {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"fail\n\0".as_ptr() as *mut i8;
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
// REWRITES-NEXT: enum Status {
// REWRITES-NEXT:     E_OK = 0,
// REWRITES-NEXT:     E_FAIL = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Parser {
// REWRITES-NEXT:     processor: Option<unsafe extern "C-unwind" fn(i32) -> u32>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn handle_ok({{arg[0-9]+}}: i32) -> u32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = Status::E_OK as u32;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn handle_fail({{arg[0-9]+}}: i32) -> u32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = Status::E_FAIL as u32;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut p: Parser = Parser { processor: None };
// REWRITES-NEXT:     for i in 0..2 {
// REWRITES-NEXT:         let mut result: aligned::Aligned<aligned::A4, Status> = aligned::Aligned(Status::E_OK);
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = i == 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> u32> = if {{_v[0-9]+}} {
// REWRITES-NEXT:             Some(handle_ok)
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             Some(handle_fail)
// REWRITES-NEXT:         };
// REWRITES-NEXT:         p.processor = {{_v[0-9]+}};
// REWRITES-NEXT:         let {{_v[0-9]+}}: u32 = unsafe { p.processor.unwrap()(5 as i32) };
// REWRITES-NEXT:         *result = unsafe { std::mem::transmute({{_v[0-9]+}}) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: u32 = *result as u32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u32 = Status::E_OK as u32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             unsafe { printf(c"ok\n".as_ptr()) };
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             unsafe { printf(c"fail\n".as_ptr()) };
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
