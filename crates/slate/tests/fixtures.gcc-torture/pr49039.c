/* PR tree-optimization/49039 */
extern void abort(void);
int         cnt;

__attribute__((noinline, noclone)) void foo(unsigned int x, unsigned int y) {
  unsigned int minv, maxv;
  if (x == 1 || y == -2U)
    return;
  minv = x < y ? x : y;
  maxv = x > y ? x : y;
  if (minv == 1)
    ++cnt;
  if (maxv == -2U)
    ++cnt;
}

int main() {
  foo(-2U, 1);
  if (cnt != 2)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR tree-optimization/49039
// CHECK-NEXT: static mut cnt: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u32, mut y: u32) {
// CHECK-NEXT:     let mut minv: u32 = 0 as u32;
// CHECK-NEXT:     let mut maxv: u32 = 0 as u32;
// CHECK-NEXT:     if x == ((1 as i32) as u32) || y == (2 as u32).wrapping_neg() {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     minv = if x < y { x } else { y };
// CHECK-NEXT:     maxv = if x > y { x } else { y };
// CHECK-NEXT:     if minv == ((1 as i32) as u32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { cnt };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             cnt = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if maxv == (2 as u32).wrapping_neg() {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { cnt };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             cnt = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo((2 as u32).wrapping_neg(), (1 as i32) as u32);
// CHECK-NEXT:     if (unsafe { cnt }) != (2 as i32) {
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
