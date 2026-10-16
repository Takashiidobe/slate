typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

u64 __attribute__((noinline, noclone)) foo(u8 u8_0, u16 u16_0, u64 u64_0,
                                           u8 u8_1, u16 u16_1, u64 u64_1,
                                           u64 u64_2, u8 u8_3, u64 u64_3) {
  u64_1 *= 0x7730;
  u64_3 *= u64_3;
  u16_1 |= u64_3;
  u64_3 -= 2;
  u8_3  /= u64_2;
  u8_0  |= 3;
  u64_3 %= u8_0;
  u8_0  -= 1;
  return u8_0 + u16_0 + u64_0 + u8_1 + u16_1 + u64_1 + u8_3 + u64_3;
}

int main() {
  unsigned x = foo(1, 1, 1, 1, 1, 1, 1, 1, 1);
  if (x != 0x7737)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(
// CHECK-NEXT:     mut u8_0: u8,
// CHECK-NEXT:     mut u16_0: u16,
// CHECK-NEXT:     mut u64_0: u64,
// CHECK-NEXT:     mut u8_1: u8,
// CHECK-NEXT:     mut u16_1: u16,
// CHECK-NEXT:     mut u64_1: u64,
// CHECK-NEXT:     mut u64_2: u64,
// CHECK-NEXT:     mut u8_3: u8,
// CHECK-NEXT:     mut u64_3: u64,
// CHECK-NEXT: ) -> u64 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = u64_1;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]].wrapping_mul(((30512 as i32) as i64) as u64);
// CHECK-NEXT:     u64_1 = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = u64_3;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]].wrapping_mul(u64_3);
// CHECK-NEXT:     u64_3 = [[V_3]];
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u16 = u16_1;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u16 = ((((([[V_4]] as u32) as i32) as i64) as u64) | u64_3) as u16;
// CHECK-NEXT:     u16_1 = [[V_5]];
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u64 = u64_3;
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u64 = [[V_6]].wrapping_sub(((2 as i32) as i64) as u64);
// CHECK-NEXT:     u64_3 = [[V_7]];
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u8 = u8_3;
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u8 = ((((([[V_8]] as u32) as i32) as i64) as u64) / u64_2) as u8;
// CHECK-NEXT:     u8_3 = [[V_9]];
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u8 = u8_0;
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u8 = (((([[V_10]] as u32) as i32) | (3 as i32)) as i8) as u8;
// CHECK-NEXT:     u8_0 = [[V_11]];
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: u64 = u64_3;
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: u64 = [[V_12]] % ((((u8_0 as u32) as i32) as i64) as u64);
// CHECK-NEXT:     u64_3 = [[V_13]];
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: u8 = u8_0;
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: u8 = (((([[V_14]] as u32) as i32) - (1 as i32)) as i8) as u8;
// CHECK-NEXT:     u8_0 = [[V_15]];
// CHECK-NEXT:     (((((u8_0 as u32) as i32) + ((u16_0 as u32) as i32)) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(u64_0)
// CHECK-NEXT:         .wrapping_add((((u8_1 as u32) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add((((u16_1 as u32) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(u64_1)
// CHECK-NEXT:         .wrapping_add((((u8_3 as u32) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(u64_3)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u32 = foo(
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i16) as u16,
// CHECK-NEXT:         ((1 as i32) as i64) as u64,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i16) as u16,
// CHECK-NEXT:         ((1 as i32) as i64) as u64,
// CHECK-NEXT:         ((1 as i32) as i64) as u64,
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((1 as i32) as i64) as u64,
// CHECK-NEXT:     ) as u32;
// CHECK-NEXT:     if x != ((30519 as i32) as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
