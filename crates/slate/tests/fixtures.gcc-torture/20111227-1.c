/* PR rtl-optimization/51667 */
/* Testcase by Uros Bizjak <ubizjak@gmail.com> */

extern void abort(void);

void __attribute__((noinline, noclone)) bar(int a) {
  if (a != -1)
    abort();
}

void __attribute__((noinline, noclone)) foo(short *a, int t) {
  short r = *a;

  if (t)
    bar((unsigned short)r);
  else
    bar((signed short)r);
}

short v = -1;

int main(void) {
  foo(&v, 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/51667
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by Uros Bizjak <ubizjak@gmail.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut a: i32) {
// CHECK-NEXT:     if a != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: *mut i16, mut t: i32) {
// CHECK-NEXT:     let mut r: i16 = unsafe { *a };
// CHECK-NEXT:     if t != (0 as i32) {
// CHECK-NEXT:         bar(((r as u16) as u32) as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         bar(r as i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i16 = -(1 as i32) as i16;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(unsafe { std::ptr::addr_of_mut!(v) }, 0 as i32);
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
