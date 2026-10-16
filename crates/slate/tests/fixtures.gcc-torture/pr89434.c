/* PR target/89434 */

#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 && __CHAR_BIT__ == 8
long g = 0;

static inline unsigned long long foo(unsigned long long u) {
  unsigned x;
  __builtin_mul_overflow(-1, g, &x);
  u |= (unsigned)u < (unsigned short)x;
  return x - u;
}

int main() {
  unsigned long long x = foo(0x222222222ULL);
  if (x != 0xfffffffddddddddeULL)
    __builtin_abort();
  return 0;
}
#else
int main() { return 0; }
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/89434
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i64 = (0 as i32) as i64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut u: u64) -> u64 {
// CHECK-NEXT:     let mut x: u32 = 0 as u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = -(1 as i32) as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = (unsafe { g }) as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(x) = [[T_2]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as u32) as i128) != [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = u;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 =
// CHECK-NEXT:         [[V_0]] | (((((u as u32) < ((((x as u16) as u32) as i32) as u32)) as i32) as i64) as u64);
// CHECK-NEXT:     u = [[V_1]];
// CHECK-NEXT:     (x as u64).wrapping_sub(u)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u64 = foo(9162596898 as u64);
// CHECK-NEXT:     if x != (18446744064546954718 as u64) {
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
