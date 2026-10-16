/* PR target/85095 */

__attribute__((noipa)) unsigned long f1(unsigned long a, unsigned long b) {
  unsigned long i = __builtin_add_overflow(a, b, &a);
  return a + i;
}

__attribute__((noipa)) unsigned long f2(unsigned long a, unsigned long b) {
  unsigned long i = __builtin_add_overflow(a, b, &a);
  return a - i;
}

__attribute__((noipa)) unsigned long f3(unsigned int a, unsigned int b) {
  unsigned int i = __builtin_add_overflow(a, b, &a);
  return a + i;
}

__attribute__((noipa)) unsigned long f4(unsigned int a, unsigned int b) {
  unsigned int i = __builtin_add_overflow(a, b, &a);
  return a - i;
}

int main() {
  if (f1(16UL, -18UL) != -2UL || f1(16UL, -17UL) != -1UL ||
      f1(16UL, -16UL) != 1UL || f1(16UL, -15UL) != 2UL ||
      f2(24UL, -26UL) != -2UL || f2(24UL, -25UL) != -1UL ||
      f2(24UL, -24UL) != -1UL || f2(24UL, -23UL) != 0UL ||
      f3(32U, -34U) != -2U || f3(32U, -33U) != -1U || f3(32U, -32U) != 1U ||
      f3(32U, -31U) != 2U || f4(35U, -37U) != -2U || f4(35U, -36U) != -1U ||
      f4(35U, -35U) != -1U || f4(35U, -34U) != 0U)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/85095
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut a: u64, mut b: u64) -> u64 {
// CHECK-NEXT:     let mut i: u64 = ({
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(a) = [[T_2]] as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as u64) as i128) != [[T_2]]
// CHECK-NEXT:     }) as u64;
// CHECK-NEXT:     a.wrapping_add(i)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut a: u64, mut b: u64) -> u64 {
// CHECK-NEXT:     let mut i: u64 = ({
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(a) = [[T_2]] as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as u64) as i128) != [[T_2]]
// CHECK-NEXT:     }) as u64;
// CHECK-NEXT:     a.wrapping_sub(i)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut a: u32, mut b: u32) -> u64 {
// CHECK-NEXT:     let mut i: u32 = ({
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(a) = [[T_2]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as u32) as i128) != [[T_2]]
// CHECK-NEXT:     }) as u32;
// CHECK-NEXT:     a.wrapping_add(i) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f4(mut a: u32, mut b: u32) -> u64 {
// CHECK-NEXT:     let mut i: u32 = ({
// CHECK-NEXT:         let [[T_0]]: i128 = a as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = b as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(a) = [[T_2]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as u32) as i128) != [[T_2]]
// CHECK-NEXT:     }) as u32;
// CHECK-NEXT:     a.wrapping_sub(i) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f1(16 as u64, (18 as u64).wrapping_neg()) != (2 as u64).wrapping_neg() {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = f1(16 as u64, (17 as u64).wrapping_neg()) != (1 as u64).wrapping_neg();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = f1(16 as u64, (16 as u64).wrapping_neg()) != (1 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = f1(16 as u64, (15 as u64).wrapping_neg()) != (2 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = f2(24 as u64, (26 as u64).wrapping_neg()) != (2 as u64).wrapping_neg();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = f2(24 as u64, (25 as u64).wrapping_neg()) != (1 as u64).wrapping_neg();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = f2(24 as u64, (24 as u64).wrapping_neg()) != (1 as u64).wrapping_neg();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         [[V_6]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = f2(24 as u64, (23 as u64).wrapping_neg()) != (0 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_6]] {
// CHECK-NEXT:         [[V_7]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = f3(32 as u32, (34 as u32).wrapping_neg()) != ((2 as u32).wrapping_neg() as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_7]] {
// CHECK-NEXT:         [[V_8]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_8]] = f3(32 as u32, (33 as u32).wrapping_neg()) != ((1 as u32).wrapping_neg() as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_8]] {
// CHECK-NEXT:         [[V_9]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_9]] = f3(32 as u32, (32 as u32).wrapping_neg()) != ((1 as u32) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_9]] {
// CHECK-NEXT:         [[V_10]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_10]] = f3(32 as u32, (31 as u32).wrapping_neg()) != ((2 as u32) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_10]] {
// CHECK-NEXT:         [[V_11]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_11]] = f4(35 as u32, (37 as u32).wrapping_neg()) != ((2 as u32).wrapping_neg() as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_11]] {
// CHECK-NEXT:         [[V_12]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_12]] = f4(35 as u32, (36 as u32).wrapping_neg()) != ((1 as u32).wrapping_neg() as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_12]] {
// CHECK-NEXT:         [[V_13]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_13]] = f4(35 as u32, (35 as u32).wrapping_neg()) != ((1 as u32).wrapping_neg() as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_13]] {
// CHECK-NEXT:         [[V_14]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_14]] = f4(35 as u32, (34 as u32).wrapping_neg()) != ((0 as u32) as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_14]] {
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
