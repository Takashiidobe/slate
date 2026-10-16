/* { dg-require-effective-target int32plus } */

#define FIELDS1 long long l;
#include "20040629-1.c"

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     l: i64,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     l: i64,
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
// CHECK-NEXT:     l: i64,
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
// CHECK-NEXT: fn fn1_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = (([[V_0]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_1]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = (([[V_2]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_3]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u32 = (([[V_4]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_5]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u32 = (([[V_6]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_7]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u32 = (([[V_8]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_9]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u32 = (([[V_10]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_11]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: u32 = (([[V_12]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_13]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: u32 = (([[V_14]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_15]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_1(mut x: u32) {
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: u32 = (([[V_16]] as i32) as u32).wrapping_add(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_17]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: u32 = (([[V_18]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_19]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: u32 = (([[V_20]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_21]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: u32 = (([[V_22]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_23]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: u32 = (([[V_24]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_25]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_26:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_27:__v[0-9]+]]: u32 = (([[V_26]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_27]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_28:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_29:__v[0-9]+]]: u32 = (([[V_28]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_29]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_30:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_31:__v[0-9]+]]: u32 = (([[V_30]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_31]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_32:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_33:__v[0-9]+]]: u32 = (([[V_32]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_33]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_2(mut x: u32) {
// CHECK-NEXT:     let mut [[V_34:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_35:__v[0-9]+]]: u32 = (([[V_34]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_35]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_36:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_37:__v[0-9]+]]: u32 = (([[V_36]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_37]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_38:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_39:__v[0-9]+]]: u32 = (([[V_38]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_39]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_40:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_41:__v[0-9]+]]: u32 = (([[V_40]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_41]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_42:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_43:__v[0-9]+]]: u32 = (([[V_42]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_43]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_44:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_45:__v[0-9]+]]: u32 = (([[V_44]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_45]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_46:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_47:__v[0-9]+]]: u32 = (([[V_46]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_47]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_48:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_49:__v[0-9]+]]: u32 = (([[V_48]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_49]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_50:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_51:__v[0-9]+]]: u32 = (([[V_50]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_51]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_3(mut x: u32) {
// CHECK-NEXT:     let mut [[V_52:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_53:__v[0-9]+]]: u32 = (([[V_52]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_53]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_54:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_55:__v[0-9]+]]: u32 = (([[V_54]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_55]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_56:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_57:__v[0-9]+]]: u32 = (([[V_56]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_57]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_58:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_59:__v[0-9]+]]: u32 = (([[V_58]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_59]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_60:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_61:__v[0-9]+]]: u32 = (([[V_60]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_61]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_62:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_63:__v[0-9]+]]: u32 = (([[V_62]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_63]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_64:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_65:__v[0-9]+]]: u32 = (([[V_64]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_65]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_66:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_67:__v[0-9]+]]: u32 = (([[V_66]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_67]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_68:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_69:__v[0-9]+]]: u32 = (([[V_68]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_69]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_4(mut x: u32) {
// CHECK-NEXT:     let mut [[V_70:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_71:__v[0-9]+]]: u32 = (([[V_70]] as i32) as u32).wrapping_sub(x);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_71]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_72:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_73:__v[0-9]+]]: u32 = (([[V_72]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_73]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_74:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_75:__v[0-9]+]]: u32 = (([[V_74]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_75]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_76:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_77:__v[0-9]+]]: u32 = (([[V_76]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_77]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_78:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_79:__v[0-9]+]]: u32 = (([[V_78]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_79]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_80:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_81:__v[0-9]+]]: u32 = (([[V_80]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_81]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_82:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_83:__v[0-9]+]]: u32 = (([[V_82]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_83]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_84:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_85:__v[0-9]+]]: u32 = (([[V_84]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_85]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_86:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_87:__v[0-9]+]]: u32 = (([[V_86]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_87]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_5(mut x: u32) {
// CHECK-NEXT:     let mut [[V_88:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_89:__v[0-9]+]]: u32 = (([[V_88]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_89]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_90:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_91:__v[0-9]+]]: u32 = (([[V_90]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_91]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_92:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_93:__v[0-9]+]]: u32 = (([[V_92]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_93]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_94:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_95:__v[0-9]+]]: u32 = (([[V_94]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_95]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_96:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_97:__v[0-9]+]]: u32 = (([[V_96]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_97]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_98:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_99:__v[0-9]+]]: u32 = (([[V_98]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_99]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_100:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_101:__v[0-9]+]]: u32 = (([[V_100]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_101]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_102:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_103:__v[0-9]+]]: u32 = (([[V_102]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_103]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_104:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_105:__v[0-9]+]]: u32 = (([[V_104]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_105]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_6(mut x: u32) {
// CHECK-NEXT:     let mut [[V_106:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_107:__v[0-9]+]]: u32 = (([[V_106]] as i32) - (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_107]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_108:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_109:__v[0-9]+]]: u32 = (([[V_108]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_109]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_110:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_111:__v[0-9]+]]: u32 = (([[V_110]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_111]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_112:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_113:__v[0-9]+]]: u32 = (([[V_112]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_113]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_114:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_115:__v[0-9]+]]: u32 = (([[V_114]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_115]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_116:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_117:__v[0-9]+]]: u32 = (([[V_116]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_117]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_118:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_119:__v[0-9]+]]: u32 = (([[V_118]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_119]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_120:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_121:__v[0-9]+]]: u32 = (([[V_120]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_121]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_122:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_123:__v[0-9]+]]: u32 = (([[V_122]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_123]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_7(mut x: u32) {
// CHECK-NEXT:     let mut [[V_124:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_125:__v[0-9]+]]: u32 = (([[V_124]] as i32) as u32) & x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_125]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_126:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_127:__v[0-9]+]]: u32 = (([[V_126]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_127]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_128:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_129:__v[0-9]+]]: u32 = (([[V_128]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_129]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_130:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_131:__v[0-9]+]]: u32 = (([[V_130]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_131]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_132:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_133:__v[0-9]+]]: u32 = (([[V_132]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_133]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_134:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_135:__v[0-9]+]]: u32 = (([[V_134]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_135]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_136:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_137:__v[0-9]+]]: u32 = (([[V_136]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_137]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_138:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_139:__v[0-9]+]]: u32 = (([[V_138]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_139]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_140:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_141:__v[0-9]+]]: u32 = (([[V_140]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_141]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_8(mut x: u32) {
// CHECK-NEXT:     let mut [[V_142:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_143:__v[0-9]+]]: u32 = (([[V_142]] as i32) as u32) | x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_143]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_144:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_145:__v[0-9]+]]: u32 = (([[V_144]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_145]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_146:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_147:__v[0-9]+]]: u32 = (([[V_146]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_147]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_148:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_149:__v[0-9]+]]: u32 = (([[V_148]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_149]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_150:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_151:__v[0-9]+]]: u32 = (([[V_150]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_151]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_152:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_153:__v[0-9]+]]: u32 = (([[V_152]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_153]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_154:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_155:__v[0-9]+]]: u32 = (([[V_154]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_155]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_156:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_157:__v[0-9]+]]: u32 = (([[V_156]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_157]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_158:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_159:__v[0-9]+]]: u32 = (([[V_158]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_159]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_9(mut x: u32) {
// CHECK-NEXT:     let mut [[V_160:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_161:__v[0-9]+]]: u32 = (([[V_160]] as i32) as u32) ^ x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_161]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_162:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_163:__v[0-9]+]]: u32 = (([[V_162]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_163]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_164:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_165:__v[0-9]+]]: u32 = (([[V_164]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_165]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_166:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_167:__v[0-9]+]]: u32 = (([[V_166]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_167]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_168:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_169:__v[0-9]+]]: u32 = (([[V_168]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_169]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_170:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_171:__v[0-9]+]]: u32 = (([[V_170]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_171]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_172:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_173:__v[0-9]+]]: u32 = (([[V_172]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_173]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_174:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_175:__v[0-9]+]]: u32 = (([[V_174]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_175]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_176:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_177:__v[0-9]+]]: u32 = (([[V_176]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_177]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_a(mut x: u32) {
// CHECK-NEXT:     let mut [[V_178:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_179:__v[0-9]+]]: u32 = (([[V_178]] as i32) as u32) / x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_179]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_180:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_181:__v[0-9]+]]: u32 = (([[V_180]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_181]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_182:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_183:__v[0-9]+]]: u32 = (([[V_182]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_183]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_184:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_185:__v[0-9]+]]: u32 = (([[V_184]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_185]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_186:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_187:__v[0-9]+]]: u32 = (([[V_186]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_187]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_188:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_189:__v[0-9]+]]: u32 = (([[V_188]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_189]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_190:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_191:__v[0-9]+]]: u32 = (([[V_190]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_191]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_192:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_193:__v[0-9]+]]: u32 = (([[V_192]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_193]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_194:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_195:__v[0-9]+]]: u32 = (([[V_194]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_195]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_b(mut x: u32) {
// CHECK-NEXT:     let mut [[V_196:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_197:__v[0-9]+]]: u32 = (([[V_196]] as i32) as u32) % x;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_197]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_198:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_199:__v[0-9]+]]: u32 = (([[V_198]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_199]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_200:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_201:__v[0-9]+]]: u32 = (([[V_200]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_201]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_202:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_203:__v[0-9]+]]: u32 = (([[V_202]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_203]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_204:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_205:__v[0-9]+]]: u32 = (([[V_204]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_205]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_206:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_207:__v[0-9]+]]: u32 = (([[V_206]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_207]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_208:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_209:__v[0-9]+]]: u32 = (([[V_208]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_209]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_210:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_211:__v[0-9]+]]: u32 = (([[V_210]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_211]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_212:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_213:__v[0-9]+]]: u32 = (([[V_212]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_213]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_c(mut x: u32) {
// CHECK-NEXT:     let mut [[V_214:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_215:__v[0-9]+]]: u32 = (([[V_214]] as i32) + (3 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_215]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_216:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_217:__v[0-9]+]]: u32 = (([[V_216]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_217]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_218:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_219:__v[0-9]+]]: u32 = (([[V_218]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_219]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_220:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_221:__v[0-9]+]]: u32 = (([[V_220]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_221]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_222:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_223:__v[0-9]+]]: u32 = (([[V_222]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_223]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_224:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_225:__v[0-9]+]]: u32 = (([[V_224]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_225]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_226:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_227:__v[0-9]+]]: u32 = (([[V_226]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_227]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_228:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_229:__v[0-9]+]]: u32 = (([[V_228]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_229]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_230:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_231:__v[0-9]+]]: u32 = (([[V_230]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_231]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_d(mut x: u32) {
// CHECK-NEXT:     let mut [[V_232:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_233:__v[0-9]+]]: u32 = (([[V_232]] as i32) - (7 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_233]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_234:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_235:__v[0-9]+]]: u32 = (([[V_234]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_235]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_236:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_237:__v[0-9]+]]: u32 = (([[V_236]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_237]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_238:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_239:__v[0-9]+]]: u32 = (([[V_238]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_239]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_240:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_241:__v[0-9]+]]: u32 = (([[V_240]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_241]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_242:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_243:__v[0-9]+]]: u32 = (([[V_242]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_243]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_244:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_245:__v[0-9]+]]: u32 = (([[V_244]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_245]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_246:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_247:__v[0-9]+]]: u32 = (([[V_246]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_247]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_248:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_249:__v[0-9]+]]: u32 = (([[V_248]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_249]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_e(mut x: u32) {
// CHECK-NEXT:     let mut [[V_250:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_251:__v[0-9]+]]: u32 = (([[V_250]] as i32) & (21 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_251]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_252:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_253:__v[0-9]+]]: u32 = (([[V_252]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_253]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_254:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_255:__v[0-9]+]]: u32 = (([[V_254]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_255]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_256:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_257:__v[0-9]+]]: u32 = (([[V_256]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_257]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_258:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_259:__v[0-9]+]]: u32 = (([[V_258]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_259]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_260:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_261:__v[0-9]+]]: u32 = (([[V_260]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_261]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_262:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_263:__v[0-9]+]]: u32 = (([[V_262]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_263]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_264:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_265:__v[0-9]+]]: u32 = (([[V_264]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_265]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_266:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_267:__v[0-9]+]]: u32 = (([[V_266]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_267]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_f(mut x: u32) {
// CHECK-NEXT:     let mut [[V_268:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_269:__v[0-9]+]]: u32 = (([[V_268]] as i32) | (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_269]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_270:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_271:__v[0-9]+]]: u32 = (([[V_270]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_271]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_272:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_273:__v[0-9]+]]: u32 = (([[V_272]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_273]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_274:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_275:__v[0-9]+]]: u32 = (([[V_274]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_275]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_276:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_277:__v[0-9]+]]: u32 = (([[V_276]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_277]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_278:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_279:__v[0-9]+]]: u32 = (([[V_278]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_279]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_280:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_281:__v[0-9]+]]: u32 = (([[V_280]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_281]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_282:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_283:__v[0-9]+]]: u32 = (([[V_282]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_283]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_284:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_285:__v[0-9]+]]: u32 = (([[V_284]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_285]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_g(mut x: u32) {
// CHECK-NEXT:     let mut [[V_286:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_287:__v[0-9]+]]: u32 = (([[V_286]] as i32) ^ (37 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_287]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_288:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_289:__v[0-9]+]]: u32 = (([[V_288]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_289]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_290:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_291:__v[0-9]+]]: u32 = (([[V_290]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_291]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_292:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_293:__v[0-9]+]]: u32 = (([[V_292]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_293]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_294:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_295:__v[0-9]+]]: u32 = (([[V_294]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_295]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_296:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_297:__v[0-9]+]]: u32 = (([[V_296]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_297]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_298:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_299:__v[0-9]+]]: u32 = (([[V_298]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_299]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_300:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_301:__v[0-9]+]]: u32 = (([[V_300]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_301]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_302:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_303:__v[0-9]+]]: u32 = (([[V_302]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_303]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_h(mut x: u32) {
// CHECK-NEXT:     let mut [[V_304:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_305:__v[0-9]+]]: u32 = (([[V_304]] as i32) / (17 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_305]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_306:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_307:__v[0-9]+]]: u32 = (([[V_306]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_i([[V_307]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_308:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_309:__v[0-9]+]]: u32 = (([[V_308]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_j([[V_309]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn3_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_310:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_311:__v[0-9]+]]: u32 = (([[V_310]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(b.__slate_bits_0)).__set_k([[V_311]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn4_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_312:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_313:__v[0-9]+]]: u32 = (([[V_312]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_i([[V_313]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn5_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_314:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_315:__v[0-9]+]]: u32 = (([[V_314]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_j([[V_315]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn6_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_316:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_317:__v[0-9]+]]: u32 = (([[V_316]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_k([[V_317]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn7_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_318:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_i() };
// CHECK-NEXT:     let mut [[V_319:__v[0-9]+]]: u32 = (([[V_318]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_i([[V_319]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn8_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_320:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_j() };
// CHECK-NEXT:     let mut [[V_321:__v[0-9]+]]: u32 = (([[V_320]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_j([[V_321]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn9_i(mut x: u32) {
// CHECK-NEXT:     let mut [[V_322:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(d.__slate_bits_0)).__get_k() };
// CHECK-NEXT:     let mut [[V_323:__v[0-9]+]]: u32 = (([[V_322]] as i32) % (19 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(d.__slate_bits_0)).__set_k([[V_323]]);
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
