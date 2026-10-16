/* PR tree-optimization/64006 */

int v;

long __attribute__((noinline, noclone)) test(long *x, int y) {
  int  i;
  long s = 1;
  for (i = 0; i < y; i++)
    if (__builtin_mul_overflow(s, x[i], &s))
      v++;
  return s;
}

int main() {
  long d[7] = {975, 975, 975, 975, 975, 975, 975};
  long r    = test(d, 7);
  if (sizeof(long) * __CHAR_BIT__ == 64 && v != 1)
    __builtin_abort();
  else if (sizeof(long) * __CHAR_BIT__ == 32 && v != 4)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/64006
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn test(mut x: *mut i64, mut y: i32) -> i64 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut s: i64 = (1 as i32) as i64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < y {
// CHECK-NEXT:         if {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: i128 = s as i128;
// CHECK-NEXT:             let [[T_1:__t[0-9]+]]: i128 = (unsafe { *unsafe { x.offset(i as isize) } }) as i128;
// CHECK-NEXT:             let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *std::ptr::addr_of_mut!(s) = [[T_2]] as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as i64) as i128) != [[T_2]]
// CHECK-NEXT:         } {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 v = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: __SlateAlign16<[i64; 7]> = __SlateAlign16([
// CHECK-NEXT:         (975 as i32) as i64,
// CHECK-NEXT:         (975 as i32) as i64,
// CHECK-NEXT:         (975 as i32) as i64,
// CHECK-NEXT:         (975 as i32) as i64,
// CHECK-NEXT:         (975 as i32) as i64,
// CHECK-NEXT:         (975 as i32) as i64,
// CHECK-NEXT:         (975 as i32) as i64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     test(d.0.as_mut_ptr() as *mut i64, 7 as i32);
// CHECK-NEXT:     if (8 as u64).wrapping_mul(((8 as i32) as i64) as u64) == (((64 as i32) as i64) as u64)
// CHECK-NEXT:         && (unsafe { v }) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if (8 as u64).wrapping_mul(((8 as i32) as i64) as u64) == (((32 as i32) as i64) as u64)
// CHECK-NEXT:             && (unsafe { v }) != (4 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
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
