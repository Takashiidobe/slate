struct X {
  int i;
};

int __attribute__((noinline)) foo(struct X *p, int *q, int a, int b) {
  struct X x, y;
  if (a)
    p = &x;
  if (b)
    q = &x.i;
  else
    q = &y.i;
  *q = 1;
  return p->i;
}
extern void abort(void);
int         main() {
  if (foo((void *)0, (void *)0, 1, 1) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct X {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: *mut X, mut q: *mut i32, mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     let mut x: X = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut y: X = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if a != (0 as i32) {
// CHECK-NEXT:         p = std::ptr::addr_of_mut!(x);
// CHECK-NEXT:     }
// CHECK-NEXT:     if b != (0 as i32) {
// CHECK-NEXT:         q = std::ptr::addr_of_mut!(x.i);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         q = std::ptr::addr_of_mut!(y.i);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *q = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { (*p).i }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(std::ptr::null_mut::<X>(), std::ptr::null_mut::<i32>(), 1 as i32, 1 as i32)
// CHECK-NEXT:         != (1 as i32)
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
