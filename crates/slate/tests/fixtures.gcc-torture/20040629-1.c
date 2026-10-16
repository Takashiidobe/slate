/* { dg-require-effective-target int32plus } */

/* Test arithmetics on bitfields.  */
#ifndef T

extern void abort(void);
extern void exit(int);

#ifndef FIELDS1
#define FIELDS1
#endif
#ifndef FIELDS2
#define FIELDS2
#endif

struct {
  FIELDS1 unsigned int i : 6, j : 11, k : 15;
  FIELDS2
} b;
struct {
  FIELDS1 unsigned int i : 5, j : 1, k : 26;
  FIELDS2
} c;
struct {
  FIELDS1 unsigned int i : 16, j : 8, k : 8;
  FIELDS2
} d;

unsigned int ret1(void) { return b.i; }
unsigned int ret2(void) { return b.j; }
unsigned int ret3(void) { return b.k; }
unsigned int ret4(void) { return c.i; }
unsigned int ret5(void) { return c.j; }
unsigned int ret6(void) { return c.k; }
unsigned int ret7(void) { return d.i; }
unsigned int ret8(void) { return d.j; }
unsigned int ret9(void) { return d.k; }

#define T(n, pre, post, op)                                                    \
  void fn1_##n(unsigned int x) { pre b.i post; }                               \
  void fn2_##n(unsigned int x) { pre b.j post; }                               \
  void fn3_##n(unsigned int x) { pre b.k post; }                               \
  void fn4_##n(unsigned int x) { pre c.i post; }                               \
  void fn5_##n(unsigned int x) { pre c.j post; }                               \
  void fn6_##n(unsigned int x) { pre c.k post; }                               \
  void fn7_##n(unsigned int x) { pre d.i post; }                               \
  void fn8_##n(unsigned int x) { pre d.j post; }                               \
  void fn9_##n(unsigned int x) { pre d.k post; }

#include "20040629-1.c"
#undef T

#define FAIL(n, i) abort()

int main(void) {
#define T(n, pre, post, op)                                                    \
  b.i = 51;                                                                    \
  b.j = 636;                                                                   \
  b.k = 31278;                                                                 \
  c.i = 21;                                                                    \
  c.j = 1;                                                                     \
  c.k = 33554432;                                                              \
  d.i = 26812;                                                                 \
  d.j = 156;                                                                   \
  d.k = 187;                                                                   \
  fn1_##n(3);                                                                  \
  if (ret1() != (op(51, 3) & ((1 << 6) - 1)))                                  \
    FAIL(n, 1);                                                                \
  b.i = 51;                                                                    \
  fn2_##n(251);                                                                \
  if (ret2() != (op(636, 251) & ((1 << 11) - 1)))                              \
    FAIL(n, 2);                                                                \
  b.j = 636;                                                                   \
  fn3_##n(13279);                                                              \
  if (ret3() != (op(31278, 13279) & ((1 << 15) - 1)))                          \
    FAIL(n, 3);                                                                \
  b.j = 31278;                                                                 \
  fn4_##n(24);                                                                 \
  if (ret4() != (op(21, 24) & ((1 << 5) - 1)))                                 \
    FAIL(n, 4);                                                                \
  c.i = 21;                                                                    \
  fn5_##n(1);                                                                  \
  if (ret5() != (op(1, 1) & ((1 << 1) - 1)))                                   \
    FAIL(n, 5);                                                                \
  c.j = 1;                                                                     \
  fn6_##n(264151);                                                             \
  if (ret6() != (op(33554432, 264151) & ((1 << 26) - 1)))                      \
    FAIL(n, 6);                                                                \
  c.k = 33554432;                                                              \
  fn7_##n(713);                                                                \
  if (ret7() != (op(26812, 713) & ((1 << 16) - 1)))                            \
    FAIL(n, 7);                                                                \
  d.i = 26812;                                                                 \
  fn8_##n(17);                                                                 \
  if (ret8() != (op(156, 17) & ((1 << 8) - 1)))                                \
    FAIL(n, 8);                                                                \
  d.j = 156;                                                                   \
  fn9_##n(199);                                                                \
  if (ret9() != (op(187, 199) & ((1 << 8) - 1)))                               \
    FAIL(n, 9);                                                                \
  d.k = 187;

#include "20040629-1.c"
#undef T
  return 0;
}

