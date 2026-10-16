typedef unsigned int       u32;
typedef unsigned long long u64;
u64                        a;
u32                        b;

u64 foo(u32 d) {
  a -= d ? 0 : ~a;
  return a + b;
}

int main(void) {
  u64 x = foo(2);
  if (x != 0)
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
// CHECK-NEXT: static mut a: u64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut d: u32) -> u64 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = unsafe { a };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]]
// CHECK-NEXT:         .wrapping_sub(
// CHECK-NEXT:             if d != (0 as u32) { ((0 as i32) as i64) as u64 } else { !unsafe { a } },
// CHECK-NEXT:         );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { a }.wrapping_add((unsafe { b }) as u64)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u64 = foo((2 as i32) as u32);
// CHECK-NEXT:     if x != (((0 as i32) as i64) as u64) {
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
