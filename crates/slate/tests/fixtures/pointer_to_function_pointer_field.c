#include <stdio.h>
#include <stdlib.h>

typedef void (*test_fn)(void);

typedef struct {
  test_fn *tests;
  int      ntests;
} suite_t;

static int last_ran = -1;

static void test_a(void) { last_ran = 0; }

static void test_b(void) { last_ran = 1; }

int main(void) {
  suite_t suite;
  suite.tests    = malloc(2 * sizeof(test_fn));
  suite.tests[0] = test_a;
  suite.tests[1] = test_b;
  suite.ntests   = 2;

  for (int i = 0; i < suite.ntests; i++) {
    suite.tests[i]();
    printf("%d\n", last_ran);
  }

  free(suite.tests);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     tests: *mut Option<unsafe extern "C-unwind" fn()>,
// CHECK-NEXT:     ntests: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut last_ran: i32 = -1;
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_a")]
// CHECK-NEXT: extern "C-unwind" fn test_a() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         last_ran = 0;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.test_b")]
// CHECK-NEXT: extern "C-unwind" fn test_b() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         last_ran = 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut suite: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     suite.tests = (unsafe { malloc(16u64) })
// CHECK-NEXT:         as *mut Option<unsafe extern "C-unwind" fn()>;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *suite.tests.offset(0 as isize) = Some(test_a);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *suite.tests.offset(1 as isize) = Some(test_b);
// CHECK-NEXT:     }
// CHECK-NEXT:     suite.ntests = 2;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < suite.ntests {
// CHECK-NEXT:         unsafe { unsafe { *suite.tests.offset(i as isize) }.unwrap()() };
// CHECK-NEXT:         println!("{}", unsafe { last_ran });
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(suite.tests as *mut ()) };
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
