/* PR middle-end/87290 */

int c;

__attribute__((noipa)) void f0(void) { c++; }

__attribute__((noipa)) int f1(int x) { return x % 16 == 13; }

__attribute__((noipa)) int f2(int x) { return x % 16 == -13; }

__attribute__((noipa)) void f3(int x) {
  if (x % 16 == 13)
    f0();
}

__attribute__((noipa)) void f4(int x) {
  if (x % 16 == -13)
    f0();
}

int main() {
  int i, j;
  for (i = -30; i < 30; i++) {
    if (f1(13 + i * 16) != (i >= 0) || f2(-13 + i * 16) != (i <= 0))
      __builtin_abort();
    f3(13 + i * 16);
    if (c != (i >= 0))
      __builtin_abort();
    f4(-13 + i * 16);
    if (c != 1 + (i == 0))
      __builtin_abort();
    for (j = 1; j < 16; j++) {
      if (f1(13 + i * 16 + j) || f2(-13 + i * 16 + j))
        __builtin_abort();
      f3(13 + i * 16 + j);
      f4(-13 + i * 16 + j);
    }
    if (c != 1 + (i == 0))
      __builtin_abort();
    c = 0;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/87290
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn f0() {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut x: i32) -> i32 {
// CHECK-NEXT:     (x % (16 as i32) == (13 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut x: i32) -> i32 {
// CHECK-NEXT:     (x % (16 as i32) == -(13 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut x: i32) {
// CHECK-NEXT:     if x % (16 as i32) == (13 as i32) {
// CHECK-NEXT:         f0();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f4(mut x: i32) {
// CHECK-NEXT:     if x % (16 as i32) == -(13 as i32) {
// CHECK-NEXT:         f0();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = -(30 as i32);
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (30 as i32) {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:         if f1((13 as i32) + i * (16 as i32)) != ((i >= (0 as i32)) as i32) {
// CHECK-NEXT:             [[V_2]] = true as bool;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_2]] = f2(-(13 as i32) + i * (16 as i32)) != ((i <= (0 as i32)) as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         if [[V_2]] {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f3((13 as i32) + i * (16 as i32));
// CHECK-NEXT:         if (unsafe { c }) != ((i >= (0 as i32)) as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         f4(-(13 as i32) + i * (16 as i32));
// CHECK-NEXT:         if (unsafe { c }) != (1 as i32) + ((i == (0 as i32)) as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         j = 1 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (16 as i32) {
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:             if f1((13 as i32) + i * (16 as i32) + j) != (0 as i32) {
// CHECK-NEXT:                 [[V_3]] = true as bool;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 [[V_3]] = f2(-(13 as i32) + i * (16 as i32) + j) != (0 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             if [[V_3]] {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             f3((13 as i32) + i * (16 as i32) + j);
// CHECK-NEXT:             f4(-(13 as i32) + i * (16 as i32) + j);
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:             j = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { c }) != (1 as i32) + ((i == (0 as i32)) as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
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
