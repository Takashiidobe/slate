void abort(void);
void exit(int);

int f(int i) { return 12; }
int g() { return 0; }

int main(void) {
  int i, s;

  for (i = 0; i < 32; i++) {
    s = f(i);

    if (i == g())
      s = 42;
    if (i == 0 || s == 12)
      ;
    else
      abort();
  }

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f(mut i: i32) -> i32 {
// CHECK-NEXT:     12 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn g() -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut s: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (32 as i32) {
// CHECK-NEXT:         s = f(i);
// CHECK-NEXT:         if i == g() {
// CHECK-NEXT:             s = 42 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         if i == (0 as i32) || s == (12 as i32) {
// CHECK-NEXT:             {}
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
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
