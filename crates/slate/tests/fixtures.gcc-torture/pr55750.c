/* PR middle-end/55750 */

extern void abort(void);

struct S {
  int m : 1;
  int n : 7;
} arr[2];

__attribute__((noinline, noclone)) void foo(unsigned i) { arr[i].n++; }

int main() {
  arr[0].m = -1;
  arr[0].n = (1 << 6) - 1;
  arr[1].m = 0;
  arr[1].n = -1;
  foo(0);
  foo(1);
  if (arr[0].m != -1 || arr[0].n != -(1 << 6) || arr[1].m != 0 || arr[1].n != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/55750
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
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
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub m: i32,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub n: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut arr: [S; 2] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut i: u32) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut S =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset(i as isize) };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { (*[[V_0]]).__slate_bits_0.__get_n() };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_0]]).__slate_bits_0.__set_n([[V_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset((0 as i32) as isize) })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_m(-(1 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset((0 as i32) as isize) })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_n(((1 as i32) << (6 as i32)) - (1 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset((1 as i32) as isize) })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_m(0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset((1 as i32) as isize) })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_n(-(1 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     foo((0 as i32) as u32);
// CHECK-NEXT:     foo((1 as i32) as u32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         (*unsafe { unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset((0 as i32) as isize) })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_m()
// CHECK-NEXT:     }) != -(1 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset((0 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_n()
// CHECK-NEXT:         }) != -((1 as i32) << (6 as i32))
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_m()
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(arr) as *mut S }.offset((1 as i32) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_n()
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:     {
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
