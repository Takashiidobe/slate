/* PR tree-optimization/114396 */
/* { dg-additional-options "-fwrapv -fno-vect-cost-model" } */

short          a = 0xF;
short          b[16];
unsigned short ua = 0xF;
unsigned short ub[16];

short __attribute__((noipa)) foo(short a) {
  for (int e = 0; e < 9; e += 1)
    b[e] = a *= 5;
  return a;
}

short __attribute__((noipa)) foo1(short a) {
  for (int e = 0; e < 9; e += 1)
    b[e] = a *= -5;
  return a;
}

unsigned short __attribute__((noipa)) foou(unsigned short a) {
  for (int e = 0; e < 9; e += 1)
    ub[e] = a *= -5;
  return a;
}

unsigned short __attribute__((noipa)) foou1(unsigned short a) {
  for (int e = 0; e < 9; e += 1)
    ub[e] = a *= 5;
  return a;
}

short __attribute__((noipa, optimize("O3"))) foo_o3(short a) {
  for (int e = 0; e < 9; e += 1)
    b[e] = a *= 5;
  return a;
}

short __attribute__((noipa, optimize("O3"))) foo1_o3(short a) {
  for (int e = 0; e < 9; e += 1)
    b[e] = a *= -5;
  return a;
}

unsigned short __attribute__((noipa, optimize("O3")))
foou_o3(unsigned short a) {
  for (int e = 0; e < 9; e += 1)
    ub[e] = a *= -5;
  return a;
}

unsigned short __attribute__((noipa, optimize("O3")))
foou1_o3(unsigned short a) {
  for (int e = 0; e < 9; e += 1)
    ub[e] = a *= 5;
  return a;
}

