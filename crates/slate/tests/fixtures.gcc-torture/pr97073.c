/* PR middle-end/97073 */
/* { dg-additional-options "-mno-stv" { target i?86-*-* x86_64-*-* } } */

typedef unsigned long long L;
union U {
  L i;
  struct T {
    unsigned k;
    L        l;
  } j;
} u;

__attribute__((noinline, noclone)) void foo(L x) { u.j.l = u.i & x; }

int main() {
  u.i = 5;
  foo(-1ULL);
  if (u.j.l != 5)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/97073
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-additional-options "-mno-stv" { target i?86-*-* x86_64-*-* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union U {
// CHECK-NEXT:     i: u64,
// CHECK-NEXT:     j: T,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     k: u32,
// CHECK-NEXT:     l: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: U = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.j.l = (unsafe { u.i }) & x;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.i = ((5 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo((1 as u64).wrapping_neg());
// CHECK-NEXT:     if (unsafe { u.j.l }) != (((5 as i32) as i64) as u64) {
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
