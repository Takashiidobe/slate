/* { dg-do run } */
/* PR rtl-optimization/28634.  On targets with delayed branches,
   dbr_schedule could do the next iteration's addition in the
   branch delay slot, then subtract the value again if the branch
   wasn't taken.  This can lead to rounding errors.  */
void   abort(void);
void   exit(int);
double x = -0x1.0p53;
double y = 1;
int    main(void) {
  while (y > 0)
    y += x;
  if (y != x + 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-do run }
// CHECK-NEXT: //
// CHECK-NEXT: // PR rtl-optimization/28634.  On targets with delayed branches,
// CHECK-NEXT: // dbr_schedule could do the next iteration's addition in the
// CHECK-NEXT: // branch delay slot, then subtract the value again if the branch
// CHECK-NEXT: // wasn't taken.  This can lead to rounding errors.
// CHECK-NEXT: static mut x: f64 = -9007199254740992.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut y: f64 = (1 as i32) as f64;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { y }) > ((0 as i32) as f64) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: f64 = unsafe { y };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: f64 = [[V_1]] + unsafe { x };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             y = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { y }) != (unsafe { x }) + ((1 as i32) as f64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
