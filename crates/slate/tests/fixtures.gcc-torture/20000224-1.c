void abort(void);
void exit(int);

int loop_1 = 100;
int loop_2 = 7;
int flag   = 0;

int test(void) {
  int i;
  int counter = 0;

  while (loop_1 > counter) {
    if (flag & 1) {
      for (i = 0; i < loop_2; i++) {
        counter++;
      }
    }
    flag++;
  }
  return 1;
}

int main() {
  if (test() != 1)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut loop_1: i32 = 100 as i32;
// CHECK-NEXT: static mut loop_2: i32 = 7 as i32;
// CHECK-NEXT: static mut flag: i32 = 0 as i32;
// CHECK-NEXT: fn test() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut counter: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { loop_1 }) > counter {
// CHECK-NEXT:         if (unsafe { flag }) & (1 as i32) != (0 as i32) {
// CHECK-NEXT:             i = 0 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < unsafe { loop_2 } {
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: i32 = counter;
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:                 counter = [[V_2]];
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:                 i = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = unsafe { flag };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             flag = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if test() != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
