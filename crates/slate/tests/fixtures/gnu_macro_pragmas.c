
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut macro_inner: i32 = 11;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut macro_outer: i32 = 7;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, unsafe { macro_inner }, unsafe { macro_outer }) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites

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
