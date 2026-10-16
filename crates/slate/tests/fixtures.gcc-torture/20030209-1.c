/* { dg-require-stack-size "8*100*100" } */

void abort(void);
void exit(int);

double x[100][100];
int    main() {
  int i;

  i       = 99;
  x[i][0] = 42;
  if (x[99][0] != 42)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-stack-size "8*100*100" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: __SlateAlign16<{{\[\[}}f64; 100]; 100]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 99 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(x.0) as *mut [f64; 100] }.offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_mut_ptr() as *mut f64
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((0 as i32) as isize)
// CHECK-NEXT:         } = (42 as i32) as f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(x.0) as *mut [f64; 100] }
// CHECK-NEXT:                         .offset((99 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .as_mut_ptr() as *mut f64
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != ((42 as i32) as f64)
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
