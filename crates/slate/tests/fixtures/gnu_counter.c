#include <stdio.h>

#define NEXT_COUNTER()     __COUNTER__
#define COUNTER_PAIR(a, b) ((a) * 10 + (b))

static int direct_zero = __COUNTER__;
static int macro_one   = NEXT_COUNTER();

#if 0
static int ignored = __COUNTER__;
#endif

static int pair = COUNTER_PAIR(__COUNTER__, NEXT_COUNTER());

int main(void) {
  int local_four = __COUNTER__;
  printf("%d %d %d %d\n", direct_zero, macro_one, pair, local_four);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut direct_zero: i32 = 0;
// CHECK-NEXT: static mut macro_one: i32 = 1;
// CHECK-NEXT: static mut pair: i32 = 23;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let local_four: i32 = 4;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", unsafe { direct_zero }, unsafe { macro_one }, unsafe { pair },
// CHECK-NEXT:         local_four
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