#else

#ifndef opadd
#define opadd(x, y)   (x + y)
#define opsub(x, y)   (x - y)
#define opinc(x, y)   (x + 1)
#define opdec(x, y)   (x - 1)
#define opand(x, y)   (x & y)
#define opior(x, y)   (x | y)
#define opxor(x, y)   (x ^ y)
#define opdiv(x, y)   (x / y)
#define oprem(x, y)   (x % y)
#define opadd3(x, y)  (x + 3)
#define opsub7(x, y)  (x - 7)
#define opand21(x, y) (x & 21)
#define opior19(x, y) (x | 19)
#define opxor37(x, y) (x ^ 37)
#define opdiv17(x, y) (x / 17)
#define oprem19(x, y) (x % 19)
#endif

T(1, , += x, opadd)
T(2, ++, , opinc)
T(3, , ++, opinc)
T(4, , -= x, opsub)
T(5, --, , opdec)
T(6, , --, opdec)
T(7, , &= x, opand)
T(8, , |= x, opior)
T(9, , ^= x, opxor)
T(a, , /= x, opdiv)
T(b, , %= x, oprem)
T(c, , += 3, opadd3)
T(d, , -= 7, opsub7)
T(e, , &= 21, opand21)
T(f, , |= 19, opior19)
T(g, , ^= 37, opxor37)
T(h, , /= 17, opdiv17)
T(i, , %= 19, oprem19)

