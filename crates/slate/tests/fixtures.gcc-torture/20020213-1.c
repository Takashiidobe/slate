/* PR c/5681
   This testcase failed on IA-32 at -O0, because safe_from_p
   incorrectly assumed it is safe to first write into a.a2 b-1
   and then read the original value from it.  */

void abort(void);
int  bar(float);

struct A {
  float a1;
  int   a2;
} a;

int b;

void foo(void) {
  a.a2 = bar(a.a1);
  a.a2 = a.a2 < b - 1 ? a.a2 : b - 1;
  if (a.a2 >= b - 1)
    abort();
}

int bar(float x) { return 2241; }

int main() {
  a.a1 = 1.0f;
  b    = 3384;
  foo();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/5681
// CHECK-NEXT: //! This testcase failed on IA-32 at -O0, because safe_from_p
// CHECK-NEXT: //! incorrectly assumed it is safe to first write into a.a2 b-1
// CHECK-NEXT: //! and then read the original value from it.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     a1: f32,
// CHECK-NEXT:     a2: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: A = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.a2 = bar(unsafe { a.a1 });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.a2 = if (unsafe { a.a2 }) < (unsafe { b }) - (1 as i32) {
// CHECK-NEXT:             unsafe { a.a2 }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (unsafe { b }) - (1 as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a.a2 }) >= (unsafe { b }) - (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: f32) -> i32 {
// CHECK-NEXT:     2241 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a.a1 = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = 3384 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo();
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
