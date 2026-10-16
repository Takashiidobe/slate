/* PR rtl-optimization/104839 */

__attribute__((noipa)) short foo(void) { return -1; }

__attribute__((noipa)) int bar(void) {
  short i = foo();
  if (i == -2)
    return 2;
  long          k = i;
  int           j = -1;
  volatile long s = 300;
  if (k < 0) {
    k += s;
    if (k < 0)
      j = 0;
  } else if (k >= s)
    j = 0;
  if (j != -1)
    return 1;
  return 0;
}

int main() {
  if (bar() != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/104839
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i16 {
// CHECK-NEXT:     -(1 as i32) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     let mut i: i16 = foo();
// CHECK-NEXT:     if (i as i32) == -(2 as i32) {
// CHECK-NEXT:         return 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut k: i64 = i as i64;
// CHECK-NEXT:     let mut j: i32 = -(1 as i32);
// CHECK-NEXT:     let mut s: i64 = (300 as i32) as i64;
// CHECK-NEXT:     if k < ((0 as i32) as i64) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i64 = k;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i64 = [[V_0]] + unsafe { std::ptr::read_volatile(std::ptr::addr_of!(s)) };
// CHECK-NEXT:         k = [[V_1]];
// CHECK-NEXT:         if k < ((0 as i32) as i64) {
// CHECK-NEXT:             j = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if k >= unsafe { std::ptr::read_volatile(std::ptr::addr_of!(s)) } {
// CHECK-NEXT:             j = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if j != -(1 as i32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if bar() != (0 as i32) {
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
