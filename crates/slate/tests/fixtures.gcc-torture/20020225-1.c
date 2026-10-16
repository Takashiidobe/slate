/* This testcase failed at -O2 on powerpc64 due to andsi3 writing
   nonzero bits to the high 32 bits of a 64 bit register.  */

extern void abort(void);
extern void exit(int);

unsigned long foo(unsigned long base, unsigned int val) {
  return base + (val & 0x80000001);
}

int main(void) {
  if (foo(0L, 0x0ffffff0) != 0L)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This testcase failed at -O2 on powerpc64 due to andsi3 writing
// CHECK-NEXT: //! nonzero bits to the high 32 bits of a 64 bit register.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut base: u64, mut val: u32) -> u64 {
// CHECK-NEXT:     base.wrapping_add((val & (2147483649 as u32)) as u64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo((0 as i64) as u64, (268435440 as i32) as u32) != ((0 as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
