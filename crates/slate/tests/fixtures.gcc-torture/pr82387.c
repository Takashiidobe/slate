/* PR tree-optimization/82387 */

struct A {
  int b;
};
int f = 1;

struct A foo(void) {
  struct A h[] = {
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
      {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1}, {1},
  };
  return h[24];
}

int main() {
  struct A i = foo(), j = i;
  j.b && (f = 0);
  return f;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/82387
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> A {
// CHECK-NEXT:     let mut h: __SlateAlign16<[A; 70]> = __SlateAlign16([
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:         A { b: 1 as i32 },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     return unsafe { *unsafe { (h.0.as_mut_ptr() as *mut A).offset((24 as i32) as isize) } };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: A = foo();
// CHECK-NEXT:     let mut j: A = i;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if j.b != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             f = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_0]] = (0 as i32) != (0 as i32);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { f }
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
