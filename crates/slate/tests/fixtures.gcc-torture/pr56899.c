/* PR tree-optimization/56899 */

#if __SIZEOF_INT__ == 4 && __CHAR_BIT__ == 8
__attribute__((noinline, noclone)) void f1(int v) {
  int x = -214748365 * (v - 1);
  if (x != -1932735285)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void f2(int v) {
  int x = 214748365 * (v + 1);
  if (x != -1932735285)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void f3(unsigned int v) {
  unsigned int x = -214748365U * (v - 1);
  if (x != -1932735285U)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void f4(unsigned int v) {
  unsigned int x = 214748365U * (v + 1);
  if (x != -1932735285U)
    __builtin_abort();
}
#endif

int main() {
#if __SIZEOF_INT__ == 4 && __CHAR_BIT__ == 8
  f1(10);
  f2(-10);
  f3(10);
  f4(-10U);
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/56899
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut v: i32) {
// CHECK-NEXT:     let mut x: i32 = -(214748365 as i32) * (v - (1 as i32));
// CHECK-NEXT:     if x != -(1932735285 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut v: i32) {
// CHECK-NEXT:     let mut x: i32 = (214748365 as i32) * (v + (1 as i32));
// CHECK-NEXT:     if x != -(1932735285 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut v: u32) {
// CHECK-NEXT:     let mut x: u32 = (214748365 as u32)
// CHECK-NEXT:         .wrapping_neg()
// CHECK-NEXT:         .wrapping_mul(v.wrapping_sub((1 as i32) as u32));
// CHECK-NEXT:     if x != (1932735285 as u32).wrapping_neg() {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f4(mut v: u32) {
// CHECK-NEXT:     let mut x: u32 = (214748365 as u32).wrapping_mul(v.wrapping_add((1 as i32) as u32));
// CHECK-NEXT:     if x != (1932735285 as u32).wrapping_neg() {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f1(10 as i32);
// CHECK-NEXT:     f2(-(10 as i32));
// CHECK-NEXT:     f3((10 as i32) as u32);
// CHECK-NEXT:     f4((10 as u32).wrapping_neg());
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
