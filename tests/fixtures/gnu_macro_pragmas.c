
// LOWERING-DAG: static mut macro_inner: i32 = 11;
// LOWERING-DAG: static mut macro_outer: i32 = 7;
// LOWERING-NOT: poisoned_but_unused

#include <stdio.h>

#define GNU_MACRO_VALUE 7
#pragma push_macro("GNU_MACRO_VALUE")
#undef GNU_MACRO_VALUE
#define GNU_MACRO_VALUE 11
static int macro_inner = GNU_MACRO_VALUE;
#pragma pop_macro("GNU_MACRO_VALUE")
static int macro_outer = GNU_MACRO_VALUE;

#pragma GCC poison poisoned_but_unused

int main(void) {
  printf("%d %d\n", macro_inner, macro_outer);
  return 0;
}

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
// REWRITES-NEXT: static mut macro_inner: i32 = 11;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut macro_outer: i32 = 7;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(c"%d %d\n".as_ptr(), unsafe { macro_inner }, unsafe {
// REWRITES-NEXT:             macro_outer
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