int main() {
  unsigned short uexp, ures;
  short          exp, res;
  exp = foo(a);
  res = foo_o3(a);
  if (exp != res)
    __builtin_abort();

  exp = foo1(a);
  res = foo1_o3(a);
  if (exp != res)
    __builtin_abort();

  uexp = foou(a);
  ures = foou_o3(a);
  if (uexp != ures)
    __builtin_abort();

  uexp = foou1(a);
  ures = foou1_o3(a);
  if (uexp != ures)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/114396
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-additional-options "-fwrapv -fno-vect-cost-model" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i16 = (15 as i32) as i16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: __SlateAlign16<[i16; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut ua: u16 = ((15 as i32) as i16) as u16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ub: __SlateAlign16<[u16; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut [[A_0:a_[0-9]+]]: i16) -> i16 {
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while e < (9 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i16 = [[A_0]];
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i16 = ([[V_0]] as i32).wrapping_mul(5 as i32) as i16;
// CHECK-NEXT:         [[A_0]] = [[V_1]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b.0) as *mut i16 }.offset(e as isize) } =
// CHECK-NEXT:                 [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]].wrapping_add(1 as i32);
// CHECK-NEXT:         e = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[A_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo1(mut [[A_1:a_[0-9]+]]: i16) -> i16 {
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while e < (9 as i32) {
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i16 = [[A_1]];
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i16 = ([[V_4]] as i32).wrapping_mul((5 as i32).wrapping_neg()) as i16;
// CHECK-NEXT:         [[A_1]] = [[V_5]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b.0) as *mut i16 }.offset(e as isize) } =
// CHECK-NEXT:                 [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]].wrapping_add(1 as i32);
// CHECK-NEXT:         e = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[A_1]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foou(mut [[A_2:a_[0-9]+]]: u16) -> u16 {
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while e < (9 as i32) {
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: u16 = [[A_2]];
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: u16 =
// CHECK-NEXT:             ((([[V_8]] as u32) as i32).wrapping_mul((5 as i32).wrapping_neg()) as i16) as u16;
// CHECK-NEXT:         [[A_2]] = [[V_9]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(ub.0) as *mut u16 }.offset(e as isize) } =
// CHECK-NEXT:                 [[V_9]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]].wrapping_add(1 as i32);
// CHECK-NEXT:         e = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[A_2]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foou1(mut [[A_3:a_[0-9]+]]: u16) -> u16 {
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while e < (9 as i32) {
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: u16 = [[A_3]];
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: u16 = ((([[V_12]] as u32) as i32).wrapping_mul(5 as i32) as i16) as u16;
// CHECK-NEXT:         [[A_3]] = [[V_13]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(ub.0) as *mut u16 }.offset(e as isize) } =
// CHECK-NEXT:                 [[V_13]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]].wrapping_add(1 as i32);
// CHECK-NEXT:         e = [[V_15]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[A_3]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo_o3(mut [[A_4:a_[0-9]+]]: i16) -> i16 {
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while e < (9 as i32) {
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: i16 = [[A_4]];
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: i16 = ([[V_16]] as i32).wrapping_mul(5 as i32) as i16;
// CHECK-NEXT:         [[A_4]] = [[V_17]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b.0) as *mut i16 }.offset(e as isize) } =
// CHECK-NEXT:                 [[V_17]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: i32 = [[V_18]].wrapping_add(1 as i32);
// CHECK-NEXT:         e = [[V_19]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[A_4]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo1_o3(mut [[A_5:a_[0-9]+]]: i16) -> i16 {
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while e < (9 as i32) {
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: i16 = [[A_5]];
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: i16 = ([[V_20]] as i32).wrapping_mul((5 as i32).wrapping_neg()) as i16;
// CHECK-NEXT:         [[A_5]] = [[V_21]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b.0) as *mut i16 }.offset(e as isize) } =
// CHECK-NEXT:                 [[V_21]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]].wrapping_add(1 as i32);
// CHECK-NEXT:         e = [[V_23]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[A_5]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foou_o3(mut [[A_6:a_[0-9]+]]: u16) -> u16 {
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while e < (9 as i32) {
// CHECK-NEXT:         let mut [[V_24:__v[0-9]+]]: u16 = [[A_6]];
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: u16 =
// CHECK-NEXT:             ((([[V_24]] as u32) as i32).wrapping_mul((5 as i32).wrapping_neg()) as i16) as u16;
// CHECK-NEXT:         [[A_6]] = [[V_25]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(ub.0) as *mut u16 }.offset(e as isize) } =
// CHECK-NEXT:                 [[V_25]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_26:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: i32 = [[V_26]].wrapping_add(1 as i32);
// CHECK-NEXT:         e = [[V_27]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[A_6]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foou1_o3(mut [[A_7:a_[0-9]+]]: u16) -> u16 {
// CHECK-NEXT:     let mut e: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while e < (9 as i32) {
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: u16 = [[A_7]];
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: u16 = ((([[V_28]] as u32) as i32).wrapping_mul(5 as i32) as i16) as u16;
// CHECK-NEXT:         [[A_7]] = [[V_29]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(ub.0) as *mut u16 }.offset(e as isize) } =
// CHECK-NEXT:                 [[V_29]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_30:__v[0-9]+]]: i32 = e;
// CHECK-NEXT:         let mut [[V_31:__v[0-9]+]]: i32 = [[V_30]].wrapping_add(1 as i32);
// CHECK-NEXT:         e = [[V_31]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[A_7]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut uexp: u16 = 0 as u16;
// CHECK-NEXT:     let mut ures: u16 = 0 as u16;
// CHECK-NEXT:     let mut exp: i16 = foo(unsafe { a });
// CHECK-NEXT:     let mut res: i16 = foo_o3(unsafe { a });
// CHECK-NEXT:     if (exp as i32) != (res as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     exp = foo1(unsafe { a });
// CHECK-NEXT:     res = foo1_o3(unsafe { a });
// CHECK-NEXT:     if (exp as i32) != (res as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     uexp = foou((unsafe { a }) as u16);
// CHECK-NEXT:     ures = foou_o3((unsafe { a }) as u16);
// CHECK-NEXT:     if ((uexp as u32) as i32) != ((ures as u32) as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     uexp = foou1((unsafe { a }) as u16);
// CHECK-NEXT:     ures = foou1_o3((unsafe { a }) as u16);
// CHECK-NEXT:     if ((uexp as u32) as i32) != ((ures as u32) as i32) {
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
