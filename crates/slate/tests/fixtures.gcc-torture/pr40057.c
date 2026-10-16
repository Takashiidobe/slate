/* PR middle-end/40057 */

extern void abort(void);

__attribute__((noinline)) int foo(unsigned long long x) {
  unsigned long long y = (x >> 31ULL) & 1ULL;
  if (y == 0ULL)
    return 0;
  return -1;
}

__attribute__((noinline)) int bar(long long x) {
  long long y = (x >> 31LL) & 1LL;
  if (y == 0LL)
    return 0;
  return -1;
}

int main(void) {
  if (sizeof(long long) != 8)
    return 0;
  if (foo(0x1682a9aaaULL))
    abort();
  if (!foo(0x1882a9aaaULL))
    abort();
  if (bar(0x1682a9aaaLL))
    abort();
  if (!bar(0x1882a9aaaLL))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/40057
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u64) -> i32 {
// CHECK-NEXT:     let mut y: u64 = x >> (31 as u64) & (1 as u64);
// CHECK-NEXT:     if y == (0 as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     -(1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i64) -> i32 {
// CHECK-NEXT:     let mut y: i64 = x >> (31 as i64) & (1 as i64);
// CHECK-NEXT:     if y == (0 as i64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     -(1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as u64) != (((8 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(6042589866 as u64) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(foo(6579460778 as u64) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if bar(6042589866 as i64) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(bar(6579460778 as i64) != (0 as i32)) {
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
