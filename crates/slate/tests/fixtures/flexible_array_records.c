#include <stddef.h>
#include <stdlib.h>

struct Entry { long value; int tag; };
struct List { int count; struct Entry entries[]; };
struct Bytes { double head; int tag; unsigned char bytes[]; };
struct Wide { char tag; long double values[]; };

int main(void) {
  struct List header = {2};
  struct List copied = header;
  if (copied.count != 2) return 7;
  struct List *list = malloc(sizeof(*list) + 5 * sizeof(struct Entry));
  struct Bytes *bytes = malloc(sizeof(*bytes) + 9);
  struct Wide *wide = malloc(sizeof(*wide) + 3 * sizeof(long double));
  if (!list || !bytes || !wide) return 1;
  if ((char *)list->entries - (char *)list != offsetof(struct List, entries)) return 2;
  if ((char *)bytes->bytes - (char *)bytes != offsetof(struct Bytes, bytes)) return 3;
  if ((char *)wide->values - (char *)wide != offsetof(struct Wide, values)) return 4;
  if ((char *)(bytes + 1) - (char *)bytes != sizeof(*bytes)) return 5;
  if ((char *)(wide + 1) - (char *)wide != sizeof(*wide)) return 6;
  list->count = 5;
  bytes->head = 2.5;
  bytes->tag = 17;
  wide->tag = 4;
  for (int i = 0; i < list->count; ++i) {
    list->entries[i].value = i * 13;
    list->entries[i].tag = i + 7;
  }
  unsigned char (*array)[] = &bytes->bytes;
  for (int i = 0; i < 9; ++i) (*array)[i] = i * 3;
  for (int i = 0; i < 3; ++i) wide->values[i] = i + 0.5L;
  const struct List *view = list;
  int result = view->entries[4].value != 52 || view->entries[3].tag != 10
    || bytes->bytes[8] != 24 || bytes->head != 2.5 || bytes->tag != 17
    || wide->values[2] != 2.5L || wide->tag != 4;
  free(wide);
  free(bytes);
  free(list);
  return result;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Entry {
// CHECK-NEXT:     value: i64,
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct List {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     entries: [Entry; 0],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Bytes {
// CHECK-NEXT:     head: f64,
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT:     bytes: [u8; 0],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Wide {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     values: [LongDouble; 0],
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let header: List = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: List = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].count = 2i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let copied: List = header;
// CHECK-NEXT:     if copied.count != 2 {
// CHECK-NEXT:         return 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     let list: *mut List = (unsafe { malloc(88u64) }) as *mut List;
// CHECK-NEXT:     let bytes: *mut Bytes = (unsafe { malloc(25u64) }) as *mut Bytes;
// CHECK-NEXT:     let wide: *mut Wide = (unsafe { malloc(64u64) }) as *mut Wide;
// CHECK-NEXT:     if list.is_null() || bytes.is_null() || wide.is_null() {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         ((std::ptr::addr_of_mut!((* list).entries) as *mut Entry) as *mut i8)
// CHECK-NEXT:             .offset_from(list as *mut i8)
// CHECK-NEXT:     }) as i64) as u64) != 8
// CHECK-NEXT:     {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         ((std::ptr::addr_of_mut!((* bytes).bytes) as *mut u8) as *mut i8)
// CHECK-NEXT:             .offset_from(bytes as *mut i8)
// CHECK-NEXT:     }) as i64) as u64) != 12
// CHECK-NEXT:     {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         ((std::ptr::addr_of_mut!((* wide).values) as *mut LongDouble) as *mut i8)
// CHECK-NEXT:             .offset_from(wide as *mut i8)
// CHECK-NEXT:     }) as i64) as u64) != 16
// CHECK-NEXT:     {
// CHECK-NEXT:         return 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { (bytes.add(1) as *mut i8).offset_from(bytes as *mut i8) }) as i64)
// CHECK-NEXT:         as u64) != 16
// CHECK-NEXT:     {
// CHECK-NEXT:         return 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { (wide.add(1) as *mut i8).offset_from(wide as *mut i8) }) as i64)
// CHECK-NEXT:         as u64) != 16
// CHECK-NEXT:     {
// CHECK-NEXT:         return 6;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*list).count = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*bytes).head = 2.5f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*bytes).tag = 17;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*wide).tag = 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         while i < unsafe { (*list).count } {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(std::ptr::addr_of_mut!((* list).entries) as *mut Entry)
// CHECK-NEXT:                     .offset(i as isize))
// CHECK-NEXT:                     .value = (i * 13) as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*(std::ptr::addr_of_mut!((* list).entries) as *mut Entry)
// CHECK-NEXT:                     .offset(i as isize))
// CHECK-NEXT:                     .tag = i + 7;
// CHECK-NEXT:             }
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let array: *mut [u8; 0] = unsafe { std::ptr::addr_of_mut!((* bytes).bytes) };
// CHECK-NEXT:     for i in 0i32..9i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(std::ptr::addr_of_mut!(* array) as *mut u8).offset(i as isize) = ((i * 3)
// CHECK-NEXT:                 as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(std::ptr::addr_of_mut!((* wide).values) as *mut LongDouble)
// CHECK-NEXT:                 .offset(i as isize) = __slate_f80_from_i32(i)
// CHECK-NEXT:                 + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let result: i32 = ((unsafe {
// CHECK-NEXT:         (*(std::ptr::addr_of!((* (list as * const List)).entries) as *const Entry)
// CHECK-NEXT:             .add(4))
// CHECK-NEXT:             .value
// CHECK-NEXT:     }) != 52
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             (*(std::ptr::addr_of!((* (list as * const List)).entries) as *const Entry)
// CHECK-NEXT:                 .add(3))
// CHECK-NEXT:                 .tag
// CHECK-NEXT:         }) != 10
// CHECK-NEXT:         || (((unsafe { *(std::ptr::addr_of_mut!((* bytes).bytes) as *mut u8).add(8) })
// CHECK-NEXT:             as u32) as i32) != 24 || (unsafe { (*bytes).head }) != 2.5f64
// CHECK-NEXT:         || (unsafe { (*bytes).tag }) != 17
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *(std::ptr::addr_of_mut!((* wide).values) as *mut LongDouble).add(2)
// CHECK-NEXT:         }) != LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64])
// CHECK-NEXT:         || ((unsafe { (*wide).tag }) as i32) != 4) as i32;
// CHECK-NEXT:     unsafe { free(wide as *mut ()) };
// CHECK-NEXT:     unsafe { free(bytes as *mut ()) };
// CHECK-NEXT:     unsafe { free(list as *mut ()) };
// CHECK-NEXT:     result
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
