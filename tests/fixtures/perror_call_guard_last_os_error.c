#include <stdio.h>

int main(void) {
  int rc = remove("slate_perror_call_guard_missing.tmp");
  if (rc < 0) {
    perror("remove failed");
    return 1;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn perror(_0: *const core::ffi::c_char);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut rc: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate_perror_call_guard_missing.tmp\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate_perror_call_guard_missing.tmp\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { remove({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     rc = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = rc;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"remove failed\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"remove failed\0".as_ptr() as *mut u8;
// LOWERING-NEXT:             unsafe { perror({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn perror(_0: *const core::ffi::c_char);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut rc: i32 = unsafe { remove(c"slate_perror_call_guard_missing.tmp".as_ptr()) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = rc < 0;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { perror(c"remove failed".as_ptr()) };
// REWRITES-NEXT:         __retval = 1;
// REWRITES-NEXT:         std::process::exit(__retval as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     __retval = 0;
// REWRITES-NEXT:     std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
