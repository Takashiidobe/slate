/* PR rtl-optimization/93402 */

struct S {
  unsigned int       a;
  unsigned long long b;
};

__attribute__((noipa)) struct S foo(unsigned long long x) {
  struct S ret;
  ret.a = 0;
  ret.b = x * 11111111111ULL + 111111111111ULL;
  return ret;
}

int main() {
  struct S a = foo(1);
  if (a.a != 0 || a.b != 122222222222ULL)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/93402
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: u32,
// CHECK-NEXT:     b: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u64) -> S {
// CHECK-NEXT:     let mut ret: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ret.a = (0 as i32) as u32;
// CHECK-NEXT:     ret.b = x
// CHECK-NEXT:         .wrapping_mul(11111111111 as u64)
// CHECK-NEXT:         .wrapping_add(111111111111 as u64);
// CHECK-NEXT:     return ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: S = foo(((1 as i32) as i64) as u64);
// CHECK-NEXT:     if a.a != ((0 as i32) as u32) || a.b != (122222222222 as u64) {
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
