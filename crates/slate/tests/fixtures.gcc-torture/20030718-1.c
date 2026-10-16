/* PR c/10320
   The function temp was not being emitted in a prerelease of 3.4 20030406.
   Contributed by pinskia@physics.uc.edu */

static inline void temp();
int                main() {
  temp();
  return 0;
}
static void temp() {}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/10320
// CHECK-NEXT: //! The function temp was not being emitted in a prerelease of 3.4 20030406.
// CHECK-NEXT: //! Contributed by pinskia@physics.uc.edu
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     temp();
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn temp() {}
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
