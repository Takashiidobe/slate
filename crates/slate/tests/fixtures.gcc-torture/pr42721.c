/* PR c/42721 */

extern void abort(void);

static unsigned long long foo(unsigned long long x, unsigned long long y) {
  return x / y;
}

static int a, b;

int main(void) {
  unsigned long long c  = 1;
  b                    ^= c && (foo(a, -1ULL) != 1L);
  if (b != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/42721
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u64, mut y: u64) -> u64 {
// CHECK-NEXT:     x / y
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: u64 = ((1 as i32) as i64) as u64;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if c != (0 as u64) {
// CHECK-NEXT:         [[V_1]] =
// CHECK-NEXT:             foo(((unsafe { a }) as i64) as u64, (1 as u64).wrapping_neg()) != ((1 as i64) as u64);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_0]] ^ ([[V_1]] as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b }) != (1 as i32) {
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
