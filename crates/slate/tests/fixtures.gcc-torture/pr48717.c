/* PR tree-optimization/48717 */

extern void abort(void);

int v = 1, w;

unsigned short foo(unsigned short x, unsigned short y) { return x + y; }

void bar(void) { v = foo(~w, w); }

int main() {
  bar();
  if (v != (unsigned short)-1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/48717
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut w: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u16, mut y: u16) -> u16 {
// CHECK-NEXT:     ((((x as u32) as i32) + ((y as u32) as i32)) as i16) as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = (foo(
// CHECK-NEXT:             (!unsafe { w } as i16) as u16,
// CHECK-NEXT:             ((unsafe { w }) as i16) as u16,
// CHECK-NEXT:         ) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bar();
// CHECK-NEXT:     if (unsafe { v }) != ((((-(1 as i32) as i16) as u16) as u32) as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
