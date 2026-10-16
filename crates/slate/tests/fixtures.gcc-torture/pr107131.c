/* PR target/107131 */

__attribute__((noipa)) unsigned long long foo(unsigned char o) {
  unsigned long long t1 = -(long long)(o == 0);
  unsigned long long t2 = -(long long)(t1 > 10439075533421201520ULL);
  unsigned long long t3 = -(long long)(t1 <= t2);
  return t3;
}

int main() {
  if (foo(0) != -1ULL)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/107131
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut o: u8) -> u64 {
// CHECK-NEXT:     let mut t1: u64 = -((((o as u32) as i32) == (0 as i32)) as i64) as u64;
// CHECK-NEXT:     let mut t2: u64 = -((t1 > (10439075533421201520 as u64)) as i64) as u64;
// CHECK-NEXT:     let mut t3: u64 = -((t1 <= t2) as i64) as u64;
// CHECK-NEXT:     t3
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(((0 as i32) as i8) as u8) != (1 as u64).wrapping_neg() {
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
