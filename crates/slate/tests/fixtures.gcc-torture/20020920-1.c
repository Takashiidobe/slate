extern void abort(void);
extern void exit(int);

struct B {
  int x;
  int y;
};

struct A {
  int      z;
  struct B b;
};

struct A f() {
  struct B b = {0, 1};
  struct A a = {2, b};
  return a;
}

int main(void) {
  struct A a = f();
  if (a.z != 2 || a.b.x != 0 || a.b.y != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     z: i32,
// CHECK-NEXT:     b: B,
// CHECK-NEXT: }
// CHECK-NEXT: fn f() -> A {
// CHECK-NEXT:     let mut b: B = B { x: 0 as i32, y: 1 as i32 };
// CHECK-NEXT:     let mut a: A = A { z: 2 as i32, b: b };
// CHECK-NEXT:     return a;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: A = f();
// CHECK-NEXT:     if a.z != (2 as i32) || a.b.x != (0 as i32) || a.b.y != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
