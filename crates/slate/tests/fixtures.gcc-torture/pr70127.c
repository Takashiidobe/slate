/* PR tree-optimization/70127 */

struct S {
  int        f;
  signed int g : 2;
} a[1], c = {5, 1}, d;
short b;

__attribute__((noinline, noclone)) void foo(int x) {
  if (x != 1)
    __builtin_abort();
}

int main() {
  while (b++ <= 0) {
    struct S e = {1, 1};
    d = e = a[0] = c;
  }
  foo(a[0].g);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/70127
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     f: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub g: i32,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: [S; 1] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: S = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].f = 5 as i32;
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_g(1 as i32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     if x != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i16 = unsafe { b };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i16 = (([[V_0]] as i32) + (1 as i32)) as i16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !(([[V_0]] as i32) <= (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut e: S = {
// CHECK-NEXT:             let mut [[T_0]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].f = 1 as i32;
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_g(1 as i32);
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: S = unsafe { c };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut S }.offset((0 as i32) as isize)
// CHECK-NEXT:             } = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: S = [[V_2]];
// CHECK-NEXT:         e = [[V_3]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(unsafe {
// CHECK-NEXT:         (*unsafe { unsafe { std::ptr::addr_of_mut!(a) as *mut S }.offset((0 as i32) as isize) })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_g()
// CHECK-NEXT:     });
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
