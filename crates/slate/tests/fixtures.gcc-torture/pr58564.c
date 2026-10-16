/* PR middle-end/58564 */

extern void abort(void);
int         a, b;
short      *c, **d = &c;

int main() {
  b = (0, 0 > ((&c == d) & (1 && (a ^ 1)))) | 0U;
  if (b != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/58564
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: *mut *mut i16 = unsafe { std::ptr::addr_of_mut!(c) };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (((((0 as i32)
// CHECK-NEXT:             > (((unsafe { std::ptr::addr_of_mut!(c) }) == unsafe { d }) as i32)
// CHECK-NEXT:                 & (((1 as i32) != (0 as i32) && (unsafe { a }) ^ (1 as i32) != (0 as i32)) as i32))
// CHECK-NEXT:             as i32) as u32)
// CHECK-NEXT:             | (0 as u32)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b }) != (0 as i32) {
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
