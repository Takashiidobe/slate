/* PR tree-optimization/48809 */

extern void abort(void);

int foo(signed char x) {
  int y = 0;
  switch (x) {
  case 0:
    y = 1;
    break;
  case 1:
    y = 7;
    break;
  case 2:
    y = 2;
    break;
  case 3:
    y = 19;
    break;
  case 4:
    y = 5;
    break;
  case 5:
    y = 17;
    break;
  case 6:
    y = 31;
    break;
  case 7:
    y = 8;
    break;
  case 8:
    y = 28;
    break;
  case 9:
    y = 16;
    break;
  case 10:
    y = 31;
    break;
  case 11:
    y = 12;
    break;
  case 12:
    y = 15;
    break;
  case 13:
    y = 111;
    break;
  case 14:
    y = 17;
    break;
  case 15:
    y = 10;
    break;
  case 16:
    y = 31;
    break;
  case 17:
    y = 7;
    break;
  case 18:
    y = 2;
    break;
  case 19:
    y = 19;
    break;
  case 20:
    y = 5;
    break;
  case 21:
    y = 107;
    break;
  case 22:
    y = 31;
    break;
  case 23:
    y = 8;
    break;
  case 24:
    y = 28;
    break;
  case 25:
    y = 106;
    break;
  case 26:
    y = 31;
    break;
  case 27:
    y = 102;
    break;
  case 28:
    y = 105;
    break;
  case 29:
    y = 111;
    break;
  case 30:
    y = 17;
    break;
  case 31:
    y = 10;
    break;
  case 32:
    y = 31;
    break;
  case 98:
    y = 18;
    break;
  case -62:
    y = 19;
    break;
  }
  return y;
}

int main() {
  if (foo(98) != 18 || foo(97) != 0 || foo(99) != 0)
    abort();
  if (foo(-62) != 19 || foo(-63) != 0 || foo(-61) != 0)
    abort();
  if (foo(28) != 105 || foo(27) != 102 || foo(29) != 111)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/48809
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i8) -> i32 {
// CHECK-NEXT:     let mut y: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match x as i32 {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 y = 1 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 y = 7 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 y = 2 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 y = 19 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 y = 5 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             5 => {
// CHECK-NEXT:                 y = 17 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             6 => {
// CHECK-NEXT:                 y = 31 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             7 => {
// CHECK-NEXT:                 y = 8 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 y = 28 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             9 => {
// CHECK-NEXT:                 y = 16 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             10 => {
// CHECK-NEXT:                 y = 31 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             11 => {
// CHECK-NEXT:                 y = 12 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             12 => {
// CHECK-NEXT:                 y = 15 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             13 => {
// CHECK-NEXT:                 y = 111 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             14 => {
// CHECK-NEXT:                 y = 17 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             15 => {
// CHECK-NEXT:                 y = 10 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             16 => {
// CHECK-NEXT:                 y = 31 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             17 => {
// CHECK-NEXT:                 y = 7 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             18 => {
// CHECK-NEXT:                 y = 2 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             19 => {
// CHECK-NEXT:                 y = 19 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             20 => {
// CHECK-NEXT:                 y = 5 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             21 => {
// CHECK-NEXT:                 y = 107 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             22 => {
// CHECK-NEXT:                 y = 31 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             23 => {
// CHECK-NEXT:                 y = 8 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             24 => {
// CHECK-NEXT:                 y = 28 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             25 => {
// CHECK-NEXT:                 y = 106 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             26 => {
// CHECK-NEXT:                 y = 31 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             27 => {
// CHECK-NEXT:                 y = 102 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             28 => {
// CHECK-NEXT:                 y = 105 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             29 => {
// CHECK-NEXT:                 y = 111 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             30 => {
// CHECK-NEXT:                 y = 17 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             31 => {
// CHECK-NEXT:                 y = 10 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             32 => {
// CHECK-NEXT:                 y = 31 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             98 => {
// CHECK-NEXT:                 y = 18 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             -62 => {
// CHECK-NEXT:                 y = 19 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     y
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo((98 as i32) as i8) != (18 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = foo((97 as i32) as i8) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = foo((99 as i32) as i8) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(-(62 as i32) as i8) != (19 as i32) {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = foo(-(63 as i32) as i8) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = foo(-(61 as i32) as i8) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo((28 as i32) as i8) != (105 as i32) {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = foo((27 as i32) as i8) != (102 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = foo((29 as i32) as i8) != (111 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         unsafe { abort() };
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
