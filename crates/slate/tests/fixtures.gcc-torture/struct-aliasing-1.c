struct S {
  float f;
};
int __attribute__((noinline)) foo(int *r, struct S *p) {
  int *q = (int *)&p->f;
  int  i = *q;
  *r     = 0;
  return i + *q;
}
extern void abort(void);
int         main() {
  int i = 1;
  if (foo(&i, (struct S *)&i) != 1)
    abort();
  return (0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     f: f32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut r: &mut i32, mut p: &mut S) -> i32 {
// CHECK-NEXT:     let mut q: *mut i32 = (unsafe { std::ptr::addr_of_mut!((* (p as * mut S)).f) })
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut i: i32 = unsafe { *q };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(r as *mut i32) = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     i + unsafe { *q }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     if foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(i)) },
// CHECK-NEXT:         unsafe { &mut (*(std::ptr::addr_of_mut!(i) as *mut S)) },
// CHECK-NEXT:     ) != (1 as i32)
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
