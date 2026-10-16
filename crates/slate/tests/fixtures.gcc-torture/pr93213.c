/* PR tree-optimization/93213 - wrong code on a multibyte store with
   -Og -foptimize-strlen
   { dg-require-effective-target int128 }
   { dg-additional-options "-Og -foptimize-strlen" } */

typedef unsigned __INT16_TYPE__ u16;
typedef unsigned __INT32_TYPE__ u32;
typedef unsigned __int128       u128;

static inline u128 foo(u16 u16_1, u32 u32_1, u128 u128_1) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  u128 u128_0  = 0;
  u128_1      -= __builtin_mul_overflow(u32_1, u16_1, &u32_1);
  __builtin_memmove(&u16_1, &u128_0, 2);
  __builtin_memmove(&u16_1, &u128_1, 1);
  return u16_1;
#else
  return 0xff;
#endif
}

__attribute__((noipa)) void bar(void) {
  char       a[] = {1, 2};
  const char b[] = {0, 0};
  const char c[] = {2};
  __builtin_memcpy(a, b, 2);
  // The above is transformed into
  //   MEM <short unsigned int> [(char * {ref-all})&a] = 0;
  // which was then dropped because of the non-nul store below.
  __builtin_memcpy(a, c, 1);

  volatile char *p = a;
  if (p[0] != 2 || p[1] != 0)
    __builtin_abort();
}

int main(void) {
  u16 x = foo(-1, -1, 0);
  if (x != 0xff)
    __builtin_abort();

  bar();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/93213 - wrong code on a multibyte store with
// CHECK-NEXT: //! -Og -foptimize-strlen
// CHECK-NEXT: //! { dg-require-effective-target int128 }
// CHECK-NEXT: //! { dg-additional-options "-Og -foptimize-strlen" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memmove"]
// CHECK-NEXT:     fn __builtin_memmove([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: *const (), [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut u16_1: u16, mut u32_1: u32, mut u128_1: u128) -> u128 {
// CHECK-NEXT:     let mut u128_0: u128 = ((0 as i32) as i128) as u128;
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u128 = u128_1;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u128 = [[V_6]].wrapping_sub(
// CHECK-NEXT:         ((({
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: i128 = u32_1 as i128;
// CHECK-NEXT:             let [[T_1:__t[0-9]+]]: i128 = u16_1 as i128;
// CHECK-NEXT:             let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *std::ptr::addr_of_mut!(u32_1) = [[T_2]] as u32;
// CHECK-NEXT:             }
// CHECK-NEXT:             [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u32) as i128) != [[T_2]]
// CHECK-NEXT:         }) as i32) as i128) as u128,
// CHECK-NEXT:     );
// CHECK-NEXT:     u128_1 = [[V_7]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memmove(
// CHECK-NEXT:             std::ptr::addr_of_mut!(u16_1) as *mut (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(u128_0) as *const (),
// CHECK-NEXT:             ((2 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memmove(
// CHECK-NEXT:             std::ptr::addr_of_mut!(u16_1) as *mut (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(u128_1) as *const (),
// CHECK-NEXT:             ((1 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     u16_1 as u128
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() {
// CHECK-NEXT:     let mut a: [i8; 2] = [(1 as i32) as i8, (2 as i32) as i8];
// CHECK-NEXT:     let mut b: [i8; 2] = [(0 as i32) as i8, (0 as i32) as i8];
// CHECK-NEXT:     let mut c: [i8; 1] = [(2 as i32) as i8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             a.as_mut_ptr() as *mut (),
// CHECK-NEXT:             b.as_ptr() as *const (),
// CHECK-NEXT:             ((2 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     // The above is transformed into
// CHECK-NEXT:     //
// CHECK-NEXT:     //   MEM <short unsigned int> [(char * {ref-all})&a] = 0;
// CHECK-NEXT:     //
// CHECK-NEXT:     // which was then dropped because of the non-nul store below.
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             a.as_mut_ptr() as *mut (),
// CHECK-NEXT:             c.as_ptr() as *const (),
// CHECK-NEXT:             ((1 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut p: *mut i8 = a.as_mut_ptr() as *mut i8;
// CHECK-NEXT:     if ((unsafe { std::ptr::read_volatile(unsafe { p.offset((0 as i32) as isize) }) }) as i32)
// CHECK-NEXT:         != (2 as i32)
// CHECK-NEXT:         || ((unsafe { std::ptr::read_volatile(unsafe { p.offset((1 as i32) as isize) }) }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u16 = foo(
// CHECK-NEXT:         (-(1 as i32) as i16) as u16,
// CHECK-NEXT:         -(1 as i32) as u32,
// CHECK-NEXT:         ((0 as i32) as i128) as u128,
// CHECK-NEXT:     ) as u16;
// CHECK-NEXT:     if ((x as u32) as i32) != (255 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     bar();
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
