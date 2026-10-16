/* PR tree-optimization/58984 */

struct S {
  int f0 : 8;
  int    : 6;
  int f1 : 5;
};
struct T {
  char f0;
  int    : 6;
  int f1 : 5;
};

int a, *c = &a, e, n, b, m;

static int foo(struct S p) {
  const unsigned short *f[36];
  for (; e < 2; e++) {
    const unsigned short **i  = &f[0];
    *c                       ^= 1;
    if (p.f1) {
      *i = 0;
      return b;
    }
  }
  return 0;
}

static int bar(struct T p) {
  const unsigned short *f[36];
  for (; e < 2; e++) {
    const unsigned short **i  = &f[0];
    *c                       ^= 1;
    if (p.f1) {
      *i = 0;
      return b;
    }
  }
  return 0;
}

int main() {
  struct S o = {1, 1};
  foo(o);
  m = n || o.f0;
  if (a != 1)
    __builtin_abort();
  e          = 0;
  struct T p = {1, 1};
  bar(p);
  m |= n || p.f0;
  if (a != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/58984
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct T {
// CHECK-NEXT:     f0: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub f0: i32,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub f1: i32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield([u8; 2], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u8,
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub f1: i32,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut i32 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut n: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut m: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: S) -> i32 {
// CHECK-NEXT:     let mut f: __SlateAlign16<[*const u16; 36]> = __SlateAlign16([0 as *const u16; 36]);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { e }) < (2 as i32) {
// CHECK-NEXT:         let mut i: *mut *const u16 =
// CHECK-NEXT:             unsafe { (f.0.as_mut_ptr() as *mut *const u16).offset((0 as i32) as isize) };
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = unsafe { *[[V_0]] };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] ^ (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if p.__slate_bits_0.__get_f1() != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *i = std::ptr::null::<u16>();
// CHECK-NEXT:             }
// CHECK-NEXT:             return unsafe { b };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut p: T) -> i32 {
// CHECK-NEXT:     let mut f: __SlateAlign16<[*const u16; 36]> = __SlateAlign16([0 as *const u16; 36]);
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { e }) < (2 as i32) {
// CHECK-NEXT:         let mut i: *mut *const u16 =
// CHECK-NEXT:             unsafe { (f.0.as_mut_ptr() as *mut *const u16).offset((0 as i32) as isize) };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = unsafe { *[[V_5]] };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] ^ (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_5]] = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if p.__slate_bits_0.__get_f1() != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *i = std::ptr::null::<u16>();
// CHECK-NEXT:             }
// CHECK-NEXT:             return unsafe { b };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_9]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut o: S = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_f0(1 as i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_f1(1 as i32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(o);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         m = ((unsafe { n }) != (0 as i32) || o.__slate_bits_0.__get_f0() != (0 as i32)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a }) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut p: T = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: T = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].f0 = (1 as i32) as i8;
// CHECK-NEXT:         [[T_1]].__slate_bits_0.__set_f1(1 as i32);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     bar(p);
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: i32 = unsafe { m };
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] | (((unsafe { n }) != (0 as i32) || p.f0 != (0 as i8)) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         m = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a }) != (0 as i32) {
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
