/* Origin: PR c/410 from Jan Echternach
   <jan.echternach@informatik.uni-rostock.de>,
   adapted to a testcase by Joseph Myers <jsm28@cam.ac.uk>.
*/

extern void exit(int);

static void foo(void) {
  struct {
    long a;
    char b[1];
  } x = {2, {0}};
}

int main(void) {
  int tmp;
  foo();
  tmp = 1;
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: PR c/410 from Jan Echternach
// CHECK-NEXT: //! <jan.echternach@informatik.uni-rostock.de>,
// CHECK-NEXT: //! adapted to a testcase by Joseph Myers <jsm28@cam.ac.uk>.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: [i8; 1],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
