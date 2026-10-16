void abort(void);
void exit(int);

int main(void) {
  unsigned long totalsize = 80;
  unsigned long msize     = 64;

  if (sizeof(long) != 4)
    exit(0);

  if (totalsize > (2147483647L * 2UL + 1) ||
      (msize != 0 && ((msize - 1) > (2147483647L * 2UL + 1))))
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut totalsize: u64 = ((80 as i32) as i64) as u64;
// CHECK-NEXT:     let mut msize: u64 = ((64 as i32) as i64) as u64;
// CHECK-NEXT:     if (8 as u64) != (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if totalsize
// CHECK-NEXT:         > ((2147483647 as i64) as u64)
// CHECK-NEXT:             .wrapping_mul(2 as u64)
// CHECK-NEXT:             .wrapping_add(((1 as i32) as i64) as u64)
// CHECK-NEXT:         || msize != (((0 as i32) as i64) as u64)
// CHECK-NEXT:             && msize.wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:                 > ((2147483647 as i64) as u64)
// CHECK-NEXT:                     .wrapping_mul(2 as u64)
// CHECK-NEXT:                     .wrapping_add(((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
