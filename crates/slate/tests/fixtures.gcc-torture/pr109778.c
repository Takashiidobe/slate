/* PR tree-optimization/109778 */

int a, b, c, d, *e = &c;

static inline unsigned foo(unsigned char x) {
  x = 1 | x << 1;
  x = x >> 4 | x << 4;
  return x;
}

static inline void bar(unsigned x) { *e = 8 > foo(x + 86) - 86; }

int main() {
  d = a && b;
  bar(d + 4);
  if (c != 1)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/109778
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: *mut i32 = unsafe { std::ptr::addr_of_mut!(c) };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u8) -> u32 {
// CHECK-NEXT:     x = (((1 as i32) | ((x as u32) as i32) << (1 as i32)) as i8) as u8;
// CHECK-NEXT:     x = ((((x as u32) as i32) >> (4 as i32) | ((x as u32) as i32) << (4 as i32)) as i8) as u8;
// CHECK-NEXT:     x as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: u32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { e } = (((8 as i32) as u32)
// CHECK-NEXT:             > foo(x.wrapping_add((86 as i32) as u32) as u8).wrapping_sub((86 as i32) as u32))
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = ((unsafe { a }) != (0 as i32) && (unsafe { b }) != (0 as i32)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(((unsafe { d }) + (4 as i32)) as u32);
// CHECK-NEXT:     if (unsafe { c }) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
