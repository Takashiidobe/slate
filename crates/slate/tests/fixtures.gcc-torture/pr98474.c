/* PR tree-optimization/98474 */

#ifdef __SIZEOF_INT128__
typedef __uint128_t T;
#define N (__SIZEOF_INT128__ * __CHAR_BIT__ / 2)
#else
typedef unsigned long long T;
#define N (__SIZEOF_LONG_LONG__ * __CHAR_BIT__ / 2)
#endif

__attribute__((noipa)) void foo(T *x) { *x += ((T)1) << (N + 1); }

int main() {
  T a = ((T)1) << (N + 1);
  T b = a;
  T n;
  foo(&b);
  n = b;
  while (n >= a)
    n -= a;
  if ((int)(n >> N) != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/98474
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &mut u128) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u128 = unsafe { *x };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u128 = [[V_0]].wrapping_add(
// CHECK-NEXT:         (((1 as i32) as i128) as u128) << (16 as i32) * (8 as i32) / (2 as i32) + (1 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *x = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u128 =
// CHECK-NEXT:         (((1 as i32) as i128) as u128) << (16 as i32) * (8 as i32) / (2 as i32) + (1 as i32);
// CHECK-NEXT:     let mut b: u128 = a;
// CHECK-NEXT:     let mut n: u128 = 0 as u128;
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(b)) });
// CHECK-NEXT:     n = b;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while n >= a {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u128 = n;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u128 = [[V_2]].wrapping_sub(a);
// CHECK-NEXT:         n = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((n >> (16 as i32) * (8 as i32) / (2 as i32)) as u32) as i32) != (0 as i32) {
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
