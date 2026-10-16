/* PR tree-optimization/81555 */

unsigned int  a = 1, d = 0xfaeU, e = 0xe376U;
_Bool         b = 0, f = 1;
unsigned char g = 1;

void foo(void) {
  _Bool c = a != b;
  if (c)
    f = 0;
  if (e & c & (unsigned char)d & c)
    g = 0;
}

int main() {
  foo();
  if (f || g != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/81555
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u32 = (1 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: u32 = 4014 as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: u32 = 58230 as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: bool = (0 as i32) != (0 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: bool = (1 as i32) != (0 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: u8 = ((1 as i32) as i8) as u8;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut c: bool = (unsafe { a }) != (((unsafe { b }) as i32) as u32);
// CHECK-NEXT:     if c {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             f = (0 as i32) != (0 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { e })
// CHECK-NEXT:         & ((c as i32) as u32)
// CHECK-NEXT:         & (((((unsafe { d }) as u8) as u32) as i32) as u32)
// CHECK-NEXT:         & ((c as i32) as u32)
// CHECK-NEXT:         != (0 as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g = ((0 as i32) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { f }) || (((unsafe { g }) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
