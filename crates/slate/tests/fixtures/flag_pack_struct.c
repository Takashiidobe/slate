/* { dg-options "-fpack-struct=2" } */
int printf(const char *, ...);

struct mixed {
  char c;
  int i;
  short h;
  double d;
};

struct nested {
  char tag;
  struct mixed inner;
};

int main(void) {
  struct mixed m = {1, 2, 3, 4.5};
  struct nested n = {7, {8, 9, 10, 11.5}};
  printf("%zu %zu %zu %zu\n", sizeof(struct mixed), __builtin_offsetof(struct mixed, i),
         __builtin_offsetof(struct mixed, d), _Alignof(struct mixed));
  printf("%zu %zu\n", sizeof(struct nested), __builtin_offsetof(struct nested, inner));
  printf("%d %d %d %g %d %g\n", m.c, m.i, m.h, m.d, n.tag + n.inner.c, n.inner.d);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fpack-struct=2" }
// CHECK-NEXT: #[repr(C, packed(2))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mixed {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     h: i16,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct nested {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     inner: mixed,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let m: mixed = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: mixed = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].c = 1i8;
// CHECK-NEXT:         [[T_0]].i = 2i32;
// CHECK-NEXT:         [[T_0]].h = 3i16;
// CHECK-NEXT:         [[T_0]].d = 4.5f64;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let n: nested = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: nested = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].tag = 7i8;
// CHECK-NEXT:         [[T_1]].inner = {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: mixed = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].c = 8i8;
// CHECK-NEXT:             [[T_2]].i = 9i32;
// CHECK-NEXT:             [[T_2]].h = 10i16;
// CHECK-NEXT:             [[T_2]].d = 11.5f64;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 16u64, 2u64, 8u64, 2u64) };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 18u64, 2u64) };
// I686-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 16u32, 2u32, 8u32, 2u32) };
// I686-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 18u32, 2u32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %g %d %g\n".as_ptr(),
// CHECK-NEXT:             m.c as i32,
// CHECK-NEXT:             m.i,
// CHECK-NEXT:             m.h as i32,
// CHECK-NEXT:             m.d,
// CHECK-NEXT:             (n.tag as i32) + (n.inner.c as i32),
// CHECK-NEXT:             n.inner.d,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
