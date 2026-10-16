/* PR target/92140 */

char c;
int  v;

__attribute__((noipa)) void         f1(void) { v += c != 0; }
__attribute__((noipa)) void         f2(void) { v -= c != 0; }
__attribute__((noipa)) void         f3(void) { v += c == 0; }
__attribute__((noipa)) void         f4(void) { v -= c == 0; }
__attribute__((noipa)) void         f5(void) { v += (c != 0) - 26; }
__attribute__((noipa)) void         f6(void) { v -= (c != 0) - 26; }
__attribute__((noipa)) void         f7(void) { v += (c == 0) - 26; }
__attribute__((noipa)) void         f8(void) { v -= (c == 0) - 26; }
__attribute__((noipa)) void         f9(void) { v += (c != 0) + 42; }
__attribute__((noipa)) void         f10(void) { v -= (c != 0) + 42; }
__attribute__((noipa)) void         f11(void) { v += (c == 0) + 42; }
__attribute__((noipa)) void         f12(void) { v -= (c == 0) + 42; }
__attribute__((noipa)) void         f13(int z) { v += (c == 0) + z; }
__attribute__((noipa)) void         f14(int z) { v -= (c == 0) + z; }
__attribute__((noipa)) unsigned int f15(unsigned int n) { return n ? 2 : 1; }

int main() {
  int i;
  for (i = 0; i < 2; i++) {
    v = 15;
    if (i == 1)
      c = 37;
    f1();
    if (v != 15 + i)
      __builtin_abort();
    f2();
    if (v != 15)
      __builtin_abort();
    f3();
    if (v != 16 - i)
      __builtin_abort();
    f4();
    if (v != 15)
      __builtin_abort();
    f5();
    if (v != 15 + i - 26)
      __builtin_abort();
    f6();
    if (v != 15)
      __builtin_abort();
    f7();
    if (v != 16 - i - 26)
      __builtin_abort();
    f8();
    if (v != 15)
      __builtin_abort();
    f9();
    if (v != 15 + i + 42)
      __builtin_abort();
    f10();
    if (v != 15)
      __builtin_abort();
    f11();
    if (v != 16 - i + 42)
      __builtin_abort();
    f12();
    if (v != 15)
      __builtin_abort();
    f13(173);
    if (v != 16 - i + 173)
      __builtin_abort();
    f14(173);
    if (v != 15)
      __builtin_abort();
    f13(-35);
    if (v != 16 - i - 35)
      __builtin_abort();
    f14(-35);
    if (v != 15)
      __builtin_abort();
  }
  if (f15(0) != 1 || f15(1) != 2 || f15(371) != 2)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/92140
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn f1() {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + ((((unsafe { c }) as i32) != (0 as i32)) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2() {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - ((((unsafe { c }) as i32) != (0 as i32)) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3() {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + ((((unsafe { c }) as i32) == (0 as i32)) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f4() {
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] - ((((unsafe { c }) as i32) == (0 as i32)) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f5() {
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (((((unsafe { c }) as i32) != (0 as i32)) as i32) - (26 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f6() {
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] - (((((unsafe { c }) as i32) != (0 as i32)) as i32) - (26 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f7() {
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: i32 = [[V_12]] + (((((unsafe { c }) as i32) == (0 as i32)) as i32) - (26 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_13]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f8() {
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] - (((((unsafe { c }) as i32) == (0 as i32)) as i32) - (26 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_15]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f9() {
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: i32 = [[V_16]] + (((((unsafe { c }) as i32) != (0 as i32)) as i32) + (42 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_17]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f10() {
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: i32 = [[V_18]] - (((((unsafe { c }) as i32) != (0 as i32)) as i32) + (42 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_19]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f11() {
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] + (((((unsafe { c }) as i32) == (0 as i32)) as i32) + (42 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_21]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f12() {
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]] - (((((unsafe { c }) as i32) == (0 as i32)) as i32) + (42 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_23]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f13(mut z: i32) {
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: i32 = [[V_24]] + (((((unsafe { c }) as i32) == (0 as i32)) as i32) + z);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_25]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f14(mut z: i32) {
// CHECK-NEXT:     let mut [[V_26:__v[0-9]+]]: i32 = unsafe { v };
// CHECK-NEXT:     let mut [[V_27:__v[0-9]+]]: i32 = [[V_26]] - (((((unsafe { c }) as i32) == (0 as i32)) as i32) + z);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = [[V_27]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f15(mut n: u32) -> u32 {
// CHECK-NEXT:     (if n != (0 as u32) { 2 as i32 } else { 1 as i32 }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             v = 15 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         if i == (1 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 c = (37 as i32) as i8;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         f1();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) + i {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f2();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f3();
// CHECK-NEXT:         if (unsafe { v }) != (16 as i32) - i {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f4();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f5();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) + i - (26 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f6();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f7();
// CHECK-NEXT:         if (unsafe { v }) != (16 as i32) - i - (26 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f8();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f9();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) + i + (42 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f10();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f11();
// CHECK-NEXT:         if (unsafe { v }) != (16 as i32) - i + (42 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f12();
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f13(173 as i32);
// CHECK-NEXT:         if (unsafe { v }) != (16 as i32) - i + (173 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f14(173 as i32);
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f13(-(35 as i32));
// CHECK-NEXT:         if (unsafe { v }) != (16 as i32) - i - (35 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f14(-(35 as i32));
// CHECK-NEXT:         if (unsafe { v }) != (15 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: i32 = [[V_28]] + (1 as i32);
// CHECK-NEXT:         i = [[V_29]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_30:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f15((0 as i32) as u32) != ((1 as i32) as u32) {
// CHECK-NEXT:         [[V_30]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_30]] = f15((1 as i32) as u32) != ((2 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_31:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_30]] {
// CHECK-NEXT:         [[V_31]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_31]] = f15((371 as i32) as u32) != ((2 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_31]] {
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
