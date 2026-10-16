void abort(void);

unsigned int buggy(unsigned int *param) {
  unsigned int accu, zero = 0, borrow;
  accu    = -*param;
  borrow  = -(accu > zero);
  *param += accu;
  return borrow;
}

int main(void) {
  unsigned int param  = 1;
  unsigned int borrow = buggy(&param);

  if (param != 0)
    abort();
  if (borrow + 1 != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn buggy(mut param: &mut u32) -> u32 {
// CHECK-NEXT:     let mut accu: u32 = unsafe { *(param as *mut u32) }.wrapping_neg();
// CHECK-NEXT:     let mut zero: u32 = (0 as i32) as u32;
// CHECK-NEXT:     let mut borrow: u32 = -((accu > zero) as i32) as u32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = unsafe { *param };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add(accu);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *param = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     borrow
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut param: u32 = (1 as i32) as u32;
// CHECK-NEXT:     let mut borrow: u32 = buggy(unsafe { &mut (*std::ptr::addr_of_mut!(param)) });
// CHECK-NEXT:     if param != ((0 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if borrow.wrapping_add((1 as i32) as u32) != ((0 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
