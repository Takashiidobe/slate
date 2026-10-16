/* Trivially making sure IPA-SRA does not introduce segfaults where they should
   not be.  */

struct bovid {
  float red;
  int   green;
  void *blue;
};

static int __attribute__((noinline)) ox(int fail, struct bovid *cow) {
  int r;
  if (fail)
    r = cow->red;
  else
    r = 0;
  return r;
}

int main(int argc, char *argv[]) {
  int r;

  r = ox((argc > 2000), (void *)0);
  return r;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Trivially making sure IPA-SRA does not introduce segfaults where they should
// CHECK-NEXT: //! not be.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bovid {
// CHECK-NEXT:     red: f32,
// CHECK-NEXT:     green: i32,
// CHECK-NEXT:     blue: *mut (),
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ox(mut fail: i32, mut cow: *mut bovid) -> i32 {
// CHECK-NEXT:     let mut r: i32 = 0 as i32;
// CHECK-NEXT:     if fail != (0 as i32) {
// CHECK-NEXT:         r = (unsafe { (*cow).red }) as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         r = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut r: i32 = ox((argc > (2000 as i32)) as i32, std::ptr::null_mut::<bovid>());
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
