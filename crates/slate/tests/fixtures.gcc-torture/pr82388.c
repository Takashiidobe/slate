/* PR tree-optimization/82388 */

struct A {
  int b;
  int c;
  int d;
} e;

struct A foo(void) {
  struct A h[30] = {{0, 0, 0}};
  return h[29];
}

int main() {
  e = foo();
  return e.b;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/82388
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     d: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: A = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> A {
// CHECK-NEXT:     let mut h: __SlateAlign16<[A; 30]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [A; 30] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = A {
// CHECK-NEXT:             b: 0 as i32,
// CHECK-NEXT:             c: 0 as i32,
// CHECK-NEXT:             d: 0 as i32,
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     return unsafe { *unsafe { (h.0.as_mut_ptr() as *mut A).offset((29 as i32) as isize) } };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = foo();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { e.b }
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
