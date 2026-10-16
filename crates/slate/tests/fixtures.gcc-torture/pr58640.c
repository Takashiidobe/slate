void exit(int);

int a, b, c, d = 1, e;

static signed char foo() {
  int f, g = a;

  for (f = 1; f < 3; f++)
    for (; b < 1; b++) {
      if (d)
        for (c = 0; c < 4; c++)
          for (f = 0; f < 3; f++) {
            for (e = 0; e < 1; e++)
              a = g;
            if (f)
              break;
          }
      else if (f)
        continue;
      return 0;
    }
  return 0;
}

int main() {
  foo();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = 1 as i32;
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo() -> i8 {
// CHECK-NEXT:     let mut f: i32 = 1 as i32;
// CHECK-NEXT:     let mut g: i32 = unsafe { a };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while f < (3 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { b }) < (1 as i32) {
// CHECK-NEXT:             '[[SLATE_CONTINUE_0:__slate_continue_[0-9]+]]: {
// CHECK-NEXT:                 if (unsafe { d }) != (0 as i32) {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         c = 0 as i32;
// CHECK-NEXT:                     }
// CHECK-NEXT:                     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { c }) < (4 as i32) {
// CHECK-NEXT:                         f = 0 as i32;
// CHECK-NEXT:                         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while f < (3 as i32) {
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 e = 0 as i32;
// CHECK-NEXT:                             }
// CHECK-NEXT:                             '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while (unsafe { e }) < (1 as i32) {
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     a = g;
// CHECK-NEXT:                                 }
// CHECK-NEXT:                                 let mut [[V_1:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:                                 let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     e = [[V_2]];
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                             if f != (0 as i32) {
// CHECK-NEXT:                                 break '[[SLATE_BREAK_3]];
// CHECK-NEXT:                             }
// CHECK-NEXT:                             let mut [[V_3:__v[0-9]+]]: i32 = f;
// CHECK-NEXT:                             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:                             f = [[V_4]];
// CHECK-NEXT:                         }
// CHECK-NEXT:                         let mut [[V_5:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:                         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             c = [[V_6]];
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if f != (0 as i32) {
// CHECK-NEXT:                         break '[[SLATE_CONTINUE_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 return (0 as i32) as i8;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 b = [[V_8]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = f;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:         f = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT:     (0 as i32) as i8
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
