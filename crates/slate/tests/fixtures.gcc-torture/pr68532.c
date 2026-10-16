/* { dg-options "-O2 -ftree-vectorize -fno-vect-cost-model" } */
/* { dg-additional-options "-fno-common" { target hppa*-*-hpux* } } */

#define SIZE 128
unsigned short _Alignas(16) in[SIZE];

__attribute__((noinline)) int test(unsigned short sum, unsigned short *in,
                                   int x) {
  for (int j = 0; j < SIZE; j += 8)
    sum += in[j] * x;
  return sum;
}

int main() {
  for (int i = 0; i < SIZE; i++)
    in[i] = i;
  if (test(0, in, 1) != 960)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-O2 -ftree-vectorize -fno-vect-cost-model" }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-additional-options "-fno-common" { target hppa*-*-hpux* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut r#in: __SlateAlign16<[u16; 128]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn test(mut sum: u16, mut [[IN_0:in_[0-9]+]]: *mut u16, mut x: i32) -> i32 {
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (128 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u16 = sum;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u16 = (((([[V_0]] as u32) as i32)
// CHECK-NEXT:             + (((unsafe { *unsafe { [[IN_0]].offset(j as isize) } }) as u32) as i32) * x)
// CHECK-NEXT:             as i16) as u16;
// CHECK-NEXT:         sum = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (8 as i32);
// CHECK-NEXT:         j = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     (sum as u32) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (128 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(r#in.0) as *mut u16 }.offset(i as isize) } =
// CHECK-NEXT:                 (i as i16) as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if test(
// CHECK-NEXT:         ((0 as i32) as i16) as u16,
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(r#in.0) as *mut u16 },
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     ) != (960 as i32)
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
