/* Origin: PR c/1540 from Mattias Lampe <lampe@tu-harburg.de>,
   adapted to a testcase by Joseph Myers <jsm28@cam.ac.uk>.
   GCC 2.95.2 fails, CVS GCC of 2001-01-13 passes.  */
extern void abort(void);
extern void exit(int);

int main(void) {
  int array1[1]    = {1};
  int array2[2][1] = {{1}, {0}};
  if (array1[0] != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Origin: PR c/1540 from Mattias Lampe <lampe@tu-harburg.de>,
// CHECK-NEXT: // adapted to a testcase by Joseph Myers <jsm28@cam.ac.uk>.
// CHECK-NEXT: // GCC 2.95.2 fails, CVS GCC of 2001-01-13 passes.
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut array1: [i32; 1] = [1 as i32];
// CHECK-NEXT:     if (unsafe { *unsafe { (array1.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
