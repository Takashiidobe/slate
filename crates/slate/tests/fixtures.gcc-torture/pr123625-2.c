/* With -ftree-coalesce-vars, one variable can have SSA names coalesced into
   partitions of other variables: the PHIs below put the two names of b into
   the partitions of a and of c.  All three variables are oversized vectors
   with no register mode, so both partitions are spilled and b legitimately
   lives in two distinct stack slots (its DECL_RTL becomes the "multiple
   places" marker).  Out-of-SSA must keep the two slots distinguishable
   without rejecting this state.  */

typedef long __attribute__((vector_size(16 * sizeof(long)))) v16di;

v16di        g0 = {1}, g1 = {5}, g2 = {3}, g3 = {7};
volatile int p, q;

int main(void) {
  v16di b = g0;
  v16di a;
  if (p)
    a = b;
  else
    a = g1;
  g1 = a;
  b  = g2;
  v16di c;
  if (q)
    c = b;
  else
    c = g3;
  g3 = c;
  if (g1[0] != 5 || g1[1] != 0 || g3[0] != 7 || g3[1] != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! With -ftree-coalesce-vars, one variable can have SSA names coalesced into
// CHECK-NEXT: //! partitions of other variables: the PHIs below put the two names of b into
// CHECK-NEXT: //! the partitions of a and of c.  All three variables are oversized vectors
// CHECK-NEXT: //! with no register mode, so both partitions are spilled and b legitimately
// CHECK-NEXT: //! lives in two distinct stack slots (its DECL_RTL becomes the "multiple
// CHECK-NEXT: //! places" marker).  Out-of-SSA must keep the two slots distinguishable
// CHECK-NEXT: //! without rejecting this state.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g0: std::simd::Simd<i64, 16> = std::simd::Simd::from_array({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [i64; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (1 as i32) as i64;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut g1: std::simd::Simd<i64, 16> = std::simd::Simd::from_array({
// CHECK-NEXT:     let mut [[T_0]]: [i64; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (5 as i32) as i64;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut g2: std::simd::Simd<i64, 16> = std::simd::Simd::from_array({
// CHECK-NEXT:     let mut [[T_0]]: [i64; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (3 as i32) as i64;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut g3: std::simd::Simd<i64, 16> = std::simd::Simd::from_array({
// CHECK-NEXT:     let mut [[T_0]]: [i64; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = (7 as i32) as i64;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-EMPTY:
// CHECK-NEXT: static mut p: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut q: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: std::simd::Simd<i64, 16> = unsafe { g0 };
// CHECK-NEXT:     let mut a: std::simd::Simd<i64, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(p)) }) != (0 as i32) {
// CHECK-NEXT:         a = b;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         a = unsafe { g1 };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g1 = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     b = unsafe { g2 };
// CHECK-NEXT:     let mut c: std::simd::Simd<i64, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) }) != (0 as i32) {
// CHECK-NEXT:         c = b;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         c = unsafe { g3 };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g3 = c;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { g1[((0 as i32) as usize)] }) != ((5 as i32) as i64)
// CHECK-NEXT:         || (unsafe { g1[((1 as i32) as usize)] }) != ((0 as i32) as i64)
// CHECK-NEXT:         || (unsafe { g3[((0 as i32) as usize)] }) != ((7 as i32) as i64)
// CHECK-NEXT:         || (unsafe { g3[((1 as i32) as usize)] }) != ((0 as i32) as i64)
// CHECK-NEXT:     {
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
