/* { dg-options { "-fwrapv" } } */

extern void abort(void);
extern void exit(int);

int dd(int x, int d) { return x / d; }

int main() {
  int i;
  for (i = -3; i <= 3; i++) {
    if (dd(i, 1) != i / 1)
      abort();
    if (dd(i, 2) != i / 2)
      abort();
    if (dd(i, 3) != i / 3)
      abort();
    if (dd(i, 4) != i / 4)
      abort();
    if (dd(i, 5) != i / 5)
      abort();
    if (dd(i, 6) != i / 6)
      abort();
    if (dd(i, 7) != i / 7)
      abort();
    if (dd(i, 8) != i / 8)
      abort();
  }
  for (i = ((unsigned)~0 >> 1) - 3; i <= ((unsigned)~0 >> 1) + 3; i++) {
    if (dd(i, 1) != i / 1)
      abort();
    if (dd(i, 2) != i / 2)
      abort();
    if (dd(i, 3) != i / 3)
      abort();
    if (dd(i, 4) != i / 4)
      abort();
    if (dd(i, 5) != i / 5)
      abort();
    if (dd(i, 6) != i / 6)
      abort();
    if (dd(i, 7) != i / 7)
      abort();
    if (dd(i, 8) != i / 8)
      abort();
  }
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options { "-fwrapv" } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn dd(mut x: i32, mut d: i32) -> i32 {
// CHECK-NEXT:     x / d
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = (3 as i32).wrapping_neg();
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (3 as i32) {
// CHECK-NEXT:         if dd(i, 1 as i32) != i / (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 2 as i32) != i / (2 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 3 as i32) != i / (3 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 4 as i32) != i / (4 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 5 as i32) != i / (5 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 6 as i32) != i / (6 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 7 as i32) != i / (7 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 8 as i32) != i / (8 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]].wrapping_add(1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = ((!(0 as i32) as u32) >> (1 as i32)).wrapping_sub((3 as i32) as u32) as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (i as u32)
// CHECK-NEXT:         <= ((!(0 as i32) as u32) >> (1 as i32)).wrapping_add((3 as i32) as u32)
// CHECK-NEXT:     {
// CHECK-NEXT:         if dd(i, 1 as i32) != i / (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 2 as i32) != i / (2 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 3 as i32) != i / (3 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 4 as i32) != i / (4 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 5 as i32) != i / (5 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 6 as i32) != i / (6 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 7 as i32) != i / (7 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if dd(i, 8 as i32) != i / (8 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]].wrapping_add(1 as i32);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
