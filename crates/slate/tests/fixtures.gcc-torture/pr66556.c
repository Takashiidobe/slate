/* { dg-do run } */
/* { dg-require-effective-target int32plus } */

extern void abort(void);

struct {
  unsigned f2;
  unsigned f3 : 15;
  unsigned f5 : 3;
  short    f6;
} b = {0x7f8000, 6, 5, 0}, g = {8, 0, 5, 0};

short         d, l;
int           a, c, h = 8;
volatile char e[237] = {4};
short        *f      = &d;
short         i[5]   = {3};
char          j;
int          *k = &c;

int fn1(unsigned p1) { return -p1; }

void fn2(char p1) {
  a = p1;
  e[0];
}

short fn3() {
  *k = 4;
  return *f;
}

int main() {

  unsigned m;
  short   *n = &i[4];

  m  = fn1((h && j) <= b.f5);
  l  = m > g.f3;
  *n = 3;
  fn2(b.f2 >> 15);
  if ((a & 0xff) != 0xff)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     f2: u32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT:     f6: i16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub f3: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub f5: u32,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: [[SLATERECORD_0]] = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].f2 = (8355840 as i32) as u32;
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f3((6 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f5((5 as i32) as u32);
// CHECK-NEXT:     [[T_0]].f6 = (0 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: [[SLATERECORD_0]] = {
// CHECK-NEXT:     let mut [[T_0]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].f2 = (8 as i32) as u32;
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f3((0 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_f5((5 as i32) as u32);
// CHECK-NEXT:     [[T_0]].f6 = (0 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut l: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: i32 = 8 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: __SlateAlign16<[i8; 237]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0]]: [i8; 237] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (4 as i32) as i8;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: *mut i16 = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: [i16; 5] = {
// CHECK-NEXT:     let mut [[T_0]]: [i16; 5] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (3 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: *mut i32 = unsafe { std::ptr::addr_of_mut!(c) };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1(mut p1: u32) -> i32 {
// CHECK-NEXT:     p1.wrapping_neg() as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2(mut p1: i8) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = p1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(e.0) as *mut i8 }.offset((0 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3() -> i16 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { k } = 4 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *unsafe { f } }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut m: u32 = fn1(
// CHECK-NEXT:         ((((unsafe { h }) != (0 as i32) && (unsafe { j }) != (0 as i8)) as i32)
// CHECK-NEXT:             <= ((unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_f5() }) as i32))
// CHECK-NEXT:             as u32,
// CHECK-NEXT:     ) as u32;
// CHECK-NEXT:     let mut n: *mut i16 =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(i) as *mut i16 }.offset((4 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         l = (m > (((unsafe { (*std::ptr::addr_of!(g.__slate_bits_0)).__get_f3() }) as i32) as u32))
// CHECK-NEXT:             as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *n = (3 as i32) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2((((unsafe { b.f2 }) >> (15 as i32)) as u8) as i8);
// CHECK-NEXT:     if (unsafe { a }) & (255 as i32) != (255 as i32) {
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
