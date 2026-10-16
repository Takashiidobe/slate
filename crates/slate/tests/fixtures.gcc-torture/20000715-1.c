void abort(void);
void exit(int);

void test1(void) {
  int x = 3, y = 2;

  if ((x < y ? x++ : y++) != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

void test2(void) {
  int x = 3, y = 2, z;

  z = (x < y) ? x++ : y++;
  if (z != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

void test3(void) {
  int x = 3, y = 2;
  int xx = 3, yy = 2;

  if ((xx < yy ? x++ : y++) != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

int x, y;

static void init_xy(void) {
  x = 3;
  y = 2;
}

void test4(void) {
  init_xy();
  if ((x < y ? x++ : y++) != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

void test5(void) {
  int z;

  init_xy();
  z = (x < y) ? x++ : y++;
  if (z != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

void test6(void) {
  int xx = 3, yy = 2;
  int z;

  init_xy();
  z = (xx < y) ? x++ : y++;
  if (z != 2)
    abort();

  if (x != 3)
    abort();

  if (y != 3)
    abort();
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn test1() {
// CHECK-NEXT:     let mut [[X_0:x_[0-9]+]]: i32 = 3 as i32;
// CHECK-NEXT:     let mut [[Y_0:y_[0-9]+]]: i32 = 2 as i32;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32;
// CHECK-NEXT:     if [[X_0]] < [[Y_0]] {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[X_0]];
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         [[X_0]] = [[V_3]];
// CHECK-NEXT:         [[V_1]] = [[V_2]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[Y_0]];
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         [[Y_0]] = [[V_5]];
// CHECK-NEXT:         [[V_1]] = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[X_0]] != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[Y_0]] != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test2() {
// CHECK-NEXT:     let mut [[X_1:x_[0-9]+]]: i32 = 3 as i32;
// CHECK-NEXT:     let mut [[Y_1:y_[0-9]+]]: i32 = 2 as i32;
// CHECK-NEXT:     let mut z: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i32;
// CHECK-NEXT:     if [[X_1]] < [[Y_1]] {
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[X_1]];
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         [[X_1]] = [[V_8]];
// CHECK-NEXT:         [[V_6]] = [[V_7]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[Y_1]];
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:         [[Y_1]] = [[V_10]];
// CHECK-NEXT:         [[V_6]] = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     z = [[V_6]];
// CHECK-NEXT:     if z != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[X_1]] != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[Y_1]] != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test3() {
// CHECK-NEXT:     let mut [[X_2:x_[0-9]+]]: i32 = 3 as i32;
// CHECK-NEXT:     let mut [[Y_2:y_[0-9]+]]: i32 = 2 as i32;
// CHECK-NEXT:     let mut xx: i32 = 3 as i32;
// CHECK-NEXT:     let mut yy: i32 = 2 as i32;
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: i32;
// CHECK-NEXT:     if xx < yy {
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = [[X_2]];
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: i32 = [[V_12]] + (1 as i32);
// CHECK-NEXT:         [[X_2]] = [[V_13]];
// CHECK-NEXT:         [[V_11]] = [[V_12]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: i32 = [[Y_2]];
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] + (1 as i32);
// CHECK-NEXT:         [[Y_2]] = [[V_15]];
// CHECK-NEXT:         [[V_11]] = [[V_14]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_11]] != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[X_2]] != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[Y_2]] != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut y: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn init_xy() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         y = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test4() {
// CHECK-NEXT:     init_xy();
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: i32;
// CHECK-NEXT:     if (unsafe { x }) < unsafe { y } {
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: i32 = [[V_17]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             x = [[V_18]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_16]] = [[V_17]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: i32 = unsafe { y };
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: i32 = [[V_19]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             y = [[V_20]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_16]] = [[V_19]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_16]] != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { y }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test5() {
// CHECK-NEXT:     let mut z: i32 = 0 as i32;
// CHECK-NEXT:     init_xy();
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: i32;
// CHECK-NEXT:     if (unsafe { x }) < unsafe { y } {
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             x = [[V_23]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_21]] = [[V_22]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut [[V_24:__v[0-9]+]]: i32 = unsafe { y };
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: i32 = [[V_24]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             y = [[V_25]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_21]] = [[V_24]];
// CHECK-NEXT:     }
// CHECK-NEXT:     z = [[V_21]];
// CHECK-NEXT:     if z != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { y }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test6() {
// CHECK-NEXT:     let mut xx: i32 = 3 as i32;
// CHECK-NEXT:     let mut z: i32 = 0 as i32;
// CHECK-NEXT:     init_xy();
// CHECK-NEXT:     let mut [[V_26:__v[0-9]+]]: i32;
// CHECK-NEXT:     if xx < unsafe { y } {
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: i32 = unsafe { x };
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: i32 = [[V_27]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             x = [[V_28]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_26]] = [[V_27]];
// CHECK-NEXT:     } else {
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: i32 = unsafe { y };
// CHECK-NEXT:         let mut [[V_30:__v[0-9]+]]: i32 = [[V_29]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             y = [[V_30]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_26]] = [[V_29]];
// CHECK-NEXT:     }
// CHECK-NEXT:     z = [[V_26]];
// CHECK-NEXT:     if z != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { y }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     test1();
// CHECK-NEXT:     test2();
// CHECK-NEXT:     test3();
// CHECK-NEXT:     test4();
// CHECK-NEXT:     test5();
// CHECK-NEXT:     test6();
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
