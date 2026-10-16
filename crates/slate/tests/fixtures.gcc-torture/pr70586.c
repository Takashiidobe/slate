/* PR tree-optimization/70586 */

int   a, e, f;
short b, c, d;

int foo(int x, int y) { return (y == 0 || (x && y == 1)) ? x : x % y; }

static short bar(void) {
  int i = foo(c, f);
  f     = foo(d, 2);
  int g = foo(b, c);
  int h = foo(g > 0, c);
  c     = (3 >= h ^ 7) <= foo(i, c);
  if (foo(e, 1))
    return a;
  return 0;
}

int main() {
  bar();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/70586
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32) -> i32 {
// CHECK-NEXT:     if y == (0 as i32) || x != (0 as i32) && y == (1 as i32) {
// CHECK-NEXT:         x
// CHECK-NEXT:     } else {
// CHECK-NEXT:         x % y
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i16 {
// CHECK-NEXT:     let mut i: i32 = foo((unsafe { c }) as i32, unsafe { f });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = foo((unsafe { d }) as i32, 2 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut g: i32 = foo((unsafe { b }) as i32, (unsafe { c }) as i32);
// CHECK-NEXT:     let mut h: i32 = foo((g > (0 as i32)) as i32, (unsafe { c }) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = ((((3 as i32) >= h) as i32) ^ (7 as i32) <= foo(i, (unsafe { c }) as i32)) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(unsafe { e }, 1 as i32) != (0 as i32) {
// CHECK-NEXT:         return (unsafe { a }) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     (0 as i32) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bar();
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
