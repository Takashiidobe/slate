/* PR rtl-optimization/68376 */

extern void abort(void);

__attribute__((noinline, noclone)) int f1(int x) { return x < 0 ? ~x : x; }

__attribute__((noinline, noclone)) int f2(int x) { return x < 0 ? x : ~x; }

__attribute__((noinline, noclone)) int f3(int x) { return x <= 0 ? ~x : x; }

__attribute__((noinline, noclone)) int f4(int x) { return x <= 0 ? x : ~x; }

__attribute__((noinline, noclone)) int f5(int x) { return x >= 0 ? ~x : x; }

__attribute__((noinline, noclone)) int f6(int x) { return x >= 0 ? x : ~x; }

__attribute__((noinline, noclone)) int f7(int x) { return x > 0 ? ~x : x; }

__attribute__((noinline, noclone)) int f8(int x) { return x > 0 ? x : ~x; }

int main() {
  if (f1(5) != 5 || f1(-5) != 4 || f1(0) != 0)
    abort();
  if (f2(5) != -6 || f2(-5) != -5 || f2(0) != -1)
    abort();
  if (f3(5) != 5 || f3(-5) != 4 || f3(0) != -1)
    abort();
  if (f4(5) != -6 || f4(-5) != -5 || f4(0) != 0)
    abort();
  if (f5(5) != -6 || f5(-5) != -5 || f5(0) != -1)
    abort();
  if (f6(5) != 5 || f6(-5) != 4 || f6(0) != 0)
    abort();
  if (f7(5) != -6 || f7(-5) != -5 || f7(0) != 0)
    abort();
  if (f8(5) != 5 || f8(-5) != 4 || f8(0) != -1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/68376
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut x: i32) -> i32 {
// CHECK-NEXT:     if x < (0 as i32) { !x } else { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut x: i32) -> i32 {
// CHECK-NEXT:     if x < (0 as i32) { x } else { !x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut x: i32) -> i32 {
// CHECK-NEXT:     if x <= (0 as i32) { !x } else { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f4(mut x: i32) -> i32 {
// CHECK-NEXT:     if x <= (0 as i32) { x } else { !x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f5(mut x: i32) -> i32 {
// CHECK-NEXT:     if x >= (0 as i32) { !x } else { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f6(mut x: i32) -> i32 {
// CHECK-NEXT:     if x >= (0 as i32) { x } else { !x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f7(mut x: i32) -> i32 {
// CHECK-NEXT:     if x > (0 as i32) { !x } else { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f8(mut x: i32) -> i32 {
// CHECK-NEXT:     if x > (0 as i32) { x } else { !x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f1(5 as i32) != (5 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = f1(-(5 as i32)) != (4 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = f1(0 as i32) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f2(5 as i32) != -(6 as i32) {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = f2(-(5 as i32)) != -(5 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = f2(0 as i32) != -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f3(5 as i32) != (5 as i32) {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = f3(-(5 as i32)) != (4 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_4]] {
// CHECK-NEXT:         [[V_5]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = f3(0 as i32) != -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f4(5 as i32) != -(6 as i32) {
// CHECK-NEXT:         [[V_6]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = f4(-(5 as i32)) != -(5 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_6]] {
// CHECK-NEXT:         [[V_7]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = f4(0 as i32) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_7]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f5(5 as i32) != -(6 as i32) {
// CHECK-NEXT:         [[V_8]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_8]] = f5(-(5 as i32)) != -(5 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_8]] {
// CHECK-NEXT:         [[V_9]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_9]] = f5(0 as i32) != -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_9]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f6(5 as i32) != (5 as i32) {
// CHECK-NEXT:         [[V_10]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_10]] = f6(-(5 as i32)) != (4 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_10]] {
// CHECK-NEXT:         [[V_11]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_11]] = f6(0 as i32) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_11]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f7(5 as i32) != -(6 as i32) {
// CHECK-NEXT:         [[V_12]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_12]] = f7(-(5 as i32)) != -(5 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_12]] {
// CHECK-NEXT:         [[V_13]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_13]] = f7(0 as i32) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_13]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f8(5 as i32) != (5 as i32) {
// CHECK-NEXT:         [[V_14]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_14]] = f8(-(5 as i32)) != (4 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_14]] {
// CHECK-NEXT:         [[V_15]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_15]] = f8(0 as i32) != -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_15]] {
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
