void abort(void);
void exit(int);

int sum;

void g(int i) { sum += i; }

void f(int j) {
  int i;

  for (i = 0; i < 9; i++) {
    j++;
    g(j);
    j = 9;
  }
}

int main() {
  f(0);
  if (sum != 81)
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
// CHECK-NEXT: static mut sum: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn g(mut i: i32) {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { sum };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + i;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sum = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut j: i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (9 as i32) {
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         j = [[V_4]];
// CHECK-NEXT:         g(j);
// CHECK-NEXT:         j = 9 as i32;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f(0 as i32);
// CHECK-NEXT:     if (unsafe { sum }) != (81 as i32) {
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
