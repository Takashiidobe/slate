void abort(void);
void exit(int);

void fail1(void) { abort(); }
void fail2(void) { abort(); }
void fail3(void) { abort(); }
void fail4(void) { abort(); }

void foo(long x) {
  switch (x) {
  case -6:
    fail1();
    break;
  case 0:
    fail2();
    break;
  case 1:
  case 2:
    break;
  case 3:
  case 4:
  case 5:
    fail3();
    break;
  default:
    fail4();
    break;
  }
  switch (x) {

  case -3:
    fail1();
    break;
  case 0:
  case 4:
    fail2();
    break;
  case 1:
  case 3:
    break;
  case 2:
  case 8:
    abort();
    break;
  default:
    fail4();
    break;
  }
}

int main(void) {
  foo(1);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn fail1() {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn fail2() {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn fail3() {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn fail4() {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: i64) {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match x {
// CHECK-NEXT:             -6 => {
// CHECK-NEXT:                 fail1();
// CHECK-NEXT:             }
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 fail2();
// CHECK-NEXT:             }
// CHECK-NEXT:             1 | 2 => {}
// CHECK-NEXT:             3..=5 => {
// CHECK-NEXT:                 fail3();
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 fail4();
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match x {
// CHECK-NEXT:             -3 => {
// CHECK-NEXT:                 fail1();
// CHECK-NEXT:             }
// CHECK-NEXT:             0 | 4 => {
// CHECK-NEXT:                 fail2();
// CHECK-NEXT:             }
// CHECK-NEXT:             1 | 3 => {}
// CHECK-NEXT:             2 | 8 => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 fail4();
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo((1 as i32) as i64);
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
