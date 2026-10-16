/* PR tree-optimization/65170 */

#ifdef __SIZEOF_INT128__
typedef unsigned __int128      V;
typedef unsigned long long int H;
#else
typedef unsigned long long int V;
typedef unsigned int           H;
#endif

__attribute__((noinline, noclone)) void foo(V b, V c) {
  V a;
  b &= (H)-1;
  c &= (H)-1;
  a  = b * c;
  if (a != 1)
    __builtin_abort();
}

int main() {
  foo(1, 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/65170
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut b: u128, mut c: u128) {
// CHECK-NEXT:     let mut a: u128 = 0 as u128;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u128 = b;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u128 = [[V_0]] & (((-(1 as i32) as i64) as u64) as u128);
// CHECK-NEXT:     b = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u128 = c;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u128 = [[V_2]] & (((-(1 as i32) as i64) as u64) as u128);
// CHECK-NEXT:     c = [[V_3]];
// CHECK-NEXT:     a = b.wrapping_mul(c);
// CHECK-NEXT:     if a != (((1 as i32) as i128) as u128) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(((1 as i32) as i128) as u128, ((1 as i32) as i128) as u128);
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
