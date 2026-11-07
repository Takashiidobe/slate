/* Macro definition comments stay with their macro_rules. */

#include <stdio.h>
#include "macro_definition_comments.h"

/* Doubles x.
 * Spans two lines. */
#define DOUBLE(x) ((x) * 2)

#define TRIPLE(x) ((x) * 3) /* triples x */

/* Squares through DOUBLE's neighbour. */
#define SQUARE(x) ((x) * (x))
/* Adds DOUBLE and SQUARE. */
#define BOTH(x) (DOUBLE(x) + SQUARE(x))

#ifdef WIDE_SCALE
/* Wide scale. */
#define SCALE(x) ((x) * 100)
#else
/* Narrow scale. */
#define SCALE(x) ((x) * 10)
#endif

/* Pastes a suffix, so it stays inline. */
#define NAMED(n) value_##n

/* Unrelated helper. */
static int unrelated(int a) {
  return a + 1;
}

int main(void) {
  int value_1 = 4;
  int v = 5;
  /* invocation comment */
  printf("%d %d %d\n", DOUBLE(v), TRIPLE(v), unrelated(v));
  printf("%d %d %d\n", BOTH(v), SCALE(v), CLAMP_HI(v, 3));
  printf("%d\n", NAMED(1));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Macro definition comments stay with their macro_rules.
// CHECK-NEXT: /// Adds DOUBLE and SQUARE.
// CHECK-NEXT: macro_rules! BOTH {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         DOUBLE!($x) + SQUARE!($x)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Clamps v to at most hi.
// CHECK-NEXT: macro_rules! CLAMP_HI {
// CHECK-NEXT:     ($v:expr, $hi:expr) => {
// CHECK-NEXT:         if ($v) > $hi { $hi } else { $v }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Doubles x.
// CHECK-NEXT: /// Spans two lines.
// CHECK-NEXT: macro_rules! DOUBLE {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         ($x) * 2
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Narrow scale.
// CHECK-NEXT: macro_rules! SCALE {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         ($x) * 10
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Squares through DOUBLE's neighbour.
// CHECK-NEXT: macro_rules! SQUARE {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         ($x) * $x
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// triples x
// CHECK-NEXT: macro_rules! TRIPLE {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         ($x) * 3
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Pastes a suffix, so it stays inline.
// CHECK-NEXT: /// Unrelated helper.
// CHECK-NEXT: fn unrelated(a: i32) -> i32 {
// CHECK-NEXT:     a + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let value_1: i32 = 4;
// CHECK-NEXT:     let v: i32 = 5;
// CHECK-NEXT:     // invocation comment
// CHECK-NEXT:     println!("{} {} {}", DOUBLE!(v), TRIPLE!(v), unrelated(v));
// CHECK-NEXT:     println!("{} {} {}", BOTH!(v), SCALE!(v), CLAMP_HI!(v, 3i32));
// CHECK-NEXT:     println!("{}", value_1);
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
