/* This testcase was being miscompiled, because operand_equal_p
   returned that (unsigned long) d and (long) d are equal.  */
extern void abort(void);
extern void exit(int);

int main(void) {
  double d = -12.0;
  long   l = (d > 10000) ? (unsigned long)d : (long)d;
  if (l != -12)
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
// CHECK-NEXT: // This testcase was being miscompiled, because operand_equal_p
// CHECK-NEXT: // returned that (unsigned long) d and (long) d are equal.
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: f64 = -12.0f64;
// CHECK-NEXT:     let mut l: i64 = (if d > ((10000 as i32) as f64) {
// CHECK-NEXT:         d as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (d as i64) as u64
// CHECK-NEXT:     }) as i64;
// CHECK-NEXT:     if l != (-(12 as i32) as i64) {
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
