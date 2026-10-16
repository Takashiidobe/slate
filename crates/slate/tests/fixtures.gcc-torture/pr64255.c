/* PR rtl-optimization/64255 */

__attribute__((noinline, noclone)) void bar(long i, unsigned long j) {
  if (i != 1 || j != 1)
    __builtin_abort();
}

__attribute__((noinline, noclone)) void foo(long i) {
  unsigned long j;

  if (!i)
    return;
  j = i >= 0 ? (unsigned long)i : -(unsigned long)i;
  if ((i >= 0 ? (unsigned long)i : -(unsigned long)i) != j)
    __builtin_abort();
  bar(i, j);
}

int main() {
  foo(1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/64255
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut i: i64, mut j: u64) {
// CHECK-NEXT:     if i != ((1 as i32) as i64) || j != (((1 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: i64) {
// CHECK-NEXT:     let mut j: u64 = 0 as u64;
// CHECK-NEXT:     if !(i != (0 as i64)) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     j = if i >= ((0 as i32) as i64) {
// CHECK-NEXT:         i as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (i as u64).wrapping_neg()
// CHECK-NEXT:     };
// CHECK-NEXT:     if (if i >= ((0 as i32) as i64) {
// CHECK-NEXT:         i as u64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (i as u64).wrapping_neg()
// CHECK-NEXT:     }) != j
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(i, j);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo((1 as i32) as i64);
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
