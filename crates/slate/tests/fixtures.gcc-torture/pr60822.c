/* { dg-require-effective-target int32plus } */
struct X {
  char fill0[800000];
  int  a;
  char fill1[900000];
  int  b;
};

int __attribute__((noinline, noclone)) Avg(struct X *p, int s) {
  return (s * (long long)(p->a + p->b)) >> 17;
}

struct X x;

int main() {
  x.a = 1 << 17;
  x.b = 2 << 17;
  if (Avg(&x, 1) != 3)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-require-effective-target int32plus }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct X {
// CHECK-NEXT:     fill0: [i8; 800000],
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     fill1: [i8; 900000],
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn Avg(mut p: *mut X, mut s: i32) -> i32 {
// CHECK-NEXT:     ((s as i64) * (((unsafe { (*p).a }) + unsafe { (*p).b }) as i64) >> (17 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: X = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.a = (1 as i32) << (17 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.b = (2 as i32) << (17 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if Avg(unsafe { std::ptr::addr_of_mut!(x) }, 1 as i32) != (3 as i32) {
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
