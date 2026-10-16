void abort(void);
void exit(int);

void f(int i, int j, int radius, int width, int N) {
  const int diff = i - radius;
  const int lowk = (diff > 0 ? diff : 0);
  int       k;

  for (k = lowk; k <= 2; k++) {
    int idx = ((k - i + radius) * width - j + radius);
    if (idx < 0)
      abort();
  }

  for (k = lowk; k <= 2; k++)
    ;
}

int main(int argc, char **argv) {
  int exc_rad = 2;
  int N       = 8;
  int i;
  for (i = 1; i < 4; i++)
    f(i, 1, exc_rad, 2 * exc_rad + 1, N);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut i: i32, mut j: i32, mut radius: i32, mut width: i32, mut N: i32) {
// CHECK-NEXT:     let mut diff: i32 = i - radius;
// CHECK-NEXT:     let mut lowk: i32 = if diff > (0 as i32) { diff } else { 0 as i32 };
// CHECK-NEXT:     let mut k: i32 = lowk;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while k <= (2 as i32) {
// CHECK-NEXT:         let mut idx: i32 = (k - i + radius) * width - j + radius;
// CHECK-NEXT:         if idx < (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         k = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     k = lowk;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while k <= (2 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         k = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut exc_rad: i32 = 2 as i32;
// CHECK-NEXT:     let mut N: i32 = 8 as i32;
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (4 as i32) {
// CHECK-NEXT:         f(i, 1 as i32, exc_rad, (2 as i32) * exc_rad + (1 as i32), N);
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
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
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
