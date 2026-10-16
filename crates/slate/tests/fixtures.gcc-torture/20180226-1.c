/* PR rtl-optimization/83496 */
/* Reported by Hauke Mehrtens <gcc@hauke-m.de> */

extern void abort(void);

typedef unsigned long mp_digit;

typedef struct {
  int       used, alloc, sign;
  mp_digit *dp;
} mp_int;

int mytest(mp_int *a, mp_digit b) __attribute__((noclone, noinline));

int mytest(mp_int *a, mp_digit b) {
  if (a->sign == 1)
    return -1;
  if (a->used > 1)
    return 1;
  if (a->dp[0] > b)
    return 1;
  if (a->dp[0] < b)
    return -1;
  return 0;
}

int main(void) {
  mp_int i = {2, 0, -1};
  if (mytest(&i, 0) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/83496
// CHECK-NEXT: //!
// CHECK-NEXT: //! Reported by Hauke Mehrtens <gcc@hauke-m.de>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     used: i32,
// CHECK-NEXT:     alloc: i32,
// CHECK-NEXT:     sign: i32,
// CHECK-NEXT:     dp: *mut u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mytest(mut a: &[[SLATERECORD_0]], mut b: u64) -> i32 {
// CHECK-NEXT:     if (unsafe { (*(a as *const [[SLATERECORD_0]])).sign }) == (1 as i32) {
// CHECK-NEXT:         return -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*(a as *const [[SLATERECORD_0]])).used }) > (1 as i32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*(a as *const [[SLATERECORD_0]])).dp }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) > b
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*(a as *const [[SLATERECORD_0]])).dp }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) < b
// CHECK-NEXT:     {
// CHECK-NEXT:         return -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].used = 2 as i32;
// CHECK-NEXT:         [[T_0]].alloc = 0 as i32;
// CHECK-NEXT:         [[T_0]].sign = -(1 as i32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     if mytest(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(i)) },
// CHECK-NEXT:         ((0 as i32) as i64) as u64,
// CHECK-NEXT:     ) != (1 as i32)
// CHECK-NEXT:     {
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
