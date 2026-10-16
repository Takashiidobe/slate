/* This testcase was miscompiled on IA-32, because fold-const
   assumed associate_trees is always done on PLUS_EXPR.  */

extern void abort(void);
extern void exit(int);

void check(unsigned int m) {
  if (m != (unsigned int)-1)
    abort();
}

unsigned int n = 1;

int main(void) {
  unsigned int m;
  m = (1 | (2 - n)) | (-n);
  check(m);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This testcase was miscompiled on IA-32, because fold-const
// CHECK-NEXT: //! assumed associate_trees is always done on PLUS_EXPR.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check(mut m: u32) {
// CHECK-NEXT:     if m != (-(1 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: u32 = (1 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut m: u32 = ((1 as i32) as u32)
// CHECK-NEXT:         | ((2 as i32) as u32).wrapping_sub(unsafe { n })
// CHECK-NEXT:         | unsafe { n }.wrapping_neg();
// CHECK-NEXT:     check(m);
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
