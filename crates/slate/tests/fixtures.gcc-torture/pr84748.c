/* { dg-require-effective-target int128 } */

typedef unsigned __int128 u128;

int  a, c, d;
u128 b;

unsigned long long g0, g1;

void store(unsigned long long a0, unsigned long long a1) {
  g0 = a0;
  g1 = a1;
}

void foo(void) {
  b      += a;
  c       = d != 84347;
  b      /= c;
  u128 x  = b;
  store(x >> 0, x >> 64);
}

int main(void) {
  foo();
  if (g0 != 0 || g1 != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int128 }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u128 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g0: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g1: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn store(mut a0: u64, mut a1: u64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g0 = a0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g1 = a1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u128 = unsafe { b };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u128 = [[V_0]].wrapping_add(((unsafe { a }) as i128) as u128);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = ((unsafe { d }) != (84347 as i32)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u128 = unsafe { b };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u128 = [[V_2]] / (((unsafe { c }) as i128) as u128);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x: u128 = unsafe { b };
// CHECK-NEXT:     store((x >> (0 as i32)) as u64, (x >> (64 as i32)) as u64);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { g0 }) != (((0 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { g1 }) != (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
