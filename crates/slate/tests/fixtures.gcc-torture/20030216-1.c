/* This test requires constant propagation of loads and stores to be
   enabled.  This is only guaranteed at -O2 and higher.  Do not run
   at -O1.  */
/* { dg-skip-if "requires higher optimization" { *-*-* } "-O1" "" } */

void         link_error(void);
const double one = 1.0;
int          main(void) {
#ifdef __OPTIMIZE__
  if ((int)one != 1)
    link_error();
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This test requires constant propagation of loads and stores to be
// CHECK-NEXT: //! enabled.  This is only guaranteed at -O2 and higher.  Do not run
// CHECK-NEXT: //! at -O1.
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-skip-if "requires higher optimization" { *-*-* } "-O1" "" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn link_error();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut one: f64 = 1.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     0 as i32
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
// SLATE-FILECHECK-END
