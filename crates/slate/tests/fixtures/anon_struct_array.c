#include <stdio.h>

int main(void) {
  struct {
    int         code;
    const char *message;
    double      confidence;
  } error_log[] = {
      {404, "Not Found", 0.99},
      {500, "Internal Server Error", 0.85},
      {200, "OK", 1.00},
  };

  printf("%d\n", error_log[0].code);
  return 0;
}

// REWRITES-NOT: error_log[0].code

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
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-NEXT:     code: i32,
// LOWERING-NEXT:     __slate_anon_1: *mut i8,
// LOWERING-NEXT:     __slate_anon_2: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut error_log: aligned::Aligned<aligned::A16, [{{anon_[0-9]+}}; 3]> = aligned::Aligned(
// LOWERING-NEXT:         [{{anon_[0-9]+}} {
// LOWERING-NEXT:             code: 0,
// LOWERING-NEXT:             __slate_anon_1: std::ptr::null_mut(),
// LOWERING-NEXT:             __slate_anon_2: 0.0,
// LOWERING-NEXT:         }; 3],
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     *error_log = [
// LOWERING-NEXT:         {{anon_[0-9]+}} {
// LOWERING-NEXT:             code: 404,
// LOWERING-NEXT:             __slate_anon_1: b"Not Found\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             __slate_anon_2: 0.99,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         {{anon_[0-9]+}} {
// LOWERING-NEXT:             code: 500,
// LOWERING-NEXT:             __slate_anon_1: b"Internal Server Error\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             __slate_anon_2: 0.85,
// LOWERING-NEXT:         },
// LOWERING-NEXT:         {{anon_[0-9]+}} {
// LOWERING-NEXT:             code: 200,
// LOWERING-NEXT:             __slate_anon_1: b"OK\0".as_ptr() as *mut i8,
// LOWERING-NEXT:             __slate_anon_2: 1.0,
// LOWERING-NEXT:         },
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = error_log[({{_v[0-9]+}} as usize)].code;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering
