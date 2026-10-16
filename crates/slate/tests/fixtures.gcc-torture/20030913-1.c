/* Assignments via pointers pointing to global variables were being killed
   by SSA-DCE.  Test contributed by Paul Brook <paul@nowt.org>  */

void abort(void);
void exit(int);

int g;

void fn2(int **q) { *q = &g; }

void test() {
  int *p;

  fn2(&p);

  *p = 42;
}

int main() {
  test();
  if (g != 42)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Assignments via pointers pointing to global variables were being killed
// CHECK-NEXT: //! by SSA-DCE.  Test contributed by Paul Brook <paul@nowt.org>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2(mut q: &mut *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(q as *mut *mut i32) = unsafe { std::ptr::addr_of_mut!(g) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test() {
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     fn2(unsafe { &mut (*std::ptr::addr_of_mut!(p)) });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = 42 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test();
// CHECK-NEXT:     if (unsafe { g }) != (42 as i32) {
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
