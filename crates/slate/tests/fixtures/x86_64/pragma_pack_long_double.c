int printf(const char *, ...);

#pragma pack(8)
struct extended {
  char c;
  long double x;
  int tail;
};
#pragma pack()

static unsigned offset(const void *base, const void *field) {
  return (unsigned)((const char *)field - (const char *)base);
}

int main(void) {
  struct extended e = {1, 2.5L, 3};
  struct extended list[2] = {{4, 5.5L, 6}, {7, 8.5L, 9}};
  e.x *= 3.0L;
  printf("%zu %zu %u %u\n", sizeof e, _Alignof(struct extended), offset(&e, &e.x),
         offset(&e, &e.tail));
  printf("%u %u\n", offset(&list[0], &list[1].x), offset(&list[0], &list[1].tail));
  printf("%d %g %d\n", e.c, (double)e.x, e.tail);
  for (int i = 0; i < 2; i++)
    printf("%d %g %d\n", list[i].c, (double)list[i].x, list[i].tail);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(
// CHECK-NEXT:         __a: LongDouble,
// CHECK-NEXT:         __b: LongDouble,
// CHECK-NEXT:         __c: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct extended {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     x: std::mem::MaybeUninit<[u64; 2]>,
// CHECK-NEXT:     tail: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn offset(base: *const (), field: *const ()) -> u32 {
// CHECK-NEXT:     (((unsafe { (field as *const i8).offset_from(base as *const i8) }) as i64) as i32)
// CHECK-NEXT:         as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: extended = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: extended = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].c = 1i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (std::ptr::addr_of_mut!([[T_0]].x) as *mut LongDouble)
// CHECK-NEXT:                 .write_unaligned(LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]));
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].tail = 3i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut list: __SlateAlign16<[extended; 2]> = __SlateAlign16([
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: extended = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].c = 4i8;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (std::ptr::addr_of_mut!([[T_1]].x) as *mut LongDouble)
// CHECK-NEXT:                     .write_unaligned(LongDouble([0, 0, 0, 0, 0, 0, 0, 176, 1, 64]));
// CHECK-NEXT:             }
// CHECK-NEXT:             [[T_1]].tail = 6i32;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: extended = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_2]].c = 7i8;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (std::ptr::addr_of_mut!([[T_2]].x) as *mut LongDouble)
// CHECK-NEXT:                     .write_unaligned(LongDouble([0, 0, 0, 0, 0, 0, 0, 136, 2, 64]));
// CHECK-NEXT:             }
// CHECK-NEXT:             [[T_2]].tail = 9i32;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: LongDouble = unsafe {
// CHECK-NEXT:         (std::ptr::addr_of!(e.x) as *const LongDouble).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(e.x) as *mut LongDouble)
// CHECK-NEXT:             .write_unaligned([[V_1]] * LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %u %u\n".as_ptr(),
// CHECK-NEXT:             32u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(e) as *const (),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(e.x) as *mut LongDouble) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(e) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(e.tail) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u\n".as_ptr(),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 (list.0.as_mut_ptr() as *mut extended).add(0) as *const (),
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(list.0[1].x) as *mut LongDouble) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:             offset(
// CHECK-NEXT:                 (list.0.as_mut_ptr() as *mut extended).add(0) as *const (),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(list.0[1].tail) as *const (),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %g %d\n".as_ptr(),
// CHECK-NEXT:             e.c as i32,
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 (std::ptr::addr_of!(e.x) as *const LongDouble).read_unaligned(),
// CHECK-NEXT:             ),
// CHECK-NEXT:             e.tail,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..2i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %g %d\n".as_ptr(),
// CHECK-NEXT:                 list.0[i as usize].c as i32,
// CHECK-NEXT:                 __slate_f80_to_f64(
// CHECK-NEXT:                     (std::ptr::addr_of!(list.0[i as usize].x) as *const LongDouble)
// CHECK-NEXT:                         .read_unaligned(),
// CHECK-NEXT:                 ),
// CHECK-NEXT:                 list.0[i as usize].tail,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
