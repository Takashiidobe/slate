/* PR tree-optimization/54471 */

#ifdef __SIZEOF_INT128__
#define T __int128
#else
#define T long long
#endif

extern void abort(void);

__attribute__((noinline)) unsigned T foo(T ixi, unsigned ctr) {
  unsigned T irslt = 1;
  T          ix    = ixi;

  for (; ctr; ctr--) {
    irslt *= ix;
    ix    *= ix;
  }

  if (irslt != 14348907)
    abort();
  return irslt;
}

int main() {
  unsigned T res;

  res = foo(3, 4);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/54471
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut ix: i128, mut ctr: u32) -> u128 {
// CHECK-NEXT:     let mut irslt: u128 = ((1 as i32) as i128) as u128;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ctr != (0 as u32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u128 = irslt;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u128 = [[V_0]].wrapping_mul(ix as u128);
// CHECK-NEXT:         irslt = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i128 = ix;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i128 = [[V_2]] * ix;
// CHECK-NEXT:         ix = [[V_3]];
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = ctr;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: u32 = [[V_4]].wrapping_sub((1 as i32) as u32);
// CHECK-NEXT:         ctr = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if irslt != (((14348907 as i32) as i128) as u128) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     irslt
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo((3 as i32) as i128, (4 as i32) as u32);
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
