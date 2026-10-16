/* PR target/94567 */

volatile int   a = 1, b;
short          c, d = 4, f = 2, g;
unsigned short e = 53736;

int foo(int i, int j) { return i && j ? 0 : i + j; }

int main() {
  for (; a; a = 0) {
    unsigned short k = e;
    g                = k >> 3;
    if (foo(g < (f || c), b))
      d = 0;
  }
  if (d != 4)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/94567
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i16 = (4 as i32) as i16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i16 = (2 as i32) as i16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: u16 = ((53736 as i32) as i16) as u16;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: i32, mut j: i32) -> i32 {
// CHECK-NEXT:     if i != (0 as i32) && j != (0 as i32) {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         i + j
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut k: u16 = unsafe { e };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g = (((k as u32) as i32) >> (3 as i32)) as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         if foo(
// CHECK-NEXT:             (((unsafe { g }) as i32)
// CHECK-NEXT:                 < (((unsafe { f }) != (0 as i16) || (unsafe { c }) != (0 as i16)) as i32))
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) },
// CHECK-NEXT:         ) != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 d = (0 as i32) as i16;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), 0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { d }) as i32) != (4 as i32) {
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
