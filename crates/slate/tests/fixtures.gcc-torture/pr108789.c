/* PR middle-end/108789 */

int add(unsigned *r, const unsigned *a, const unsigned *b) {
  return __builtin_add_overflow(*a, *b, r);
}

int mul(unsigned *r, const unsigned *a, const unsigned *b) {
  return __builtin_mul_overflow(*a, *b, r);
}

int main() {
  unsigned x;

  /* 1073741824U + 1073741824U should not overflow.  */
  x = (__INT_MAX__ + 1U) / 2;
  if (add(&x, &x, &x))
    __builtin_abort();

  /* 256U * 256U should not overflow */
  x = 1U << (sizeof(int) * __CHAR_BIT__ / 4);
  if (mul(&x, &x, &x))
    __builtin_abort();

  /* 2147483648U + 2147483648U should overflow */
  x = __INT_MAX__ + 1U;
  if (!add(&x, &x, &x))
    __builtin_abort();

  /* 65536U * 65536U should overflow */
  x = 1U << (sizeof(int) * __CHAR_BIT__ / 2);
  if (!mul(&x, &x, &x))
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/108789
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add(mut r: &mut u32, mut a: &u32, mut b: &u32) -> i32 {
// CHECK-NEXT:     ({
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = (unsafe { *(a as *const u32) }) as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = (unsafe { *(b as *const u32) }) as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(r as *mut u32) = [[T_2]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as u32) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mul(mut r: &mut u32, mut a: &u32, mut b: &u32) -> i32 {
// CHECK-NEXT:     ({
// CHECK-NEXT:         let [[T_0]]: i128 = (unsafe { *(a as *const u32) }) as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = (unsafe { *(b as *const u32) }) as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(r as *mut u32) = [[T_2]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u32) as i128) != [[T_2]]
// CHECK-NEXT:     }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u32 = 0 as u32;
// CHECK-NEXT:     // 1073741824U + 1073741824U should not overflow.
// CHECK-NEXT:     x = ((2147483647 as i32) as u32).wrapping_add(1 as u32) / ((2 as i32) as u32);
// CHECK-NEXT:     if add(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const u32)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const u32)) },
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // 256U * 256U should not overflow
// CHECK-NEXT:     x = (1 as u32)
// CHECK-NEXT:         << (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((4 as i32) as i64) as u64);
// CHECK-NEXT:     if mul(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const u32)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const u32)) },
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // 2147483648U + 2147483648U should overflow
// CHECK-NEXT:     x = ((2147483647 as i32) as u32).wrapping_add(1 as u32);
// CHECK-NEXT:     if !(add(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const u32)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const u32)) },
// CHECK-NEXT:     ) != (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // 65536U * 65536U should overflow
// CHECK-NEXT:     x = (1 as u32)
// CHECK-NEXT:         << (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64);
// CHECK-NEXT:     if !(mul(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const u32)) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(x) as *const u32)) },
// CHECK-NEXT:     ) != (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
