unsigned p;

long __attribute__((noinline, noclone)) test(unsigned a) {
  return (long)(p + a) - (long)p;
}

int main() {
  p = (unsigned)-2;
  if (test(0) != 0)
    __builtin_abort();
  if (test(1) != 1)
    __builtin_abort();
  if (test(2) != -(long)(unsigned)-2)
    __builtin_abort();
  p = (unsigned)-1;
  if (test(0) != 0)
    __builtin_abort();
  if (test(1) != -(long)(unsigned)-1)
    __builtin_abort();
  if (test(2) != -(long)(unsigned)-2)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut p: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn test(mut a: u32) -> i64 {
// CHECK-NEXT:     ((unsafe { p }.wrapping_add(a) as u64) as i64) - (((unsafe { p }) as u64) as i64)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p = -(2 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if test((0 as i32) as u32) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test((1 as i32) as u32) != ((1 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test((2 as i32) as u32) != -(((-(2 as i32) as u32) as u64) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p = -(1 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if test((0 as i32) as u32) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test((1 as i32) as u32) != -(((-(1 as i32) as u32) as u64) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test((2 as i32) as u32) != -(((-(2 as i32) as u32) as u64) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
