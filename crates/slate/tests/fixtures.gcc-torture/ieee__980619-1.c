/* This used to fail on ia32, with or without -ffloat-store.
   It works now, but some people think that's a fluke. */

/* { dg-do run } */
void abort(void);
void exit(int);

int main(void) {
  float reale = 1.0f;
  float oneplus;
  int   i;

  if (sizeof(float) != 4)
    exit(0);

  for (i = 0;; i++) {
    oneplus = 1.0f + reale;
    if (oneplus == 1.0f)
      break;
    reale = reale / 2.0f;
  }
  /* Assumes ieee754 accurate arithmetic above.  */
  if (i != 24)
    abort();
  else
    exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This used to fail on ia32, with or without -ffloat-store.
// CHECK-NEXT: //! It works now, but some people think that's a fluke.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-do run }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut reale: f32 = 1.0f32;
// CHECK-NEXT:     let mut oneplus: f32 = 0 as f32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     if (4 as u64) != (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         oneplus = 1.0f32 + reale;
// CHECK-NEXT:         if oneplus == 1.0f32 {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         reale /= 2.0f32;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     // Assumes ieee754 accurate arithmetic above.
// CHECK-NEXT:     if i != (24 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
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