#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Test arithmetics on bitfields.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
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
// CHECK-NEXT:         #[bits(6)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(26)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(16)]
// CHECK-NEXT:         pub i: u32,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(8)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_2:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn ret1() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ret2() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ret3() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ret4() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ret5() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ret6() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ret7() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ret8() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ret9() -> u32 {
// CHECK-NEXT:     ((unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() }) as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target int32plus }
// CHECK-NEXT: // Test arithmetics on bitfields.
// CHECK-NEXT: fn fn1_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = (([[V_1]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_2]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u32 = (([[V_3]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_4]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u32 = (([[V_5]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_6]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u32 = (([[V_7]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_8]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u32 = (([[V_9]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_10]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: u32 = (([[V_11]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_12]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: u32 = (([[V_13]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_14]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: u32 = (([[V_15]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_16]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: u32 = (([[V_17]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_18]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: u32 = (([[V_19]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_20]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: u32 = (([[V_21]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_22]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: u32 = (([[V_23]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_24]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_26:__v[0-9]+]]: u32 = (([[V_25]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_26]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_27:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_28:__v[0-9]+]]: u32 = (([[V_27]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_28]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_29:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_30:__v[0-9]+]]: u32 = (([[V_29]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_30]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_31:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_32:__v[0-9]+]]: u32 = (([[V_31]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_32]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_33:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_34:__v[0-9]+]]: u32 = (([[V_33]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_34]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_35:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_36:__v[0-9]+]]: u32 = (([[V_35]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_36]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_37:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_38:__v[0-9]+]]: u32 = (([[V_37]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_38]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_39:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_40:__v[0-9]+]]: u32 = (([[V_39]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_40]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_41:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_42:__v[0-9]+]]: u32 = (([[V_41]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_42]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_43:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_44:__v[0-9]+]]: u32 = (([[V_43]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_44]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_45:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_46:__v[0-9]+]]: u32 = (([[V_45]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_46]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_47:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_48:__v[0-9]+]]: u32 = (([[V_47]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_48]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_49:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_50:__v[0-9]+]]: u32 = (([[V_49]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_50]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_51:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_52:__v[0-9]+]]: u32 = (([[V_51]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_52]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_53:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_54:__v[0-9]+]]: u32 = (([[V_53]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_54]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_55:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_56:__v[0-9]+]]: u32 = (([[V_55]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_56]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_57:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_58:__v[0-9]+]]: u32 = (([[V_57]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_58]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_59:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_60:__v[0-9]+]]: u32 = (([[V_59]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_60]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_61:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_62:__v[0-9]+]]: u32 = (([[V_61]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_62]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_63:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_64:__v[0-9]+]]: u32 = (([[V_63]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_64]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_65:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_66:__v[0-9]+]]: u32 = (([[V_65]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_66]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_67:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_68:__v[0-9]+]]: u32 = (([[V_67]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_68]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_69:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_70:__v[0-9]+]]: u32 = (([[V_69]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_70]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_71:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_72:__v[0-9]+]]: u32 = (([[V_71]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_72]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_73:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_74:__v[0-9]+]]: u32 = (([[V_73]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_74]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_75:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_76:__v[0-9]+]]: u32 = (([[V_75]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_76]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_77:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_78:__v[0-9]+]]: u32 = (([[V_77]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_78]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_79:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_80:__v[0-9]+]]: u32 = (([[V_79]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_80]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_81:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_82:__v[0-9]+]]: u32 = (([[V_81]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_82]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_83:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_84:__v[0-9]+]]: u32 = (([[V_83]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_84]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_85:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_86:__v[0-9]+]]: u32 = (([[V_85]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_86]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_87:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_88:__v[0-9]+]]: u32 = (([[V_87]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_88]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_89:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_90:__v[0-9]+]]: u32 = (([[V_89]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_90]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_91:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_92:__v[0-9]+]]: u32 = (([[V_91]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_92]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_93:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_94:__v[0-9]+]]: u32 = (([[V_93]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_94]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_95:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_96:__v[0-9]+]]: u32 = (([[V_95]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_96]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_97:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_98:__v[0-9]+]]: u32 = (([[V_97]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_98]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_99:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_100:__v[0-9]+]]: u32 = (([[V_99]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_100]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_101:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_102:__v[0-9]+]]: u32 = (([[V_101]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_102]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_103:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_104:__v[0-9]+]]: u32 = (([[V_103]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_104]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_105:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_106:__v[0-9]+]]: u32 = (([[V_105]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_106]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_107:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_108:__v[0-9]+]]: u32 = (([[V_107]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_108]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_109:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_110:__v[0-9]+]]: u32 = (([[V_109]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_110]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_111:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_112:__v[0-9]+]]: u32 = (([[V_111]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_112]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_113:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_114:__v[0-9]+]]: u32 = (([[V_113]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_114]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_115:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_116:__v[0-9]+]]: u32 = (([[V_115]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_116]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_117:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_118:__v[0-9]+]]: u32 = (([[V_117]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_118]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_119:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_120:__v[0-9]+]]: u32 = (([[V_119]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_120]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_121:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_122:__v[0-9]+]]: u32 = (([[V_121]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_122]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_123:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_124:__v[0-9]+]]: u32 = (([[V_123]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_124]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_125:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_126:__v[0-9]+]]: u32 = (([[V_125]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_126]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_127:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_128:__v[0-9]+]]: u32 = (([[V_127]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_128]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_129:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_130:__v[0-9]+]]: u32 = (([[V_129]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_130]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_131:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_132:__v[0-9]+]]: u32 = (([[V_131]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_132]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_133:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_134:__v[0-9]+]]: u32 = (([[V_133]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_134]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_135:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_136:__v[0-9]+]]: u32 = (([[V_135]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_136]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_137:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_138:__v[0-9]+]]: u32 = (([[V_137]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_138]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_139:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_140:__v[0-9]+]]: u32 = (([[V_139]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_140]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_141:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_142:__v[0-9]+]]: u32 = (([[V_141]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_142]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_143:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_144:__v[0-9]+]]: u32 = (([[V_143]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_144]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_145:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_146:__v[0-9]+]]: u32 = (([[V_145]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_146]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_147:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_148:__v[0-9]+]]: u32 = (([[V_147]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_148]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_149:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_150:__v[0-9]+]]: u32 = (([[V_149]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_150]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_151:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_152:__v[0-9]+]]: u32 = (([[V_151]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_152]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_153:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_154:__v[0-9]+]]: u32 = (([[V_153]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_154]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_155:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_156:__v[0-9]+]]: u32 = (([[V_155]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_156]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_157:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_158:__v[0-9]+]]: u32 = (([[V_157]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_158]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_159:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_160:__v[0-9]+]]: u32 = (([[V_159]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_160]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_161:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_162:__v[0-9]+]]: u32 = (([[V_161]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_162]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_163:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_164:__v[0-9]+]]: u32 = (([[V_163]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_164]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_165:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_166:__v[0-9]+]]: u32 = (([[V_165]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_166]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_167:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_168:__v[0-9]+]]: u32 = (([[V_167]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_168]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_169:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_170:__v[0-9]+]]: u32 = (([[V_169]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_170]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_171:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_172:__v[0-9]+]]: u32 = (([[V_171]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_172]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_173:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_174:__v[0-9]+]]: u32 = (([[V_173]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_174]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_175:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_176:__v[0-9]+]]: u32 = (([[V_175]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_176]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_177:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_178:__v[0-9]+]]: u32 = (([[V_177]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_178]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_179:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_180:__v[0-9]+]]: u32 = (([[V_179]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_180]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_181:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_182:__v[0-9]+]]: u32 = (([[V_181]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_182]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_183:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_184:__v[0-9]+]]: u32 = (([[V_183]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_184]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_185:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_186:__v[0-9]+]]: u32 = (([[V_185]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_186]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_187:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_188:__v[0-9]+]]: u32 = (([[V_187]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_188]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_189:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_190:__v[0-9]+]]: u32 = (([[V_189]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_190]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_191:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_192:__v[0-9]+]]: u32 = (([[V_191]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_192]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_193:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_194:__v[0-9]+]]: u32 = (([[V_193]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_194]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_195:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_196:__v[0-9]+]]: u32 = (([[V_195]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_196]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_197:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_198:__v[0-9]+]]: u32 = (([[V_197]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_198]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_199:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_200:__v[0-9]+]]: u32 = (([[V_199]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_200]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_201:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_202:__v[0-9]+]]: u32 = (([[V_201]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_202]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_203:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_204:__v[0-9]+]]: u32 = (([[V_203]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_204]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_205:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_206:__v[0-9]+]]: u32 = (([[V_205]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_206]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_207:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_208:__v[0-9]+]]: u32 = (([[V_207]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_208]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_209:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_210:__v[0-9]+]]: u32 = (([[V_209]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_210]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_211:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_212:__v[0-9]+]]: u32 = (([[V_211]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_212]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_213:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_214:__v[0-9]+]]: u32 = (([[V_213]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_214]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_215:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_216:__v[0-9]+]]: u32 = (([[V_215]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_216]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_217:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_218:__v[0-9]+]]: u32 = (([[V_217]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_218]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_219:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_220:__v[0-9]+]]: u32 = (([[V_219]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_220]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_221:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_222:__v[0-9]+]]: u32 = (([[V_221]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_222]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_223:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_224:__v[0-9]+]]: u32 = (([[V_223]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_224]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_225:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_226:__v[0-9]+]]: u32 = (([[V_225]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_226]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_227:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_228:__v[0-9]+]]: u32 = (([[V_227]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_228]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_229:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_230:__v[0-9]+]]: u32 = (([[V_229]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_230]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_231:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_232:__v[0-9]+]]: u32 = (([[V_231]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_232]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_233:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_234:__v[0-9]+]]: u32 = (([[V_233]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_234]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_235:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_236:__v[0-9]+]]: u32 = (([[V_235]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_236]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_237:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_238:__v[0-9]+]]: u32 = (([[V_237]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_238]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_239:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_240:__v[0-9]+]]: u32 = (([[V_239]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_240]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_241:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_242:__v[0-9]+]]: u32 = (([[V_241]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_242]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_243:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_244:__v[0-9]+]]: u32 = (([[V_243]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_244]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_245:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_246:__v[0-9]+]]: u32 = (([[V_245]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_246]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_247:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_248:__v[0-9]+]]: u32 = (([[V_247]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_248]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_249:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_250:__v[0-9]+]]: u32 = (([[V_249]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_250]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_251:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_252:__v[0-9]+]]: u32 = (([[V_251]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_252]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_253:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_254:__v[0-9]+]]: u32 = (([[V_253]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_254]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_255:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_256:__v[0-9]+]]: u32 = (([[V_255]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_256]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_257:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_258:__v[0-9]+]]: u32 = (([[V_257]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_258]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_259:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_260:__v[0-9]+]]: u32 = (([[V_259]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_260]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_261:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_262:__v[0-9]+]]: u32 = (([[V_261]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_262]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_263:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_264:__v[0-9]+]]: u32 = (([[V_263]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_264]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_265:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_266:__v[0-9]+]]: u32 = (([[V_265]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_266]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_267:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_268:__v[0-9]+]]: u32 = (([[V_267]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_268]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_269:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_270:__v[0-9]+]]: u32 = (([[V_269]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_270]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_271:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_272:__v[0-9]+]]: u32 = (([[V_271]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_272]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_273:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_274:__v[0-9]+]]: u32 = (([[V_273]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_274]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_275:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_276:__v[0-9]+]]: u32 = (([[V_275]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_276]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_277:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_278:__v[0-9]+]]: u32 = (([[V_277]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_278]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_279:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_280:__v[0-9]+]]: u32 = (([[V_279]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_280]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_281:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_282:__v[0-9]+]]: u32 = (([[V_281]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_282]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_283:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_284:__v[0-9]+]]: u32 = (([[V_283]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_284]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_285:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_286:__v[0-9]+]]: u32 = (([[V_285]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_286]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_287:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_288:__v[0-9]+]]: u32 = (([[V_287]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_288]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_289:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_290:__v[0-9]+]]: u32 = (([[V_289]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_290]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_291:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_292:__v[0-9]+]]: u32 = (([[V_291]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_292]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_293:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_294:__v[0-9]+]]: u32 = (([[V_293]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_294]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_295:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_296:__v[0-9]+]]: u32 = (([[V_295]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_296]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_297:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_298:__v[0-9]+]]: u32 = (([[V_297]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_298]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_299:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_300:__v[0-9]+]]: u32 = (([[V_299]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_300]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_301:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_302:__v[0-9]+]]: u32 = (([[V_301]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_302]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_303:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_304:__v[0-9]+]]: u32 = (([[V_303]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_304]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_305:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_306:__v[0-9]+]]: u32 = (([[V_305]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_306]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_307:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_308:__v[0-9]+]]: u32 = (([[V_307]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_308]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_309:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_310:__v[0-9]+]]: u32 = (([[V_309]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_310]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_311:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_312:__v[0-9]+]]: u32 = (([[V_311]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_312]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_313:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_314:__v[0-9]+]]: u32 = (([[V_313]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_314]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_315:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_316:__v[0-9]+]]: u32 = (([[V_315]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_316]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_317:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_318:__v[0-9]+]]: u32 = (([[V_317]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_318]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_319:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_320:__v[0-9]+]]: u32 = (([[V_319]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_320]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_321:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_322:__v[0-9]+]]: u32 = (([[V_321]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_322]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_323:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_324:__v[0-9]+]]: u32 = (([[V_323]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_324]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // { dg-require-effective-target int32plus }
// CHECK-NEXT:     // Test arithmetics on bitfields.
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_1((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) + (3 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_1((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) + (251 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_1((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != (((31278 as i32) + (13279 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_1((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) + (24 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_1((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) + (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_1((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) + (264151 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_1((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) + (713 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_1((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) + (17 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_1((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) + (199 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_2((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) + (1 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_2((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) + (1 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_2((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) + (1 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_2((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) + (1 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_2((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) + (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_2((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) + (1 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_2((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) + (1 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_2((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) + (1 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_2((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) + (1 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_3((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) + (1 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_3((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) + (1 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_3((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) + (1 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_3((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) + (1 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_3((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) + (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_3((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) + (1 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_3((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) + (1 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_3((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) + (1 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_3((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) + (1 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_4((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) - (3 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_4((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) - (251 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_4((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != (((31278 as i32) - (13279 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_4((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) - (24 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_4((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) - (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_4((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) - (264151 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_4((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) - (713 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_4((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) - (17 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_4((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) - (199 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_5((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) - (1 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_5((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) - (1 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_5((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) - (1 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_5((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) - (1 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_5((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) - (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_5((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) - (1 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_5((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) - (1 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_5((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) - (1 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_5((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) - (1 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_6((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) - (1 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_6((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) - (1 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_6((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) - (1 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_6((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) - (1 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_6((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) - (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_6((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) - (1 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_6((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) - (1 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_6((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) - (1 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_6((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) - (1 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_7((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) & (3 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_7((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) & (251 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_7((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != (((31278 as i32) & (13279 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_7((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) & (24 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_7((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) & (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_7((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) & (264151 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_7((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) & (713 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_7((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) & (17 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_7((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) & (199 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_8((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != ((((51 as i32) | (3 as i32)) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_8((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != ((((636 as i32) | (251 as i32)) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_8((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != ((((31278 as i32) | (13279 as i32)) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_8((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != ((((21 as i32) | (24 as i32)) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_8((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != ((((1 as i32) | (1 as i32)) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_8((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != ((((33554432 as i32) | (264151 as i32)) & ((1 as i32) << (26 as i32)) - (1 as i32))
// CHECK-NEXT:             as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_8((713 as i32) as u32);
// CHECK-NEXT:     if ret7()
// CHECK-NEXT:         != ((((26812 as i32) | (713 as i32)) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_8((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != ((((156 as i32) | (17 as i32)) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_8((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != ((((187 as i32) | (199 as i32)) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_9((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != ((((51 as i32) ^ (3 as i32)) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_9((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != ((((636 as i32) ^ (251 as i32)) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_9((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != ((((31278 as i32) ^ (13279 as i32)) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_9((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != ((((21 as i32) ^ (24 as i32)) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_9((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != ((((1 as i32) ^ (1 as i32)) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_9((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != ((((33554432 as i32) ^ (264151 as i32)) & ((1 as i32) << (26 as i32)) - (1 as i32))
// CHECK-NEXT:             as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_9((713 as i32) as u32);
// CHECK-NEXT:     if ret7()
// CHECK-NEXT:         != ((((26812 as i32) ^ (713 as i32)) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_9((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != ((((156 as i32) ^ (17 as i32)) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_9((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != ((((187 as i32) ^ (199 as i32)) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_a((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) / (3 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_a((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) / (251 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_a((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != (((31278 as i32) / (13279 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_a((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) / (24 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_a((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) / (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_a((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) / (264151 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_a((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) / (713 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_a((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) / (17 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_a((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) / (199 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_b((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) % (3 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_b((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) % (251 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_b((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != (((31278 as i32) % (13279 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_b((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) % (24 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_b((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) % (1 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_b((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) % (264151 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_b((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) % (713 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_b((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) % (17 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_b((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) % (199 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_c((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) + (3 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_c((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) + (3 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_c((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) + (3 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_c((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) + (3 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_c((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) + (3 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_c((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) + (3 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_c((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) + (3 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_c((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) + (3 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_c((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) + (3 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_d((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) - (7 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_d((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) - (7 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_d((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) - (7 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_d((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) - (7 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_d((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) - (7 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_d((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) - (7 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_d((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) - (7 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_d((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) - (7 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_d((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) - (7 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_e((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) & (21 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_e((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) & (21 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_e((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) & (21 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_e((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) & (21 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_e((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) & (21 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_e((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) & (21 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_e((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) & (21 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_e((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) & (21 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_e((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) & (21 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_f((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != ((((51 as i32) | (19 as i32)) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_f((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != ((((636 as i32) | (19 as i32)) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_f((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != ((((31278 as i32) | (19 as i32)) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_f((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != ((((21 as i32) | (19 as i32)) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_f((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != ((((1 as i32) | (19 as i32)) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_f((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != ((((33554432 as i32) | (19 as i32)) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_f((713 as i32) as u32);
// CHECK-NEXT:     if ret7()
// CHECK-NEXT:         != ((((26812 as i32) | (19 as i32)) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_f((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != ((((156 as i32) | (19 as i32)) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_f((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != ((((187 as i32) | (19 as i32)) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_g((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != ((((51 as i32) ^ (37 as i32)) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_g((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != ((((636 as i32) ^ (37 as i32)) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_g((13279 as i32) as u32);
// CHECK-NEXT:     if ret3()
// CHECK-NEXT:         != ((((31278 as i32) ^ (37 as i32)) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_g((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != ((((21 as i32) ^ (37 as i32)) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_g((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != ((((1 as i32) ^ (37 as i32)) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_g((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != ((((33554432 as i32) ^ (37 as i32)) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_g((713 as i32) as u32);
// CHECK-NEXT:     if ret7()
// CHECK-NEXT:         != ((((26812 as i32) ^ (37 as i32)) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_g((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != ((((156 as i32) ^ (37 as i32)) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_g((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != ((((187 as i32) ^ (37 as i32)) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_h((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) / (17 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_h((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) / (17 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_h((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) / (17 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_h((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) / (17 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_h((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) / (17 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_h((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) / (17 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_h((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) / (17 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_h((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) / (17 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_h((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) / (17 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1_i((3 as i32) as u32);
// CHECK-NEXT:     if ret1() != (((51 as i32) % (19 as i32) & ((1 as i32) << (6 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i((51 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn2_i((251 as i32) as u32);
// CHECK-NEXT:     if ret2() != (((636 as i32) % (19 as i32) & ((1 as i32) << (11 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((636 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3_i((13279 as i32) as u32);
// CHECK-NEXT:     if ret3() != (((31278 as i32) % (19 as i32) & ((1 as i32) << (15 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j((31278 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn4_i((24 as i32) as u32);
// CHECK-NEXT:     if ret4() != (((21 as i32) % (19 as i32) & ((1 as i32) << (5 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i((21 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn5_i((1 as i32) as u32);
// CHECK-NEXT:     if ret5() != (((1 as i32) % (19 as i32) & ((1 as i32) << (1 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn6_i((264151 as i32) as u32);
// CHECK-NEXT:     if ret6()
// CHECK-NEXT:         != (((33554432 as i32) % (19 as i32) & ((1 as i32) << (26 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k((33554432 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn7_i((713 as i32) as u32);
// CHECK-NEXT:     if ret7() != (((26812 as i32) % (19 as i32) & ((1 as i32) << (16 as i32)) - (1 as i32)) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i((26812 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn8_i((17 as i32) as u32);
// CHECK-NEXT:     if ret8() != (((156 as i32) % (19 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j((156 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     fn9_i((199 as i32) as u32);
// CHECK-NEXT:     if ret9() != (((187 as i32) % (19 as i32) & ((1 as i32) << (8 as i32)) - (1 as i32)) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k((187 as i32) as u32);
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
