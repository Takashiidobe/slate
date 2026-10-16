/* PR tree-optimization/119030 */

static inline unsigned foo(long long x) { return x & 0x8000; }

static inline long long bar(long long x) {
  if (foo(x))
    return -1000L;
  else
    return x >> 16;
}

long long x = -0x20000LL;

int main() {
  if (bar(x) >= 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/119030
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut [[X_0:x_[0-9]+]]: i64) -> u32 {
// CHECK-NEXT:     (([[X_0]] & ((32768 as i32) as i64)) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut [[X_1:x_[0-9]+]]: i64) -> i64 {
// CHECK-NEXT:     if foo([[X_1]]) != (0 as u32) {
// CHECK-NEXT:         return -(1000 as i64);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return [[X_1]] >> (16 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i64 = -(131072 as i64);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if bar(unsafe { x }) >= ((0 as i32) as i64) {
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
