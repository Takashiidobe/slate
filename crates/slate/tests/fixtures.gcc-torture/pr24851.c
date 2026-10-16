/* We used to handle pointer addition wrongly
   at the time of recombining to an ARRAY_REF
   in the case of
     p + -4B
   where -4B is represented as unsigned.  */

void abort(void);
int  main() {
  int a[10], *p, *q;
  q = &a[1];
  p = &q[-1];
  if (p >= &a[9])
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! We used to handle pointer addition wrongly
// CHECK-NEXT: //! at the time of recombining to an ARRAY_REF
// CHECK-NEXT: //! in the case of
// CHECK-NEXT: //!   p + -4B
// CHECK-NEXT: //! where -4B is represented as unsigned.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut q: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     q = unsafe { (a.0.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) };
// CHECK-NEXT:     p = unsafe { q.offset(-(1 as i32) as isize) };
// CHECK-NEXT:     if p >= unsafe { (a.0.as_mut_ptr() as *mut i32).offset((9 as i32) as isize) } {
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
