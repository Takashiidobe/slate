/* This is reduced from sel-sched.cc which was noticed was being miscompiled
 * too. */
int g(int min_need_stall) __attribute__((__noipa__));
int g(int min_need_stall) {
  return min_need_stall < 0 ? 1
                            : ((min_need_stall) < (1) ? (min_need_stall) : (1));
}
int main(void) {
  for (int i = -100; i <= 100; i++) {
    int t = g(i);
    if (t != (i != 0))
      __builtin_abort();
  }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// This is reduced from sel-sched.cc which was noticed was being miscompiled
// CHECK-NEXT: /// too.
// CHECK-NEXT: fn g(mut min_need_stall: i32) -> i32 {
// CHECK-NEXT:     if min_need_stall < (0 as i32) {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if min_need_stall < (1 as i32) {
// CHECK-NEXT:             min_need_stall
// CHECK-NEXT:         } else {
// CHECK-NEXT:             1 as i32
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = -(100 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (100 as i32) {
// CHECK-NEXT:         let mut t: i32 = g(i);
// CHECK-NEXT:         if t != ((i != (0 as i32)) as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
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
