unsigned int foo(unsigned int a, unsigned int b) {
  unsigned i;
  a = a & 1;
  for (i = 0; i < b; ++i)
    a = a << 1 | a >> (sizeof(unsigned int) * 8 - 1);
  return a;
}
extern void abort(void);
int         main() {
  if (foo(1, sizeof(unsigned int) * 8 + 1) != 2)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: u32, mut b: u32) -> u32 {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     a &= (1 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < b {
// CHECK-NEXT:         a = a << (1 as i32)
// CHECK-NEXT:             | a
// CHECK-NEXT:                 >> (4 as u64)
// CHECK-NEXT:                     .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                     .wrapping_sub(((1 as i32) as i64) as u64);
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         (1 as i32) as u32,
// CHECK-NEXT:         (4 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_add(((1 as i32) as i64) as u64) as u32,
// CHECK-NEXT:     ) != ((2 as i32) as u32)
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
