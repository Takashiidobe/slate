#include <stdio.h>

int twice(int n) { return n * 2; }

static int folded(int n) {
  /* leading folded */
  int a = twice(n); /* trailing folded */
  /* leading folded return */
  return a + 1;
}

static int zeroed(int n) {
  int s;
  int j;
  s = 0; /* trailing zeroed init */
  /* leading zeroed loop */
  for (j = 0; j < n; j++) {
    s += j;
  }
  /* trailing zeroed return */
  return s;
}

static int dispatched(int n) {
  int acc = 0; /* trailing dispatched acc */
  int tmp;     /* trailing dispatched tmp */
top:
  /* leading dispatched add */
  acc += n;
  tmp = acc * 2; /* trailing dispatched assign */
  if (acc < 100) goto top;
  /* leading dispatched return */
  return tmp;
}

int main(void) {
  printf("%d %d %d\n", folded(3), zeroed(5), dispatched(7));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn twice(n: i32) -> i32 {
// CHECK-NEXT:     n * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn folded(n: i32) -> i32 {
// CHECK-NEXT:     // leading folded
// CHECK-NEXT:     let a: i32 = twice(n); // trailing folded
// CHECK-NEXT:     // leading folded return
// CHECK-NEXT:     a + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn zeroed(n: i32) -> i32 {
// CHECK-NEXT:     // trailing zeroed init
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     // leading zeroed loop
// CHECK-NEXT:     for j in 0..n {
// CHECK-NEXT:         s += j;
// CHECK-NEXT:     }
// CHECK-NEXT:     // trailing zeroed return
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dispatched(n: i32) -> i32 {
// CHECK-NEXT:     // trailing dispatched tmp
// CHECK-NEXT:     let mut tmp: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     // trailing dispatched acc
// CHECK-NEXT:     let mut acc: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         acc = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             acc += n; // leading dispatched add
// CHECK-NEXT:             tmp = acc * 2; // trailing dispatched assign
// CHECK-NEXT:             if !(acc < 100) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         // leading dispatched return
// CHECK-NEXT:         return tmp;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         folded(3 as i32),
// CHECK-NEXT:         zeroed(5 as i32),
// CHECK-NEXT:         dispatched(7 as i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
