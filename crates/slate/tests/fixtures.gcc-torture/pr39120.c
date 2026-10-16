struct X {
  int *p;
} x;

struct X __attribute__((noinline)) foo(int *p) {
  struct X x;
  x.p = p;
  return x;
}

void __attribute((noinline)) bar() { *x.p = 1; }

extern void abort(void);
int         main() {
  int i = 0;
  x     = foo(&i);
  bar();
  if (i != 1)
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
// CHECK-NEXT:     p: *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: X = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut p: *mut i32) -> X {
// CHECK-NEXT:     let mut [[X_0:x_[0-9]+]]: X = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[X_0]].p = p;
// CHECK-NEXT:     return [[X_0]];
// CHECK-NEXT: }
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.p } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = foo(std::ptr::addr_of_mut!(i));
// CHECK-NEXT:     }
// CHECK-NEXT:     bar();
// CHECK-NEXT:     if i != (1 as i32) {
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
