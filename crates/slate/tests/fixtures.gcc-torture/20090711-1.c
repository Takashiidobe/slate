/* Used to be miscompiled at -O0 due to incorrect choice of sign extension
   vs. zero extension.  __attribute__ ((noinline)) added to try to make it
   fail at higher optimization levels too.  */

extern void abort(void);

long long __attribute__((noinline)) div(long long val) { return val / 32768; }

int main(void) {
  long long d1 = -990000000;
  long long d2 = div(d1);
  if (d2 != -30212)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Used to be miscompiled at -O0 due to incorrect choice of sign extension
// CHECK-NEXT: //! vs. zero extension.  __attribute__ ((noinline)) added to try to make it
// CHECK-NEXT: //! fail at higher optimization levels too.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn div(mut val: i64) -> i64 {
// CHECK-NEXT:     val / ((32768 as i32) as i64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d1: i64 = -(990000000 as i32) as i64;
// CHECK-NEXT:     let mut d2: i64 = div(d1);
// CHECK-NEXT:     if d2 != (-(30212 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
