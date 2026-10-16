extern void abort(void);

#define N (1 << (sizeof(int) * __CHAR_BIT__ - 2))

int f(int n) {
  if (-N <= n && n <= N - 1)
    return 1;
  return 0;
}

int main() {
  if (f(N))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut n: i32) -> i32 {
// CHECK-NEXT:     if -((1 as i32)
// CHECK-NEXT:         << (4 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((2 as i32) as i64) as u64)) <= n
// CHECK-NEXT:         && n
// CHECK-NEXT:             <= ((1 as i32)
// CHECK-NEXT:                 << (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((2 as i32) as i64) as u64)) - (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(
// CHECK-NEXT:         (1 as i32)
// CHECK-NEXT:             << (4 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((2 as i32) as i64) as u64),
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
